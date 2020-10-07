# Subdivision Surfaces

メッシュの分割

[source code of meshlab](https://github.com/cnr-isti-vclab/meshlab/blob/master/src/meshlabplugins/filter_meshing/meshfilter.cpp)

## Midpoint

中点で分割していくだけのシンプルな方法。指定した回数だけ分割を行う(辺の長さが指定値未満になったら分割されない)

```cpp
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Tetrahedron(mesh); // create preset mesh data
	
	mesh.face.EnableFFAdjacency();
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	
	tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	tri::Allocator<CMeshO>::CompactVertexVector(mesh);
	tri::UpdateFlags<CMeshO>::FaceBorderFromFF(mesh);

	if (  tri::Clean<CMeshO>::CountNonManifoldEdgeFF(mesh) > 0)
	{
		printf("Mesh has some not 2 manifoldfaces, subdivision surfaces require manifoldness\n");
		return 0; // can't continue, mesh can't be processed
	}
	
	MESHLAB_SCALAR threshold = mesh.bbox.Diag()*0.01;
	int cnt = 3;
	bool selected = false;
	for(int i=0; i<cnt; ++i){
		Refine<CMeshO,MidPoint<CMeshO> > (mesh, MidPoint<CMeshO>(&mesh), threshold, selected);
	}
	
	tri::io::ExporterPLY<CMeshO>::Save(mesh,"out.ply",tri::io::Mask::IOM_NONE);
	
	return 1;
}
```



## Butterfly Subdivision

エッジが少し丸くなる

```cpp
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Tetrahedron(mesh); // create preset mesh data
	
	mesh.face.EnableFFAdjacency();
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	
	tri::Allocator<CMeshO>::CompactFaceVector(mesh);
	tri::Allocator<CMeshO>::CompactVertexVector(mesh);
	//m.updateDataMask( MeshModel::MM_FACEFACETOPO);
	tri::UpdateFlags<CMeshO>::FaceBorderFromFF(mesh);

	if (  tri::Clean<CMeshO>::CountNonManifoldEdgeFF(mesh) > 0)
	{
		printf("Mesh has some not 2 manifoldfaces, subdivision surfaces require manifoldness\n");
		return 0; // can't continue, mesh can't be processed
	}
	
	MESHLAB_SCALAR threshold = mesh.bbox.Diag()*0.01;
	int cnt = 3;
	bool selected = false;
	for(int i=0; i<cnt; ++i){
		Refine<CMeshO,MidPointButterfly<CMeshO> > (mesh, MidPointButterfly<CMeshO>(mesh), threshold, selected);
	}
	
	tri::io::ExporterPLY<CMeshO>::Save(mesh,"out.ply",tri::io::Mask::IOM_NONE);
	
	return 1;
}
```



## Loop

形状がかなり縮小される

```cpp

```



## Catmull-Clark

```cpp

```



