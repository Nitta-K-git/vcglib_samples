#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/clean.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

using namespace vcg;
using namespace std;

int main()
{
	CMeshO mesh;
	tri::Tetrahedron(mesh); // create preset mesh data
	
	// if you use face color, you have to enable color because meshlab has Color4bOcf
	mesh.face.EnableColor();
	
	mesh.vert[0].C() = Color4b(0xff,0,0,0xff);
	mesh.vert[1].C() = Color4b::Cyan;
	mesh.face[0].C() = Color4b::Yellow;
	
	auto flags = tri::io::Mask::IOM_VERTCOLOR|
				 tri::io::Mask::IOM_FACECOLOR;
	bool binary = false;
	int result = tri::io::ExporterPLY<CMeshO>::Save(mesh, "out.ply", flags, binary);
	if(result != ply::PlyError::E_NOERROR){
		cout << "error" << endl;
	}
	
	return 0;
}

