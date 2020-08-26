#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Octahedron(mesh); // create preset mesh data
	
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();

	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	
	CMeshO::VertexPointer vp;
	vp = &(mesh.vert[0]);
	if(vp->HasVFAdjacency()==false){
		cout << "Adjacency is not valid" << endl;
		return 0;
	}
	
	// Around the adjacent faces centering on the vertex vp (The order of rotation is undefined)
	cout << "VFIterator" << endl;
	for (face::VFIterator<CMeshO::FaceType> vfi(vp); !vfi.End(); ++vfi){
		cout << vfi.F()->Index() << endl;
	}
	
	// Around the adjacent vertices centering on the vertex vp (The order of rotation is counter clock wise in default option)
	cout << "VVOrderedStarFF" << endl;
	vector<CMeshO::VertexPointer> vecvt;
	face::VFIterator<CMeshO::FaceType> vfi(vp); // set VFIterator from vertex
	face::Pos<CMeshO::FaceType> fpos;
	fpos.Set(vfi.f, vp); // set face pos from VFIterator
	face::VVOrderedStarFF<CMeshO::FaceType>(fpos, vecvt); // get adjacent vertices of vp (counter clock wise)
	for(auto &&vp : vecvt){
		cout << vp->Index() << endl;
	}
	
	// Around the adjacent faces centering on the vertex vp (The order of rotation is counter clock wise in default option)
	cout << "VFOrderedStarFF" << endl;
	vector<face::Pos<CMeshO::FaceType>> vecfpos;
	face::VFOrderedStarFF<CMeshO::FaceType>(fpos, vecfpos);
	cout << "  vert" << endl;
	for(auto &&fps : vecfpos){
		cout << fps.V()->Index() << endl; // vert is always same because it's center of ratation
	}
	cout << "  face" << endl;
	for(auto &&fps : vecfpos){
		cout << fps.F()->Index() << endl;
	}
	return 1;
}

