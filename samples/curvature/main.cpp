#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

using namespace vcg;
using namespace std;

int deleteNonManifold(CMeshO &mesh);

bool isNonManifold(CMeshO &mesh){
	int cnt = 0;
	cnt += tri::Clean<CMeshO>::CountNonManifoldVertexFF(mesh, false);
	cnt += tri::Clean<CMeshO>::CountNonManifoldEdgeFF(mesh, false);
	return cnt>0;
}

int setDeleteFlagToSelectedFaceVert(CMeshO &mesh){
	int vvn = mesh.vn;
	int ffn = mesh.fn;
	for (CMeshO::FaceIterator fi = mesh.face.begin(); fi != mesh.face.end(); ++fi)
		if (!(*fi).IsD() && (*fi).IsS())
			tri::Allocator<CMeshO>::DeleteFace(mesh, *fi);
	for (CMeshO::VertexIterator vi = mesh.vert.begin(); vi != mesh.vert.end(); ++vi)
		if (!(*vi).IsD() && (*vi).IsS())
			tri::Allocator<CMeshO>::DeleteVertex(mesh, *vi);
	return vvn - mesh.vn + ffn - mesh.fn;
}

int deleteUnreferencedVert(CMeshO &mesh, bool delDuplicate){
	int dup = 0;
	if(delDuplicate){
		dup = tri::Clean<CMeshO>::RemoveDuplicateVertex(mesh);			// STLとかの頂点が重複定義されるものは統合する
	}
	int delvert = tri::Clean<CMeshO>::RemoveUnreferencedVertex(mesh);
	if(delvert>0 || dup>0){
		tri::Allocator<CMeshO>::CompactFaceVector(mesh);
		tri::Allocator<CMeshO>::CompactVertexVector(mesh);
	}
	return delvert;
}
int deleteRelatedFaceAndVert(CMeshO &mesh){
	// set select flag to faces if there is selected vertex
	tri::UpdateSelection<CMeshO>::FaceClear(mesh);
	tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(mesh);
	
	int cnt = setDeleteFlagToSelectedFaceVert(mesh);
	if(cnt>0){
		// delete elements which have delete flag
		tri::Allocator<CMeshO>::CompactEveryVector(mesh);
		tri::UpdateTopology<CMeshO>::FaceFace(mesh);
		tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	}
	return cnt;
}
int deleteNonManifold_Vert(CMeshO &mesh){
	int ret = 0;
	int cnt = tri::Clean<CMeshO>::CountNonManifoldVertexFF(mesh, true); // Select non manifold vertices
	if(cnt>0){
		ret = deleteRelatedFaceAndVert(mesh);
	}
	return ret;
}
int deleteNonManifold_Edge(CMeshO &mesh){
	int ret = 0;
	int cnt = tri::Clean<CMeshO>::CountNonManifoldEdgeFF(mesh, true); // Select faces and vertices with non manifold edges
	if(cnt>0){
		ret = deleteRelatedFaceAndVert(mesh);
	}
	return cnt;
}

int deleteNonManifold(CMeshO &mesh){
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	tri::UpdateNormal<CMeshO>::PerFace(mesh);
	tri::UpdateNormal<CMeshO>::NormalizePerFace(mesh);
	
	int sum=0;
	sum += deleteNonManifold_Edge(mesh); // 構造上1回削除すればなくなるはず
	
	while(1){ // 削除した結果、別の頂点がnon manifoldの状態になる可能性があるためループ処理する
		int cnt = deleteNonManifold_Vert(mesh);
		sum += cnt;
		if(cnt==0){break;}
	}
	
	deleteUnreferencedVert(mesh, false); // 面に所属しなくなった頂点を削除
	tri::UpdateBounding<CMeshO>::Box(mesh);
	if(mesh.fn>0) {
		tri::UpdateNormal<CMeshO>::PerFaceNormalized(mesh);
		tri::UpdateNormal<CMeshO>::PerVertexAngleWeighted(mesh);
	}
	return sum;
}

void set_color_by_Q_value(CMeshO &mesh){
	tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh);
}

bool calc_min_curvature(CMeshO &mesh){
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();
	mesh.vert.EnableCurvature();
	mesh.vert.EnableCurvatureDir();
	
	// delete invalid vertices and faces
	// compactVert(mesh);
	
	// calc normal
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	// calc curvature
	tri::UpdateCurvature<CMeshO>::MeanAndGaussian(mesh);
	for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		CMeshO::VertexType *pv = &*vi;
		float H = pv->Kh();
		float G = pv->Kg();
		float a = std::max(0.f, H*H-G); // h^2-gは手法によっては負になる場合があるので0でmaxとる
		MESHLAB_SCALAR sq = sqrt(a);
		pv->Q() = H-sq; // minimum curvature
		//pv->Q() = H+sq; // maximum curvature
	}
	std::pair<float,float> curv_minmax = tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(mesh);
	float curv_min = curv_minmax.first;
	float curv_max = curv_minmax.second;
	printf("curvature min,max = (%f, %f)\n", curv_min, curv_max);
	tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh, curv_min, curv_max);
	
	//set_color_by_Q_value(mesh);
	return true;
}

int main(int argc, char ** argv){
	CMeshO mesh;
	
	if(argc<2){
		cout << "add filename" << endl;
		return 0;
	}
	
	int err = tri::io::Importer<CMeshO>::Open(mesh, argv[1]);
	if (err) { // all the importers return 0 in case of success
		printf("Error in reading %s: '%s'\n", argv[1], tri::io::Importer<CMeshO>::ErrorMsg(err));
		exit(-1);
	}
	
	calc_min_curvature(mesh);
	
	int result;
	bool bBinary = false;
	result = tri::io::ExporterPLY<CMeshO>::Save(mesh,"out.ply",tri::io::Mask::IOM_VERTCOLOR, bBinary);
	printf("result ply: %d\n", result);
	return 1;
}

