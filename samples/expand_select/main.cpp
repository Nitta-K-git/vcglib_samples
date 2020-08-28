#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	mesh.vert.EnableVFAdjacency();
	mesh.face.EnableFFAdjacency();
	mesh.face.EnableVFAdjacency();
	
	tri::Tetrahedron(mesh); // create preset mesh data
	
	// you have to update adjacent relation after loading data
	tri::UpdateTopology<CMeshO>::FaceFace(mesh);
	tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	
	// Recursively select faces that are connected to the selected faces
	mesh.face[0].SetS();
	tri::UpdateSelection<CMeshO>::FaceConnectedFF(mesh);
	for(auto &&f:mesh.face)
		if(f.IsS())
			cout << f.Index() << endl;
	
	int cnt;
	// Select all the vertices that are touched by at least a single selected faces
	tri::UpdateFlags<CMeshO>::Clear(mesh);
	mesh.face[0].SetS();
	cnt = tri::UpdateSelection<CMeshO>::VertexFromFaceLoose(mesh);
	cout << "VertexFromFaceLoose: " << cnt << endl;
	
	// Select ONLY the vertices that are touched ONLY by selected faces
	tri::UpdateFlags<CMeshO>::Clear(mesh);
	mesh.face[0].SetS();
	mesh.face[1].SetS();
	mesh.face[2].SetS();
	cnt = tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(mesh);
	cout << "VertexFromFaceStrict: " << cnt << endl;
	
	// Select all the faces with at least one selected vertex
	tri::UpdateFlags<CMeshO>::Clear(mesh);
	mesh.vert[0].SetS();
	cnt = tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(mesh);
	cout << "FaceFromVertexLoose: " << cnt << endl;
	
	// Select ONLY the faces with ALL the vertices selected
	tri::UpdateFlags<CMeshO>::Clear(mesh);
	mesh.vert[0].SetS();
	mesh.vert[1].SetS();
	mesh.vert[2].SetS();
	cnt = tri::UpdateSelection<CMeshO>::FaceFromVertexStrict(mesh);
	cout << "FaceFromVertexStrict: " << cnt << endl;

	return 1;
}

