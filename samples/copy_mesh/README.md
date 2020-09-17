# copy and append

## main.cpp

```cpp
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh1, mesh2;
	tri::Tetrahedron(mesh1);
	tri::Hexahedron(mesh2);
	
	cout << "mesh1 : " << mesh1.VN() << "," << mesh1.FN() << endl;
	cout << "mesh2 : " << mesh2.VN() << "," << mesh2.FN() << endl;
	
	bool selected = false;
	bool adjFlag = true;

	tri::Append<CMeshO, CMeshO>::Mesh(mesh1, mesh2, selected, adjFlag); // mesh1 += mesh2
	cout << "mesh1 += mesh2 : " << mesh1.VN() << "," << mesh1.FN() << endl;
	
	CMeshO mesh;
	tri::Append<CMeshO, CMeshO>::MeshCopy(mesh, mesh2, selected, adjFlag); // mesh = mesh2
	cout << "mesh = mesh2 : " << mesh.VN() << "," << mesh.FN() << endl;
	
	return 1;
}
```

## output

```shell
mesh1 : 4,4
mesh2 : 8,12
mesh1 += mesh2 : 12,16
mesh = mesh2 : 8,12
```

