#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <iostream>

using namespace vcg;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::Color4b, vertex::Normal3f, vertex::Qualityf>{};
class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::Color4b, face::Normal3f>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\nload usagi.ply\n");
	}
	
	MyMesh m;
	
	//open ply
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,(argc<2)?"usagi.ply":argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
	// calc normal
	tri::UpdateNormal<MyMesh>::NormalizePerVertex(m);
	tri::UpdateNormal<MyMesh>::NormalizePerFace(m);
	// set vertex color and quality
	MyMesh::VertexIterator vi;
	for(auto vi=m.vert.begin(); vi!=m.vert.end(); ++vi){
		vi->C()[0] = 100;
		vi->Q() = 0.22f;
	}
	// set face color
	MyMesh::FaceIterator fi;
	for(auto fi=m.face.begin(); fi!=m.face.end(); ++fi){
		fi->C()[1] = 50; 
	}
	// save ply
	int result;
	bool bBinary = false;
	auto flags = tri::io::Mask::IOM_VERTCOLOR|tri::io::Mask::IOM_VERTNORMAL|tri::io::Mask::IOM_FACECOLOR|tri::io::Mask::IOM_VERTQUALITY;
	result = tri::io::ExporterPLY<MyMesh>::Save(m,"out003.ply",flags, bBinary);
	printf("result ply: %d\n", result);
	return 0;
}
