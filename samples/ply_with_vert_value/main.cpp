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

