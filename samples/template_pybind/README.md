You can use data structure used by [MeshLab](https://github.com/cnr-isti-vclab/meshlab).

What you have to do is only to import [`meshlab/src/common/ml_mesh_type.h`](https://github.com/cnr-isti-vclab/meshlab/blob/master/src/common/ml_mesh_type.h)

Then you can declare mesh data as `CMeshO`.



CMakeLists.txt

```cmake
# define path of meshlab source
set(MESHLAB_DIR C:/Users/Public/Documents/GitHub/meshlab_latest/src)
add_definitions(-DMESHLAB_SCALAR=float) # you should add this definition if you use meshlab source
```



main.cpp

```cpp
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Tetrahedron(mesh); // create preset mesh data

	for(CMeshO::VertexType &vt : mesh.vert){
		cout << vt.Index() << endl;
	}
	
	return 1;
}
```

