## main.cpp

```cpp
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Tetrahedron(mesh); // create preset mesh data
	cout << "default" << endl;
	for(auto &&v:mesh.vert)
		printf("(%f,%f,%f)\n",v.P().X(), v.P().Y(), v.P().Z());

	// meshlab\src\meshlabplugins\filter_meshing\meshfilter.cpp
	Matrix44m trRot, trTran, trTranInv, transfM;
	
	Point3m rotAxis = Point3m(1,0,0);
	Point3m rotCenter = Point3m(0,0,0);
	float angleDeg= 45.0;
	
	trRot.SetRotateDeg(angleDeg,rotAxis);
	trTran.SetTranslate(rotCenter);
	trTranInv.SetTranslate(-rotCenter);
	transfM = trTran*trRot*trTranInv;
	
	tri::UpdatePosition<CMeshO>::Matrix(mesh, transfM);
	tri::UpdateBounding<CMeshO>::Box(mesh);

	cout << "translated" << endl;
	for(auto &&v:mesh.vert)
		printf("(%f,%f,%f)\n",v.P().X(), v.P().Y(), v.P().Z());
	return 1;
}
```

