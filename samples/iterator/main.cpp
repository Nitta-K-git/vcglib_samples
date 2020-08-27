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
	tri::Tetrahedron(mesh); // create preset mesh data
	
	// Access to face and vertex via iterator
	cout << "iterator" << endl;
	MyMesh::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // iterate each faces
		cout << "face : " << tri::Index(mesh, *fi) << endl; // get face index
		// cout << fi->P(0).X() << endl; // get point
	}
	MyMesh::VertexIterator vi;
	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		cout << "vert : " << tri::Index(mesh, *vi) << endl;
		// cout << vi->P().X() << endl; // get point
	}
	
	// via pointer
	cout << "pointer" << endl;
	MyMesh::FacePointer fp;
	for(int i=0; i<mesh.FN(); ++i){
		fp = &(mesh.face[i]);
		cout << "face : " << tri::Index(mesh, fp) << endl;
	}
	MyMesh::VertexPointer vp;
	for(int i=0; i<mesh.VN(); ++i){
		vp = &(mesh.vert[i]);
		cout << "vert : " << tri::Index(mesh, vp) << endl;
	}
	return 1;
}

