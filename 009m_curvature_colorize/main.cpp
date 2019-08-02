#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>

#include <iostream>

#include <vcg/complex/algorithms/create/platonic.h>

#include <common/ml_mesh_type.h>

using namespace vcg;

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
	auto flags = tri::io::Mask::IOM_VERTQUALITY | tri::io::Mask::IOM_VERTCOLOR;
	
	CMeshO m;
	const char *fname = "usagi.ply";
//	const char *fname = "non_manifold_sharing_edge.ply"; //error Assertion failed: f->FFp(z)->FFp(f->FFi(z))==f, file C:\vcglib\vcg/simplex/face/pos.h, line 200
//	const char *fname = "non_manifold_same_vertex.ply"; //ok
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,(argc<2)?fname:argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<CMeshO>::ErrorMsg(ret));
		return -1;
	}
	
	m.vert.EnableVFAdjacency();
	m.vert.EnableCurvature();
	m.vert.EnableCurvatureDir();
	
	m.face.EnableFFAdjacency();
	m.face.EnableVFAdjacency();
//	m.face.EnableCurvatureDir();
//	m.face.EnableQuality();
	
	// calc normal
	tri::UpdateTopology<CMeshO>::FaceFace(m);
	tri::UpdateTopology<CMeshO>::VertexFace(m);
	// calc curvature
	tri::UpdateCurvature<CMeshO>::MeanAndGaussian(m); // これも最大最小値は自前計算。これが一番見た目いい感じに計算されてる
	for(CMeshO::VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi){
		CMeshO::VertexType *pv = &*vi;
		float H = pv->Kh();
		float G = pv->Kg();
		pv->Q() = get_K2(H,G);
	}
	// colorize by vertex quality
	std::pair<float, float> minmax;
	minmax = vcg::tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(m);
	float minVal = minmax.first;
	float maxVal = minmax.second;
	std::cout << "min:max = " << minVal << "," << maxVal << std::endl;
	vcg::tri::UpdateColor<CMeshO>::PerVertexQualityRamp(m, minVal, maxVal);
	
	tri::io::ExporterPLY<CMeshO>::Save(m,"outm_K2_MeanAndGaussian_col.ply",flags, bBinary);
	return 0;
}
