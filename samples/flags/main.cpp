#include <vcg/complex/algorithms/create/platonic.h>

using namespace vcg;
using namespace std;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<	Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes, 
		vertex::Coord3f, 
		vertex::Normal3f,
		vertex::BitFlags, 
		vertex::VFAdj
		> {};
class MyFace : public Face< MyUsedTypes, 
		face::VertexRef, 
		face::Normal3f,
		face::VFAdj, 
		face::BitFlags
		> {};
class MyMesh : public tri::TriMesh<
		vector<MyVertex>, 
		vector<MyFace> 
		> {};

int main(){
	MyMesh mesh;
	vcg::tri::Tetrahedron(mesh); // create preset mesh data
	
	// set selected flag manually
	MyMesh::VertexPointer vp;
	vp = &(mesh.vert[0]);
	vp->SetS();
	vp = &(mesh.vert[2]);
	vp->SetS();
	
	// get all selected flag state
	MyMesh::VertexIterator vi;
	for(auto &&vi : mesh.vert){
		cout << vi.IsS() << endl;
	}
	return 1;
}

