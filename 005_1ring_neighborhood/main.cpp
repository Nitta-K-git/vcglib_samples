#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>

/*
 * 1ring頂点の巡回をPosクラスを用いて実装する 基本的に時計回り
 * 任意の頂点指定
 * FlipVで反対側の頂点へ移動（そこが1個目の頂点）
 * E>Vで2個目の頂点へ
 * 同じ頂点の隣の面へ移動 E>F
 * E>Vで反対側の頂点へ(3個目)
 * 繰り返し
 * 
 * 別の方法
 * 面巡回で実装してから指定頂点の反対側の頂点を取得してまた戻る
*/

using namespace vcg;

class MyFace;
//class MyEdge;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::VFAdj>{};
class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::FFAdj, face::VFAdj>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\nuse octahedron.ply\n");
	}
	
	MyMesh m;
	//open ply
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,(argc<2)?"octahedron.ply":argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
	
	// ポインタからインデックス返す配列作成
	vcg::SimpleTempData<typename MyMesh::VertContainer, unsigned int> vert_indices(m.vert);
	vcg::SimpleTempData<typename MyMesh::FaceContainer, unsigned int> face_indices(m.face);
	unsigned int i=0;
	MyMesh::VertexIterator vi;
	for(i=0,vi=m.vert.begin(); vi!=m.vert.end(); ++vi,++i){
		vert_indices[vi] = i; // vertex point to vertex index
	}
	MyMesh::FaceIterator fi;
	for(i=0,fi=m.face.begin(); fi!=m.face.end(); ++fi,++i){
		face_indices[fi] = i; // face point to face index
	}
	
	tri::UpdateTopology<MyMesh>::FaceFace(m);
	tri::UpdateTopology<MyMesh>::VertexFace(m); // 初期化必要
	
	MyMesh::VertexType *v = &(m.vert[0]);
	MyMesh::FaceType *pf = v->VFp();
//	MyMesh::FaceType *pf = &(m.face[0]);
	face::Pos<MyMesh::FaceType> p;
//	p.Set(pf, 0, pf->V(0)); // 面ポインタ、辺インデックス、頂点ポインタで一意の場所を決定する(辺インデックスは頂点インデックスを反時計回りにずらしたもの)
//		// なのでp.Set(pf, 1, pf->V(0));とかに設定すると辺と頂点が対向する位置になるのでエラーになる
	p.Set(pf, v); // 面ポインタ、頂点ポインタで一意の場所を決定(辺番号は頂点番号の一つ前になる)
	std::cout << "center vertex:" << vert_indices[v] << std::endl;
	p.FlipV();
	MyMesh::VertexType *first_vt = p.V();
	std::cout << vert_indices[first_vt] << ",";
	p.FlipE();
	p.FlipV();
	std::cout << vert_indices[p.V()] << ",";
	while(true){
		p.FlipE();
		p.FlipF();
		p.FlipE();
		p.FlipV();
		if(p.V() == first_vt){break;}
		std::cout << vert_indices[p.V()] << ",";
	}
	std::cout << std::endl << "---------------------------" << std::endl;
	
	// 方法２
	pf = v->VFp();
	p.Set(pf, v);
	std::cout << "face::" << face_indices[pf] << ",";
	std::cout << "center::" << vert_indices[v] << std::endl;
//	p.FlipF(); // 時計回りに巡回するために反転させる → 最後の番号が抜けてしまうので却下。最後に逆順にすることで対応する
	while(true){
		p.FlipV();
		std::cout << vert_indices[p.V()] << ",";
		p.FlipV(); // 元に戻る
		p.NextE();
		if(pf == p.F()){break;}
	}
	std::cout << std::endl;
	
	return 0;
}
