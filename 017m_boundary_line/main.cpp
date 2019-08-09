#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>

#include <common/ml_mesh_type.h>

using namespace vcg;

int main(int argc,char ** argv){
	CMeshO mesh;
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();

	int ret= tri::io::ImporterPLY<CMeshO>::Open(mesh,"octahedron_ascii.ply");
//	int ret= tri::io::ImporterPLY<CMeshO>::Open(mesh,"sample_face.ply");
	
	//open ply
	if(ret!=0){
		return -1;
	}
	
	CMeshO::EdgeIterator ei;
	for(ei=mesh.edge.begin(); ei!=mesh.edge.end(); ++ei){
		size_t eid = vcg::tri::Index(mesh, *ei);
		std::cout << eid << ", ";
	}
	std::cout << std::endl;
	
	return 0;
}
