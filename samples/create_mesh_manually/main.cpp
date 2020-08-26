#include <vcg/complex/algorithms/create/platonic.h>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_stl.h>

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

	// Create a simple triangles mesh using just a vector of coords and a vector of indexes
	vector<Point3f> coordVec;
	vector<Point3i> indexVec;
	
	coordVec.push_back(Point3f(0,0,0));
	coordVec.push_back(Point3f(10,0,0));
	coordVec.push_back(Point3f(0,10,0));
	coordVec.push_back(Point3f(0,0,10));
	
	indexVec.push_back(Point3i(0,1,3));
	indexVec.push_back(Point3i(0,2,1));
	indexVec.push_back(Point3i(0,3,2));
	indexVec.push_back(Point3i(1,2,3));
	
	tri::BuildMeshFromCoordVectorIndexVector(mesh,coordVec,indexVec);
	tri::io::ExporterSTL<MyMesh>::Save(mesh, "out.stl");
	return 1;
}

