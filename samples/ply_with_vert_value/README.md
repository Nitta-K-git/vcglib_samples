## main.cpp

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

	mesh.vert[0].Q() = 0.1;
	mesh.vert[1].Q() = -2.0;
	mesh.vert[2].Q() = 100.0;
	
	for(auto &&vp:mesh.vert)
		cout << vp.Q() << endl;
	
	auto flags = tri::io::Mask::IOM_VERTCOLOR|
				 tri::io::Mask::IOM_VERTQUALITY;
	bool bBinary = false;
	int result = tri::io::ExporterPLY<CMeshO>::Save(mesh, "out.ply", flags, bBinary);
	if(result == ply::PlyError::E_NOERROR){
		return 1;
	}else{
		cout << "save error" << endl;
		return 0;
	}
}
```



## output

```shell
0.1
-2
100
0
```



## out.ply

```
ply
format ascii 1.0
comment VCGLIB generated
element vertex 4
property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
property uchar alpha
property float quality
element face 4
property list uchar int vertex_indices
end_header
1 1 1 255 255 255 255 0.1 
-1 1 -1 255 255 255 255 -2 
-1 -1 1 255 255 255 255 100 
1 -1 -1 255 255 255 255 0 
3 0 1 2 
3 0 2 3 
3 0 3 1 
3 3 2 1 
```

