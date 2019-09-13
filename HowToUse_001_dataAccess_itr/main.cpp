#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>
#include<vcg/simplex/face/pos.h>

class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(){
	MyMesh mesh;
	vcg::tri::Octahedron(mesh); // 正八面体のデータを作成
	
	
	MyMesh::FacePointer fp;
	MyMesh::VertexPointer vp;
	fp = &mesh.face[1];
	vp = &mesh.vert[1];
	std::cout << vcg::tri::Index(mesh, fp) << "," << std::endl;
	std::cout << vcg::tri::Index(mesh, vp) << "," << std::endl;
	return 1;
}
