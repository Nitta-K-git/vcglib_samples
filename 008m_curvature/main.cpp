#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <vcg/complex/algorithms/update/curvature.h>

#include <iostream>

#include <vcg/complex/algorithms/create/platonic.h>

#include <common/ml_mesh_type.h>

using namespace vcg;

//class MyFace;
//class MyVertex;

//struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
//class MyVertex  : public Vertex<MyUsedTypes, vertex::Coord3f, vertex::Qualityf, vertex::Curvaturef, vertex::CurvatureDirf, vertex::VFAdj, vertex::BitFlags, vertex::Normal3f>{};
//class MyFace    : public Face  <MyUsedTypes, face::VertexRef, face::VFAdj, face::FFAdj>{};
//class MyMesh    : public vcg::tri::TriMesh<std::vector<MyVertex>, std::vector<MyFace>>{};

float get_K2(float h, float g){
	float a = std::max((float)0., h*h-g); // h^2-gは手法によっては負になる場合があるので0でmaxとる
	return h - sqrt(a);
}

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: <filename>\nuse usagi.ply\n");
	}
	
	// save param
	bool bBinary = false;
	auto flags = tri::io::Mask::IOM_VERTQUALITY;// | tri::io::Mask::IOM_FACEQUALITY;
	
	CMeshO m;
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,(argc<2)?"usagi.ply":argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<CMeshO>::ErrorMsg(ret));
		return -1;
	}
//	tri::Torus(m,30,10);
	
	m.vert.EnableVFAdjacency();
	m.vert.EnableCurvature();
	m.vert.EnableCurvatureDir();
//	m.vert.EnableNormal(); //ocfで定義されていないものをenableにするとエラー
//	m.vert.EnableMark(); // 曲率計算ではEnableにしなくてよい
//	m.vert.EnableQuality(); //ocfで定義されていないものをenableにするとエラー
	
	m.face.EnableFFAdjacency();
	m.face.EnableVFAdjacency();
//	m.face.EnableCurvatureDir();
//	m.face.EnableQuality();
	
	// calc normal
	tri::UpdateTopology<CMeshO>::FaceFace(m);
	tri::UpdateTopology<CMeshO>::VertexFace(m);
	// calc curvature
	tri::UpdateCurvature<CMeshO>::PerVertex(m); // KhとKgしか計算しないので、最大最小は自前で計算
	for(CMeshO::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){
		CMeshO::VertexType *pv = &*vi;
		float H = pv->Kh();
		float G = pv->Kg();
		pv->Q() = get_K2(H,G);
//		pv->Q() = pv->K2();
	}
//	for(auto vi:m.vert){
//		vi.Q() = vi.Kh();
//		std::cout << vi.Kh() << ":" << vi.Q() << std::endl;
//	}
	
	// 曲率の値がVertex Qualityに含まれているので、meshlabでcolorize by vertex quality使うと可視化される
	tri::io::ExporterPLY<CMeshO>::Save(m,"outm_K2_PerVertex.ply",flags, bBinary);
	
	tri::UpdateCurvature<CMeshO>::MeanAndGaussian(m); // これも最大最小値は自前計算。これが一番見た目いい感じに計算されてる
	for(CMeshO::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){
//		(&*vi)->Q() = get_K2((&*vi)->Kh(), (&*vi)->Kg()); // このやり方だと値が代入されてない
		CMeshO::VertexType *pv = &*vi;
		float H = pv->Kh();
		float G = pv->Kg();
		pv->Q() = get_K2(H,G);
	}
	tri::io::ExporterPLY<CMeshO>::Save(m,"outm_K2_MeanAndGaussian.ply",flags, bBinary);
	
	tri::UpdateCurvature<CMeshO>::PrincipalDirections(m);
	for(CMeshO::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){(&*vi)->Q() = (&*vi)->K2();}
//	for(CMeshO::FaceIterator fi=m.face.begin(); fi!=m.face.end(); ++fi){(&*fi)->Q() = (&*fi)->K2();} // face qualityには値が入らない
	tri::io::ExporterPLY<CMeshO>::Save(m,"outm_K2_PrincipalDirections.ply",flags, bBinary); // torusだとよく分からない
	
	tri::UpdateCurvature<CMeshO>::PrincipalDirectionsNormalCycle(m);
	for(CMeshO::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){(&*vi)->Q() = (&*vi)->K2();}
//	for(CMeshO::FaceIterator fi=m.face.begin(); fi!=m.face.end(); ++fi){(&*fi)->Q() = (&*fi)->K2();}
	tri::io::ExporterPLY<CMeshO>::Save(m,"outm_K2_PrincipalDirectionsNormalCycle.ply",flags, bBinary);

	return 0;
}
