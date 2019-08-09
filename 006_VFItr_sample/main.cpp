#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>
#include <vcg/simplex/face/topology.h>

#include <iostream>

/*
 * VFIteratorを使うと任意の頂点に属する面を巡回することができる(non manifoldでも可能)
*/

using namespace vcg;

class MyFace;
//class MyEdge;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::VFAdj>{};
class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::FFAdj, face::VFAdj>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(){
	MyMesh m;
	//open ply
//	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,"non_manifold_same_vertex.ply");
//	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,"non_manifold_sharing_edge.ply");
//	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,"octahedron_ascii.ply");
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,"sample_face.ply");
	
	if(ret!=0){
		printf("Unable to open '%s'\n",tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
	tri::UpdateTopology<MyMesh>::FaceFace(m);
	tri::UpdateTopology<MyMesh>::VertexFace(m);

	// ポインタからインデックス返す配列作成
//	vcg::SimpleTempData<typename MyMesh::VertContainer, unsigned int> vert_indices(m.vert);
//	vcg::SimpleTempData<typename MyMesh::FaceContainer, unsigned int> face_indices(m.face);
//	unsigned int i=0;
//	MyMesh::VertexIterator vi;
//	for(i=0,vi=m.vert.begin(); vi!=m.vert.end(); ++vi,++i){
//		vert_indices[vi] = i; // vertex point to vertex index
//	}
//	MyMesh::FaceIterator fi;
//	for(i=0,fi=m.face.begin(); fi!=m.face.end(); ++fi,++i){
//		face_indices[fi] = i; // face point to face index
//	}
	
	// dump all face
//	for(MyMesh::FaceIterator fi = m.face.begin(); fi!=m.face.end(); ++fi){
////		MyMesh::FaceType *pf = &*fi;
////		std::cout << pf << std::endl;
//		std::cout << face_indices[fi] << std::endl;
//	}
//	std::cout << "---------------------" << std::endl;
	
//	MyMesh::FaceType *pf = &*fi;
	// Posで巡回
	MyMesh::FaceType *pf = &(m.face[1]);
	face::Pos<MyMesh::FaceType> p;
	p.Set(pf, 0, pf->V(0));
	while(true){
//		std::cout << face_indices[p.F()] << std::endl;
		std::cout << vcg::tri::Index(m, p.F()) << ", ";
		p.NextE();
		if(pf == p.F()){
			std::cout << "end" << std::endl;
			break;
		}
		// >> 1 2 2
	}
	
	// VFItratorで巡回
//	MyMesh::VertexType *pv = &(m.vert[2]);
	MyVertex *pv = &(m.vert[0]);	// 上と同じ
	face::VFIterator<MyMesh::FaceType> vfi(pv);
	for(; !vfi.End(); ++vfi){
		MyFace *f = vfi.F();
//		std::cout << face_indices[f] << std::endl;
		std::cout << vcg::tri::Index(m, f) << ", ";
		// >> 2 1 0
	}
	std::cout << std::endl;
	
	// 1ring頂点の巡回
	// 順番は保証されない "C:\Users\nitta\Documents\GitLab\shiken_tooth_segment_gui\bin\usagi_test.ply" の e.g. 177番頂点
	// http://www.vcglib.net/group__face.html#ga5bdbed96cd7b3eaedde86cd822ab3055
	std::vector<MyVertex::VertexType *> vecvt;
	MyMesh::VertexType *vt = &(m.vert[0]);
	face::VVStarVF<MyFace>(vt, vecvt);
	std::cout <<"VVStarVF: ";
	for(int i=0; i<vecvt.size(); ++i){
		std::cout << tri::Index(m, vecvt[i]) << ", ";
	}
	std::cout << std::endl;
	
	// posで位置指定するタイプ。Orderedは時計回りに整列？
	p.Set(pf, 0, pf->V(0));
	face::VVOrderedStarFF<MyFace>(p, vecvt);
	std::cout <<"VVOrderedStarFF: ";
	for(int i=0; i<vecvt.size(); ++i){
		std::cout << tri::Index(m, vecvt[i]) << ", ";
	}
	std::cout << std::endl;
	
	{
		// posの指定は面倒→頂点から所属する面のポインタを取得する
		MyMesh::VertexType *vt = &(m.vert[0]);
		face::VFIterator<MyFace> vfi(vt);
		face::Pos<MyMesh::FaceType> p;
		p.Set(vfi.f, vt);
		face::VVOrderedStarFF<MyFace>(p, vecvt);
		std::cout <<"VVOrderedStarFF2: ";
		for(int i=0; i<vecvt.size(); ++i){
			std::cout << tri::Index(m, vecvt[i]) << ", ";
		}
		std::cout << std::endl;
	}
	
	// 1ringのさらにn回り外周も含めたものを返す
	vt = &(m.vert[156]);
	face::VVExtendedStarVF<MyFace>(vt, 2, vecvt); //
	std::cout <<"VVExtendedStarVF: ";
	for(int i=0; i<vecvt.size(); ++i){
		std::cout << tri::Index(m, vecvt[i]) << ", ";
	}
	std::cout << std::endl;
	
	return 0;
}
