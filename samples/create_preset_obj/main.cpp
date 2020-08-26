#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>

using namespace vcg;
using namespace std;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<	Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes, vertex::Coord3f, vertex::BitFlags, vertex::VFAdj> {};
class MyFace : public Face< MyUsedTypes, face::VertexRef, face::VFAdj, face::BitFlags> {};
class MyMesh : public tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};


int main(){
	MyMesh mesh;
	tri::Tetrahedron(mesh);
	tri::Square(mesh);
	tri::Hexahedron(mesh);
	tri::Octahedron(mesh);
	tri::Dodecahedron(mesh);
	tri::Icosahedron(mesh);
	tri::Sphere(mesh, 3);
	return 1;
}

