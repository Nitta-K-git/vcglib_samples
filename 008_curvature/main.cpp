#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <vcg/complex/algorithms/update/curvature.h>

#include <iostream>

#include <vcg/complex/algorithms/create/platonic.h>


using namespace vcg;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::Qualityf, vertex::Curvaturef, vertex::CurvatureDirf, vertex::VFAdj, vertex::BitFlags, vertex::Normal3f>{};
class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::VFAdj, face::FFAdj>{};
class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: <filename>\nuse usagi.ply\n");
	}
	
	// save param
	bool bBinary = false;
	auto flags = tri::io::Mask::IOM_VERTQUALITY;
	
	MyMesh m;
	
	//open ply
	int ret= tri::io::ImporterPLY<MyMesh>::Open(m,(argc<2)?"usagi.ply":argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<MyMesh>::ErrorMsg(ret));
		return -1;
	}
//	tri::Torus(m,30,10);
	
	// calc normal
	tri::UpdateTopology<MyMesh>::FaceFace(m);
	tri::UpdateTopology<MyMesh>::VertexFace(m);
	// calc curvature
	tri::UpdateCurvature<MyMesh>::PerVertex(m);
	for(MyMesh::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){
		MyVertex *pv = &*vi;
		pv->Q() = pv->Kh();
	}
//	for(auto vi:m.vert){
//		vi.Q() = vi.Kh();
//		std::cout << vi.Kh() << ":" << vi.Q() << std::endl;
//	}
	
	// 曲率の値がVertex Qualityに含まれているので、meshlabでcolorize by vertex quality使うと可視化される
	tri::io::ExporterPLY<MyMesh>::Save(m,"out_Kh_PerVertex.ply",flags, bBinary);
	
	tri::UpdateCurvature<MyMesh>::MeanAndGaussian(m);
	for(MyMesh::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){(&*vi)->Q() = (&*vi)->Kh();}
	tri::io::ExporterPLY<MyMesh>::Save(m,"out_Kh_MeanAndGaussian.ply",flags, bBinary);
	
	tri::UpdateCurvature<MyMesh>::PrincipalDirections(m);
	for(MyMesh::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){(&*vi)->Q() = (&*vi)->K1();}
	tri::io::ExporterPLY<MyMesh>::Save(m,"out_K1_PrincipalDirections.ply",flags, bBinary); // torusだとよく分からない
	
	tri::UpdateCurvature<MyMesh>::PrincipalDirectionsNormalCycle(m);
	for(MyMesh::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){(&*vi)->Q() = (&*vi)->K1();}
	tri::io::ExporterPLY<MyMesh>::Save(m,"out_K1_PrincipalDirectionsNormalCycle.ply",flags, bBinary);

	return 0;
}
