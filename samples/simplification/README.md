# Simplification

メッシュの再分割。いくつかの方法があり、それぞれ使用するヘッダファイルが異なる

## Clustering Decimation

```cpp
#include <vcg/complex/algorithms/clustering.h>

void SimplificationClusteringDecimation(CMeshO &mesh, float grid_size){
	float threshold = grid_size;
	tri::Clustering<CMeshO, tri::AverageColorCell<CMeshO> > ClusteringGrid;
	ClusteringGrid.Init(mesh.bbox,100000,threshold);
	if(mesh.FN() == 0)
		ClusteringGrid.AddPointSet(mesh);
	else
		ClusteringGrid.AddMesh(mesh);
	ClusteringGrid.ExtractMesh(mesh);
	tri::UpdateBounding<CMeshO>::Box(mesh);
	if(mesh.fn>0) {
		tri::UpdateNormal<CMeshO>::PerFaceNormalized(mesh);
		tri::UpdateNormal<CMeshO>::PerVertexAngleWeighted(mesh);
	}
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
	
	{
		CMeshO mesh1;
		tri::Append<CMeshO, CMeshO>::MeshCopy(mesh1, mesh);
		float gridsize = 0.1;
		mesh1.bbox.Diag();
		SimplificationClusteringDecimation(mesh1, gridsize);
		tri::io::ExporterPLY<CMeshO>::Save(mesh,"SimplificationClusteringDecimation.ply",tri::io::Mask::IOM_NONE, false);
	}
	return 1;
}
```

## Edge Collapse for Marching Cube Meshes

```cpp
#include <vcg/complex/algorithms/create/plymc/plymc.h> 
// mingwだとコンパイル時にエラー？ volume.hで_int64が未定義になる(定義漏れ？issueに上げる)

bool SimplificationEdgeCollapseMarchingCube(CMeshO &mesh, float absoluteError){
	if (mesh.fn == 0){
		return false;
	}
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableVFAdjacency();
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	
	mesh.face.EnableFFAdjacency();
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	
	mesh.vert.EnableMark();
	
	int res = tri::MCSimplify<CMeshO>(mesh, absoluteError, false);
	if (res !=1){
		return false;
	}
	
	tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	tri::Clean<CMeshO>::RemoveTVertexByFlip(mesh,20,true);
	tri::Clean<CMeshO>::RemoveFaceFoldByFlip(mesh);
	tri::Allocator<CMeshO>::CompactVertexVector(mesh);
	tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
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
	
	{
		CMeshO mesh1;
		tri::Append<CMeshO, CMeshO>::MeshCopy(mesh1, mesh);
		float abs_error = 0.1;
		SimplificationEdgeCollapseMarchingCube(mesh1, abs_error);
		tri::io::ExporterPLY<CMeshO>::Save(mesh,"SimplificationEdgeCollapseMarchingCube.ply",tri::io::Mask::IOM_NONE, false);
	}
	return 1;
}

```

## Quadric Edge Collapse Decimation

MeshLabのソースファイルから`quadric_simp`のヘッダファイルとcppファイルをコピーする

```cpp
#include "quadric_simp.h" // for simplification collapse edge

void SimplificationQuadricEdgeCollapseDecimation(CMeshO &mesh, int targetFaceNum, double qualityThreshold, bool selectedOnly, bool autoClean){
	int TargetFaceNum = targetFaceNum;
	//double TargetPerc = 0;
	double QualityThr = qualityThreshold;
	bool PreserveBoundary = false;
	double BoundaryWeight = 1.0;
	bool PreserveNormal = false;
	bool PreserveTopology = false;
	bool OptimalPlacement = true;
	bool PlanarQuadric = false;
	double PlanarWeight = 0.001;
	bool QualityWeight = false;
	bool AutoClean = autoClean;
	bool Selected = selectedOnly;
	
	if(TargetFaceNum<=0){
		TargetFaceNum = mesh.FN()/2;
	}
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableVFAdjacency();
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	mesh.vert.EnableMark();
	
	tri::UpdateFlags<CMeshO>::FaceBorderFromVF(mesh);

	tri::TriEdgeCollapseQuadricParameter pp;
	pp.QualityThr = QualityThr;
	pp.PreserveBoundary = PreserveBoundary;
	pp.BoundaryQuadricWeight = pp.BoundaryQuadricWeight * BoundaryWeight;
	pp.PreserveTopology = PreserveTopology;
	pp.QualityWeight = QualityWeight;
	pp.NormalCheck = PreserveNormal;
	pp.OptimalPlacement = OptimalPlacement;
	pp.QualityQuadric = PlanarQuadric;
	pp.QualityQuadricWeight = PlanarWeight;

	QuadricSimplification(mesh,TargetFaceNum,Selected,pp);

	if(AutoClean)
	{
		int nullFaces=tri::Clean<CMeshO>::RemoveFaceOutOfRangeArea(mesh,0);
		int deldupvert=tri::Clean<CMeshO>::RemoveDuplicateVertex(mesh);
		int delvert=tri::Clean<CMeshO>::RemoveUnreferencedVertex(mesh);
		//mesh.clearDataMask(MeshModel::MM_FACEFACETOPO );
		mesh.face.DisableFFAdjacency();
		tri::Allocator<CMeshO>::CompactVertexVector(mesh);
		tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	}

	//mesh.UpdateBoxAndNormals();
	tri::UpdateBounding<CMeshO>::Box(mesh);
	if(mesh.fn>0) {
		tri::UpdateNormal<CMeshO>::PerFaceNormalized(mesh);
		tri::UpdateNormal<CMeshO>::PerVertexAngleWeighted(mesh);
	}
	
	tri::UpdateNormal<CMeshO>::NormalizePerFace(mesh);
	tri::UpdateNormal<CMeshO>::PerVertexFromCurrentFaceNormal(mesh);
	tri::UpdateNormal<CMeshO>::NormalizePerVertex(mesh);
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
	
	{
		CMeshO mesh1;
		tri::Append<CMeshO, CMeshO>::MeshCopy(mesh1, mesh);
		int targetFaceNum = 0;
		bool selectedOnly = false;
		bool autoClean = true;
		double qualityThreshold = 0.3;
		SimplificationQuadricEdgeCollapseDecimation(mesh1, targetFaceNum, qualityThreshold, selectedOnly, autoClean);	
		tri::io::ExporterPLY<CMeshO>::Save(mesh,"SimplificationQuadricEdgeCollapseDecimation.ply",tri::io::Mask::IOM_NONE, false);
	}
	return 1;
}
```

