#include<vcg/complex/complex.h>
#include<vcg/complex/algorithms/create/platonic.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <iostream>

using namespace vcg;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::Normal3f, vertex::BitFlags>{}; // 色々と変数追加しないと実行時にエラー出る
class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::Normal3f, face::FFAdj, face::BitFlags>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(int argc,char ** argv){
	MyMesh diskMesh;
	
	// Create a simple triangle mesh using just a vector of coords and a vector of indexes
	std::vector<Point3f> coordVec;
	std::vector<Point3i> indexVec;
	coordVec.push_back(Point3f(0,0,0)); // center point
	for(int i=0;i<36;++i) {
		float angleRad = float(i)*M_PI/18.0;
		coordVec.push_back(Point3f(sin(angleRad),cos(angleRad),0));
		indexVec.push_back(Point3i(0,i+1,1+(i+1)%36));
	}
	tri::BuildMeshFromCoordVectorIndexVector(diskMesh,coordVec,indexVec);
	tri::io::ExporterPLY<MyMesh>::Save(diskMesh,"disk.ply",tri::io::Mask::IOM_NONE);
	
	// Create the platonic solids  
	MyMesh platonicMesh;
	tri::Tetrahedron(platonicMesh);
	tri::io::ExporterPLY<MyMesh>::Save(platonicMesh,"tetrahedron.ply",tri::io::Mask::IOM_NONE);
	tri::Octahedron(platonicMesh);
	tri::io::ExporterPLY<MyMesh>::Save(platonicMesh,"octahedron.ply",tri::io::Mask::IOM_NONE);
	tri::Hexahedron(platonicMesh);
	tri::io::ExporterPLY<MyMesh>::Save(platonicMesh,"hexahedron.ply",tri::io::Mask::IOM_NONE);
	tri::Dodecahedron(platonicMesh);
	tri::io::ExporterPLY<MyMesh>::Save(platonicMesh,"dodecahedron.ply",tri::io::Mask::IOM_NONE);
	tri::Icosahedron(platonicMesh);
	tri::io::ExporterPLY<MyMesh>::Save(platonicMesh,"icosahedron.ply",tri::io::Mask::IOM_NONE);
	
	// Procedurally transform a mesh into a solid collection of triangular prisms
	MyMesh facePrismMesh;
	tri::BuildPrismFaceShell(platonicMesh, facePrismMesh, 0.1f, 0.1f, true);
	tri::io::ExporterPLY<MyMesh>::Save(facePrismMesh,"facePrism.ply",tri::io::Mask::IOM_NONE);
	
	return 0;
}
