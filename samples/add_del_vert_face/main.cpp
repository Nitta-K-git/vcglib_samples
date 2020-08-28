#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>
#include <vcg/complex/algorithms/create/platonic.h>

using namespace vcg;
using namespace std;

int main(){
	CMeshO mesh;
	tri::Tetrahedron(mesh);
	
	cout << "initial state" << endl;
	cout << "vp: ";
	for(auto &&vp:mesh.vert)
		cout << vp.Index() << " ";
	cout << endl;
	cout << "fp: ";
	for(auto &&fp:mesh.face)
		cout << fp.Index() << " ";
	cout << endl;
	
	// add vertices and faces
	size_t v_add_num = 3;
	CMeshO::VertexIterator vi = tri::Allocator<CMeshO>::AddVertices(mesh, v_add_num);
	size_t f_add_num = 1;
	CMeshO::FaceIterator fi = tri::Allocator<CMeshO>::AddFaces(mesh, f_add_num);
	
	vi[0].P() = Point3f(1,1,1);
	vi[1].P() = Point3f(1,0,0);
	vi[2].P() = Point3f(2,2,2);
	
	fi[0].V(0) = &(vi[0]);
	fi[0].V(1) = &(vi[1]);
	fi[0].V(2) = &(vi[2]);
	
	cout << "added state" << endl;
	cout << "vp: ";
	for(auto &&vp:mesh.vert)
		cout << vp.Index() << " ";
	cout << endl;
	cout << "fp: ";
	for(auto &&fp:mesh.face)
		cout << fp.Index() << " ";
	cout << endl;
	
	// delete vertices and faces
	{
		cout << "delete" << endl;
		
		mesh.vert.EnableVFAdjacency();
		mesh.face.EnableFFAdjacency();
		mesh.face.EnableVFAdjacency();
		
		CMeshO::VertexPointer vp = &(mesh.vert[0]);
		tri::Allocator<CMeshO>::DeleteVertex(mesh, *vp);
		
		// if you delete vertices, you need to consider whether the vertices belong to a face.
		for (face::VFIterator<CMeshO::FaceType> vfi(vp); !vfi.End(); ++vfi){
			CMeshO::FacePointer fp = vfi.F();
			tri::Allocator<CMeshO>::DeleteFace(mesh, *fp);
		}
		
		// you have to update some informations
		tri::Allocator<CMeshO>::CompactEveryVector(mesh);
		tri::UpdateTopology<CMeshO>::FaceFace(mesh);
		tri::UpdateTopology<CMeshO>::VertexFace(mesh);
	}
	
	// another sample : add mesh2 to mesh1
	{
		cout << "append mesh 1" << endl;
		CMeshO mesh1, mesh2;
		tri::Hexahedron(mesh1);
		tri::Tetrahedron(mesh2);
		
		// vi and fi start from adding position
		auto vi = tri::Allocator<CMeshO>::AddVertices(mesh1, mesh2.VN());
		auto fi = tri::Allocator<CMeshO>::AddFaces(mesh1, mesh2.FN());
		
		for(int i=0; i<mesh2.VN(); ++i)
			vi[i].P() = mesh2.vert[i].P();
		
		for(int i=0; i<mesh2.FN(); ++i){
			for(int j=0; j<3; ++j){
				int id = mesh2.face[i].V(j)->Index();
				fi[i].V(j) = &vi[id];
			}
		}
		cout << "append mesh: " << mesh1.VN() << ":" << mesh1.FN() << endl;
	}
	// and above code is same below one
	{
		cout << "append mesh 2" << endl;
		CMeshO mesh1, mesh2;
		tri::Hexahedron(mesh1);
		tri::Tetrahedron(mesh2);
		
		bool selected = false;
		bool adjFlag = true;
		tri::Append<CMeshO,CMeshO>::Mesh(mesh1, mesh2, selected, adjFlag); // copy with adj information
		
		cout << "append mesh: " << mesh1.VN() << ":" << mesh1.FN() << endl;
	}
	
	return 1;
}
