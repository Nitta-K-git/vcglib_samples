#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>

class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(){
	MyMesh mesh;
	vcg::tri::Octahedron(mesh); // create preset mesh data
	
	MyMesh::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // iterate each faces
		std::cout << vcg::tri::Index(mesh, *fi) << ": "; // get face index
		for(int i=0; i<fi->VN(); ++i){ // iterate each vertex of face
			std::cout << vcg::tri::Index(mesh, fi->V(i)) << ","; // get vertex index
		}
		std::cout << std::endl;
	}
	return 1;
}

