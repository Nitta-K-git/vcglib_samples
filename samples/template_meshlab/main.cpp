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

