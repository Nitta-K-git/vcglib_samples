[Basic concepts](http://vcg.isti.cnr.it/vcglib/basic_concepts.html)



```cpp
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
```

