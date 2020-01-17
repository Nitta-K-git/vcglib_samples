#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>

class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(){
	MyMesh mesh;
	vcg::tri::Octahedron(mesh); // 正八面体のデータを作成
	
	MyMesh::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // メッシュの各面を巡回する
		std::cout << vcg::tri::Index(mesh, *fi) << ": "; // 面のインデックスを取得
		for(int i=0; i<fi->VN(); ++i){ // 面の各頂点を巡回する
			std::cout << vcg::tri::Index(mesh, fi->V(i)) << ","; // 頂点のインデックスを取得
		}
		std::cout << std::endl;
	}
	return 1;
}

