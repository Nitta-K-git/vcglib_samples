#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <wrap/io_trimesh/export_stl.h>

using namespace vcg;
using namespace std;
namespace py = pybind11;

class Vcgmesh{
public:
	void create_Tetrahedron();
	bool load(string s);
	bool save_ply(string s, bool binary, bool withVertColor, bool withFaceColor);
	
	CMeshO mesh;
};

void Vcgmesh::create_Tetrahedron(){
	tri::Tetrahedron(mesh);
}

bool Vcgmesh::load(string s){
	int err = tri::io::Importer<CMeshO>::Open(mesh, s.c_str()); // 多分ライブラリにあるファイルなら何でも開ける
	if (err) { // all the importers return 0 in case of success
		printf("Error in reading %s: '%s'\n", s.c_str(), tri::io::Importer<CMeshO>::ErrorMsg(err));
		return false;
	}
	return true;
}

bool Vcgmesh::save_ply(string s, bool binary, bool withVertColor, bool withFaceColor){
	int mask = tri::io::Mask::IOM_NONE;
	if(withVertColor) mask |= tri::io::Mask::IOM_VERTCOLOR;
	if(withFaceColor) mask |= tri::io::Mask::IOM_FACECOLOR;
	int result = tri::io::ExporterPLY<CMeshO>::Save(mesh, s.c_str(), mask, binary);
	if(ply::PlyError::E_NOERROR == result){
		return true;
	}else{
		printf("ply save error : error_code is %d\n", result);
		return false;
	}
}

void hello(){
	cout << "Hello Vcgmesh" << endl;
}

PYBIND11_MODULE(MY_MODULE_NAME, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
	m.def("hello", &hello, "Hello Vcgmesh");

	py::class_<Vcgmesh>(m, "Vcgmesh")
	.def(py::init<>())
	.def("load", [](Vcgmesh &a, string fname){
		bool ret = a.load(fname);
		return ret;
	},"input file path(e.g. PLY, OFF, STL...)")
	.def("save_ply", [](Vcgmesh &a, string fname, bool binary, bool withVertColor, bool withFaceColor){
		// TODO : check extension is ply
		a.save_ply(fname, binary, withVertColor, withFaceColor);
	}, py::arg("fname")="out.ply", py::arg("binary")=true, py::arg("withVertColor")=false, py::arg("withFaceColor")=false, "save as ply format.")
	.def("__repr__", [](const Vcgmesh &a){
		return "vert: " + to_string(a.mesh.VN()) + ", face: " + std::to_string(a.mesh.FN());
	},"print mesh infomation. number of vertices and faces")
	.def("create_Tetrahedron", &Vcgmesh::create_Tetrahedron)
	;
}
