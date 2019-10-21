#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>
#include <vcg/complex/algorithms/create/platonic.h>

#include <common/ml_mesh_type.h>

using namespace vcg;

int main(int argc,char ** argv){
	CMeshO mesh;
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();
	
	

	vcg::tri::Octahedron(mesh);
	
	CMeshO::EdgeIterator ei;
	std::cout << mesh.EN() << std::endl;
	
	for(ei=mesh.edge.begin(); ei!=mesh.edge.end(); ++ei){
		size_t eid = vcg::tri::Index(mesh, *ei);
		std::cout << eid << ", ";
	}
	std::cout << std::endl;
	
	return 0;
}
