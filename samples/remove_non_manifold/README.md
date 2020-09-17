# Curvature

## Default curvature function

曲率計算はvcglibに実装されているものは

```cpp
#include <vcg/complex/algorithms/update/curvature.h>
vcg::tri::UpdateCurvature<CMeshO>::MeanAndGaussian(mesh);
```

計算結果は各頂点に平均曲率とガウス曲率が入る。最大・最小曲率を計算したい場合は自分で計算する。

以下は最小曲率を各頂点のQ要素に入れている(Q要素はデフォルトで用意されている浮動小数点を保持するメンバ)

```cpp
for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
    CMeshO::VertexType *pv = &*vi;
    float H = pv->Kh();
    float G = pv->Kg();
    float a = std::max(0.f, H*H-G); // h^2-gは手法によっては負になる場合があるので0でmaxとる
    MESHLAB_SCALAR sq = sqrt(a);
    pv->Q() = H-sq; // minimum curvature
    //pv->Q() = H+sq; // maximum curvature
}
```

曲率に応じて色分けをしたい場合はQ値によって色分けする関数を使用する

```cpp
std::pair<float,float> curv_minmax = vcg::tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(mesh);
float curv_min = curv_minmax.first;
float curv_max = curv_minmax.second;
vcg::tri::UpdateColor<CMeshO>::PerVertexQualityRamp(mesh, curv_min, curv_max);
```

## APSS function

MeshLabには曲率計算の拡張機能が実装されている



## Preparation for calculating curvature

メッシュの構造が不正なもの(Non manifold)の状態で曲率を計算するとエラーが起きる。

この場合は、あらかじめ不正な状態を解消する。

不正な状態は以下の関数で検出できる。

```cpp
// Non manifoldの検出
#include <vcg/complex/algorithms/clean.h>

bool isNonManifold(CMeshO &mesh){
	int cnt = 0;
	bool select_flag = false;
    
    // Count the number of non manifold edges in a mesh, e.g. the edges where there are more than 2 incident faces.
    // Note that this test is not enough to say that a mesh is two manifold, you have to count also the non manifold vertexes.
	cnt += tri::Clean<CMeshO>::CountNonManifoldEdgeFF(mesh, select_flag);
    
    // Count (and eventually select) non 2-Manifold vertexes of a mesh
    // typical situation two cones connected by one vertex.
	cnt += tri::Clean<CMeshO>::CountNonManifoldVertexFF(mesh, select_flag);
	return cnt>0;
}

mesh.vert.EnableVFAdjacency();
mesh.face.EnableFFAdjacency();
bool b = isNonManifold(mesh);
```



不正な箇所は手動で修正することもできるが、一番簡単な方法は該当箇所を丸ごと削除してしまうことである。

```cpp
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

deleteNonManifold(mesh);
```

この方法だとメッシュに穴が開くこともあることに注意。

