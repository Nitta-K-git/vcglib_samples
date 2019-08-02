#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <wrap/io_trimesh/export_stl.h>

#include <iostream>

using namespace vcg;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f>{};
class MyFace    : public Face  <MyUsedTypes, face::VertexRef>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(int argc,char ** argv){
	const char *def_fname = "usagi.ply";
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\n");
	}
	
	MyMesh m;
	
	//open ply
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,(argc<2)?def_fname:argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
	// save ply
	int result;
	bool bBinary = false;
	result = tri::io::ExporterPLY<MyMesh>::Save(m,"out.ply",tri::io::Mask::IOM_NONE, bBinary);
	printf("result ply: %d\n", result);
	// save stl
	result = tri::io::ExporterSTL<MyMesh>::Save(m,"out.stl");
	printf("result stl: %d\n", result);
	return 0;
}
