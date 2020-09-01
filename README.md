# vcglib_samples
sample codes of vcglib and MeshLab function.

# Introduction

```
The Visualization and Computer Graphics Library (VCG for short) is a open source portable C++ templated library for manipulation, processing and displaying with OpenGL of triangle and tetrahedral meshes.
```

- [VCG Library](http://vcg.isti.cnr.it/vcglib/)([Github](https://github.com/cnr-isti-vclab/vcglib))
- [meshlab](https://www.meshlab.net/)([Github](https://github.com/cnr-isti-vclab/meshlab))

# Get started

## Requirements

- [cmake](https://cmake.org/)
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- C++ compiler(visual studio, gcc, ... etc.)
- [Qt](https://www.qt.io/jp) (not necessary but it's easy to visualize)

## Get library from github

Clone vcglib from github. You should select `devel`branch (`master` branch is too old).

```shell
$ git clone -b devel https://github.com/cnr-isti-vclab/vcglib.git
Cloning into 'vcglib'...
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (5/5), done.
remote: Total 42007 (delta 0), reused 1 (delta 0), pack-reused 42002
Receiving objects: 100% (42007/42007), 17.24 MiB | 1.23 MiB/s, done.
Resolving deltas: 100% (28087/28087), done.
Checking out files: 100% (1063/1063), done.

$ cd vcglib

$ git branch
* devel
```

## How to build with cmake

First, you have to set below environment values

- `VCGLIB_DIR` : path/to/vcglib (e.g. `C:/Users/Public/Documents/GitHub/vcglib`)
- `MESHLAB_DIR` : path/to/MeshLab/source (e.g. `C:/Users/Public/Documents/GitHub/meshlab/src`)
- `EIGEN3_INCLUDE_DIR` : path/to/Eigen3 (e.g. `C:/eigen-3.3.7`)

see this [sample](./samples/hello_mesh) for details.

# Samples

## Basic data structure

- template data structure : [CODE](./samples/template)
- template data structure from MeshLab : [CODE](./samples/template_meshlab)
- access mesh vertices/faces : [CODE](./samples/iterator)
- set flag of vertices/faces : [CODE](./samples/flags)
- expand select function : [CODE](./samples/expand_select)
- get adjacent vertices/faces : [CODE](./samples/adjacent)
-  create mesh using just a vector of coords and a vector of indexes : [CODE](./samples/create_mesh_manually)
-  user defined attribute : [CODE](./samples/user_defined_attr)
-  add and delete vertices/faces : [CODE](./samples/add_del_vert_face)
-  set color each vertex/face and save as ply: [CODE](./samples/set_color)



## Read/Write file

-  read and write basic file formats :  [CODE](./samples/read_write_file)
-  create preset object : [CODE](./samples/create_preset_obj)




## Basic algorithm (vcglib functions)

-  convert degree and radian: [CODE](./samples/conv_deg_and_rad)
-  distance between two points : [CODE](./samples/dist_two_points)
-  distance between point and line : [CODE](./samples/dist_point_line)
-  distance between point and plane : [CODE](./samples/dist_point_plane)
-  angle between two vectors : [CODE](./samples/angle_two_vector)
-  quaternion : [CODE](./samples/quaternion)
-  transformation matrix : [CODE](./samples/transformation_matrix)
-  translate matrix with Eigen : [CODE]



## Extra algorithm (implemented in MeshLab)

use MeshLab data structure.

-  template data structure from MeshLab : [CODE](./samples/template_meshlab)
-  calculate curvature : [CODE]
-  hole filing : [CODE]
-  mesh simplification : [CODE]
-  calculate Hausdorff distance : [CODE]
-  save ply with vertex value : [CODE]
-  mesh intersection : [CODE]
-  : [CODE]



## Visualize with Qt



## Visualize with three.js





---

# 残り

## 020_trackball

トラックボール表示するだけのサンプル

視点操作はトラックボール使うと楽にできるが、固定シェーダ前提になる

## 021_trackball_mesh

トラックボールとメッシュデータ表示するサンプル

光源用の処理も追加



## Eigenとの変換

```cpp
template <typename T>
using RMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
typedef RMatrix<MESHLAB_SCALAR> RMatrixm;
using RefRMatrix = Eigen::Ref<const RMatrixm>;
using EVector = Eigen::Vector3f;
```

```cpp
vcg::Quaternionf qua(vcg::Angle(y01,Y),y01^Y);
Matrix44m M;
RMatrixm eigenM = RMatrixm::Identity(4,4); // 初期化時にサイズを指定すること
qua.ToMatrix(M);
M.ToEigenMatrix<RMatrixm>(eigenM);
```

```cpp
Matrix44m M;
M.FromEigenMatrix<Eigen::Ref<const RMatrixm>>(ME);
vcg::tri::UpdatePosition<CMeshO>::Matrix(a.mesh, M);
vcg::tri::UpdateBounding<CMeshO>::Box(a.mesh);
```

## 変換行列の適用

```cpp
tri::UpdatePosition<CMeshO>::Matrix(m->cm, m->cm.Tr,true);
tri::UpdateBounding<CMeshO>::Box(m->cm);
m->cm.Tr.SetIdentity();
```

### メッシュ間の干渉判定

```cpp
// mesh1においてmesh2との干渉面が選択される(mesh2はそのまま)
mesh2.face.EnableMark();
int a = vcg::tri::Clean<CMeshO>::SelectIntersectingFaces(mesh1, mesh2);
```

メッシュの座標変換

```cpp
Matrix44m M;
M.SetTranslate(Point3m(1,1,1));
vcg::tri::UpdatePosition<CMeshO>::Matrix(mesh, M);
vcg::tri::UpdateBounding<CMeshO>::Box(mesh);

```



## header

```cpp
#include "glwidget.h"
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <wrap/io_trimesh/export_stl.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/create/resampler.h>
#include <QRandomGenerator>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
//#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include "quadric_simp.h" // for simplification collapse edge
#include <vcg/complex/algorithms/clustering.h>
#include <vcg/complex/algorithms/create/plymc/plymc.h>

#include <QDebug>

#include <QFileInfo>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <QColor>

#include <vcg/space/triangle3.h>

```

## hausdorff

```cpp
	.def("calc_Hausdorff_dist", [](Vcgmesh &sample, Vcgmesh &target, bool onlySelected){
		// I want to add these flag value and set default arguments, but I don't know how declare default arg in class function.
		bool sampleVert = true;
		bool sampleEdge = false;
		bool sampleFace = false;
		bool sampleFauxEdge = false;
		bool saveSampleFlag = true;
		typedef std::unordered_map<std::string, float> MAP;
		MAP dict;
		CMeshO *mesh_sample = &sample.mesh;
		CMeshO *mesh_target = &target.mesh;
		float distUpperBound = std::numeric_limits<float>::max();//mesh_sample->bbox.Diag()/2.0f;
		
		if(sampleEdge && mesh_sample->fn==0) {
			//Log("Disabled edge sampling. Meaningless when sampling point clouds");
			sampleEdge=false;
		}
		if(sampleFace && mesh_sample->fn==0) {
			//Log("Disabled face sampling. Meaningless when sampling point clouds");
			sampleFace=false;
		}
		mesh_target->face.EnableMark();
		vcg::tri::UpdateNormal<CMeshO>::PerFaceNormalized(*mesh_sample);

		vcg::tri::HausdorffSampler<CMeshO> hs(mesh_target);

		CMeshO samplePtMesh;
		CMeshO closestPtMesh;
		if(saveSampleFlag)
		{
//			closestPtMesh->updateDataMask(MeshModel::MM_VERTCOLOR | MeshModel::MM_VERTQUALITY);
//			samplePtMesh->updateDataMask(MeshModel::MM_VERTCOLOR | MeshModel::MM_VERTQUALITY);
			hs.init(&(samplePtMesh),&(closestPtMesh));
		}
		
		hs.dist_upper_bound = distUpperBound;

		qDebug("Sampled  mesh has %7i vert %7i face",mesh_sample->vn,mesh_sample->fn);
		qDebug("Searched mesh has %7i vert %7i face",mesh_target->vn,mesh_target->fn);
		qDebug("Max sampling distance %f on a bbox diag of %f",distUpperBound,mesh_target->bbox.Diag());

		if(sampleVert)
			vcg::tri::SurfaceSampling<CMeshO,HausdorffSampler<CMeshO> >::VertexUniform(*mesh_sample,hs,mesh_sample->vn, onlySelected); // seleceted flag apply to sample
		if(sampleEdge)
			vcg::tri::SurfaceSampling<CMeshO,HausdorffSampler<CMeshO> >::EdgeUniform(*mesh_sample,hs,mesh_sample->vn,sampleFauxEdge);
		if(sampleFace)
			vcg::tri::SurfaceSampling<CMeshO,HausdorffSampler<CMeshO> >::Montecarlo(*mesh_sample,hs,mesh_sample->vn);

		std::vector<float> vLen;
		if(saveSampleFlag)
		{
//			vcg::tri::UpdateBounding<CMeshO>::Box(samplePtMesh);
//			vcg::tri::UpdateBounding<CMeshO>::Box(closestPtMesh);
//			vcg::tri::UpdateColor<CMeshO>::PerVertexQualityRamp(samplePtMesh);
//			vcg::tri::UpdateColor<CMeshO>::PerVertexQualityRamp(closestPtMesh);
			for(auto v:samplePtMesh.vert){
				vLen.push_back(v.Q());
			}
		}
		
		//Log("Hausdorff Distance computed");
		dict["total_samples"] = hs.n_total_samples;
		dict["min"] = hs.getMinDist();
		dict["max"] = hs.getMaxDist();
		dict["mean"] = hs.getMeanDist();
		dict["RMS"] = hs.getRMSDist();
		//Log("     Sampled %i pts (rng: 0) on %s searched closest on %s",hs.n_total_samples,qUtf8Printable(mm0->label()),qUtf8Printable(mm1->label()));
		//Log("     min : %f   max %f   mean : %f   RMS : %f",hs.getMinDist(),hs.getMaxDist(),hs.getMeanDist(),hs.getRMSDist());
		//float d = mesh_sample->bbox.Diag();
		//Log("Values w.r.t. BBox Diag (%f)",d); // with recpect to
		//Log("     min : %f   max %f   mean : %f   RMS : %f\n",hs.getMinDist()/d,hs.getMaxDist()/d,hs.getMeanDist()/d,hs.getRMSDist()/d);
		return std::tuple<MAP,std::vector<float>>(dict,vLen);
	}, "calculate Hausdorff, sample, target")

```

# simplification

```cpp
	.def("decimate_QuadricEdgeCollapse", [](Vcgmesh &a, int targetFaceNum, bool selectedOnly, bool autoClean){
		double qualityThreshold = 0.3;
		a.SimplificationQuadricEdgeCollapseDecimation(targetFaceNum, qualityThreshold, selectedOnly, autoClean);
	},py::arg("targetFaceNum"), py::arg("selectedOnly")=false, py::arg("autoClean")=true)
	.def("decimate_Clustering", [](Vcgmesh &a, float gridsize){
		a.SimplificationClusteringDecimation(gridsize);
	})
	.def("decimate_EdgeCollapseMarchingCube", [](Vcgmesh &a, float abs_error){
		return a.SimplificationEdgeCollapseMarchingCube(abs_error);
	})
  void Vcgmesh::SimplificationClusteringDecimation(float grid_size){
	float threshold = grid_size;
	vcg::tri::Clustering<CMeshO, vcg::tri::AverageColorCell<CMeshO> > ClusteringGrid;
	ClusteringGrid.Init(mesh.bbox,100000,threshold);
	if(mesh.FN() == 0)
		ClusteringGrid.AddPointSet(mesh);
	else
		ClusteringGrid.AddMesh(mesh);
	ClusteringGrid.ExtractMesh(mesh);
	//m.UpdateBoxAndNormals();
	vcg::tri::UpdateBounding<CMeshO>::Box(mesh);
	if(mesh.fn>0) {
		vcg::tri::UpdateNormal<CMeshO>::PerFaceNormalized(mesh);
		vcg::tri::UpdateNormal<CMeshO>::PerVertexAngleWeighted(mesh);
	}
	//m.clearDataMask(MeshModel::MM_FACEFACETOPO);
	mesh.face.DisableFFAdjacency();
}

void Vcgmesh::SimplificationQuadricEdgeCollapseDecimation(int targetFaceNum, double qualityThreshold, bool selectedOnly, bool autoClean){
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
	vcg::tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	mesh.vert.EnableMark();
	
	vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromVF(mesh);

	vcg::tri::TriEdgeCollapseQuadricParameter pp;
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
		int nullFaces=vcg::tri::Clean<CMeshO>::RemoveFaceOutOfRangeArea(mesh,0);
		int deldupvert=vcg::tri::Clean<CMeshO>::RemoveDuplicateVertex(mesh);
		int delvert=vcg::tri::Clean<CMeshO>::RemoveUnreferencedVertex(mesh);
		//mesh.clearDataMask(MeshModel::MM_FACEFACETOPO );
		mesh.face.DisableFFAdjacency();
		vcg::tri::Allocator<CMeshO>::CompactVertexVector(mesh);
		vcg::tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	}

	//mesh.UpdateBoxAndNormals();
	vcg::tri::UpdateBounding<CMeshO>::Box(mesh);
	if(mesh.fn>0) {
		vcg::tri::UpdateNormal<CMeshO>::PerFaceNormalized(mesh);
		vcg::tri::UpdateNormal<CMeshO>::PerVertexAngleWeighted(mesh);
	}
	
	vcg::tri::UpdateNormal<CMeshO>::NormalizePerFace(mesh);
	vcg::tri::UpdateNormal<CMeshO>::PerVertexFromCurrentFaceNormal(mesh);
	vcg::tri::UpdateNormal<CMeshO>::NormalizePerVertex(mesh);
}
bool Vcgmesh::SimplificationEdgeCollapseMarchingCube(float absoluteError){
	if (mesh.fn == 0)
	{
		return false;
	}
	//mm.updateDataMask(MeshModel::MM_VERTFACETOPO+MeshModel::MM_FACEFACETOPO+MeshModel::MM_VERTMARK);
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableVFAdjacency();
	vcg::tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	
	mesh.face.EnableFFAdjacency();
	vcg::tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	
	mesh.vert.EnableMark();
	
	int res = vcg::tri::MCSimplify<CMeshO>(mesh, absoluteError, false);
	if (res !=1)
	{
		//mm.clearDataMask(MeshModel::MM_VERTFACETOPO);
		//mm.clearDataMask(MeshModel::MM_FACEFACETOPO);
//		mesh.face.DisableVFAdjacency();
//		mesh.vert.DisableVFAdjacency();
//		mesh.face.DisableFFAdjacency();
		return false;
	}
	
	vcg::tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	vcg::tri::Clean<CMeshO>::RemoveTVertexByFlip(mesh,20,true);
	vcg::tri::Clean<CMeshO>::RemoveFaceFoldByFlip(mesh);
	//mm.clearDataMask(MeshModel::MM_VERTFACETOPO);
	//mm.clearDataMask(MeshModel::MM_FACEFACETOPO);
//	mesh.face.DisableVFAdjacency();
//	mesh.vert.DisableVFAdjacency();
//	mesh.face.DisableFFAdjacency();
	vcg::tri::Allocator<CMeshO>::CompactVertexVector(mesh);
	vcg::tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	vcg::tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	vcg::tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	return true;
}

```

## curvature

```cpp
void Vcgmesh::calcCurvature(){
	compactVert();
	
	mesh.vert.EnableVFAdjacency();
	mesh.vert.EnableCurvature();
	mesh.vert.EnableCurvatureDir();
	
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();
	
	// calc normal
	vcg::tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	vcg::tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	// calc curvature
	vcg::tri::UpdateCurvature<CMeshO>::MeanAndGaussian(mesh); // これも最大最小値は自前計算。これが一番見た目いい感じに計算されてる
	CMeshO::VertexIterator vi;
	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		CMeshO::VertexType *pv = &*vi;
		float H = pv->Kh();
		float G = pv->Kg();
		float a = std::max(0.f, H*H-G); // h^2-gは手法によっては負になる場合があるので0でmaxとる
		MESHLAB_SCALAR sq = sqrt(a);
		pv->Q() = H-sq;
		vertex_curvature_maxmin[vi] = std::pair<MESHLAB_SCALAR, MESHLAB_SCALAR>(H+sq, H-sq);
	}
	// colorize by vertex quality
	std::pair<float,float> curv_minmax = vcg::tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(mesh);
	curv_min = curv_minmax.first;
	curv_max = curv_minmax.second;
	qDebug("min:max = %f, %f", curv_min, curv_max);
	vcg::tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh, curv_min, curv_max);
	
	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		vi->C() = col_def;
	}
	// 色の反映
	vcg::tri::UpdateFlags<CMeshO>::VertexClearS(mesh);
	setVertexQualityThreshold(50);	// 初期値設定
//	colorizeMarked();
}
```



## ply with value

```cpp
	auto flags = vcg::tri::io::Mask::IOM_VERTCOLOR|
//				 vcg::tri::io::Mask::IOM_VERTNORMAL|
//				 vcg::tri::io::Mask::IOM_FACECOLOR|
				 vcg::tri::io::Mask::IOM_VERTQUALITY;
	int result = vcg::tri::io::ExporterPLY<CMeshO>::Save(mesh,qPrintable(fname), flags, true);
	if(result == vcg::ply::PlyError::E_NOERROR){
		saveFileName = qPrintable(fname);
		return 1; //success
	}
void Vcgmesh::setVertexQualityThresholdAbs(float val){
	threshold = val;
	qDebug("th: %f", threshold);
	int cnt =0;
	int total=0;
	
//	clearAllFlag(MARKED); // 毎回やるのは無駄なので省略
	CMeshO::VertexIterator vi;
	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		if(vi->Q()<=threshold){
//			setFlag(vi, MARKED); // 属性データに保存
			vi->SetS();
			cnt++;total++;
		}else{
//			clearFlag(vi, MARKED);
			vi->ClearS();
			total++;
		}
	}
	qDebug("cnt/total: %d/ %d", cnt, total);
//	colorizeMarked();
	loadMeshColor();
}

```



### end



