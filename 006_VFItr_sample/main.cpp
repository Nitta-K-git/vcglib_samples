#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

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
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,"non_manifold_sharing_edge.ply");
	if(ret!=0){
		printf("Unable to open '%s'\n",tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
	tri::UpdateTopology<MyMesh>::FaceFace(m);
	tri::UpdateTopology<MyMesh>::VertexFace(m);

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
		std::cout << face_indices[p.F()] << std::endl;
		p.NextE();
		if(pf == p.F()){
			std::cout << "end" << std::endl;
			break;
		}
		// >> 1 2 2
	}
	std::cout << "---------------------" << std::endl;
	// VFItratorで巡回
//	MyMesh::VertexType *pv = &(m.vert[2]);
	MyVertex *pv = &(m.vert[0]);	// 上と同じ
	face::VFIterator<MyMesh::FaceType> vfi(pv);
	for(; !vfi.End(); ++vfi){
		MyFace *f = vfi.F();
		std::cout << face_indices[f] << std::endl;
		// >> 2 1 0
	}
	return 0;
}
