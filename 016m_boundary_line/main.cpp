#include<vcg/complex/complex.h>

#include<vcg/complex/algorithms/clean.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include <QVector>
#include <QVector3D>

using namespace vcg;
using namespace std;

class MyFace;
class MyVertex;
class MyEdge;

struct MyUsedTypes : public UsedTypes<Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType, Use<MyEdge>::AsEdgeType> {};
class MyVertex : public Vertex< MyUsedTypes, vertex::Coord3f, vertex::BitFlags, vertex::VFAdj> {};
class MyFace : public Face< MyUsedTypes, face::VertexRef, face::VFAdj, face::BitFlags> {};
class MyEdge : public Edge< MyUsedTypes, edge::VertexRef, edge::VertexRef, edge::EVAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> , vector<MyEdge>> {};

int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;
	MyMesh mesh;
	tri::Octahedron(mesh);
	
	MyMesh::EdgeIterator ei;
	for(ei=mesh.edge.begin(); ei!=mesh.edge.end(); ++ei){
		size_t eid = vcg::tri::Index(mesh, *ei);
		std::cout << eid << ", ";
	}
	std::cout << std::endl;
//	// インデックス出力
//	MyMesh::FaceIterator fi;
//	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
//		size_t fid = tri::Index(mesh, *fi);
//		std::cout << "fid: " << fid << ", vid: ";
//		for(int k=0; k<fi->VN(); ++k){
//			MyMesh::VertexType *pv = fi->cV(k);
//			size_t vid = tri::Index(mesh, pv);
//			std::cout << vid << ", ";
//		}
//		std::cout << std::endl;
//	}
//	// attrを作成して適当な値を入れる.
//	// PerMesh: meshデータ1つに1個
//	// PerFace, PerVertex: 面、頂点のイテレータなどでアクセス可能
//	MyMesh::PerMeshAttributeHandle<QVector3D> attr_mesh;
//	attr_mesh = tri::Allocator<MyMesh>::GetPerMeshAttribute<QVector3D>(mesh,"mesh_pos");
	
//	// イテレータとかを使って入出力ができる http://www.vcglib.net/trimesh__attribute_8cpp_source.html
//	MyMesh::PerVertexAttributeHandle<int> attr_vert = tri::Allocator<MyMesh>::GetPerVertexAttribute<int>(mesh, "vertex");
//	MyMesh::VertexIterator vi;
//	int i;
//	for(i=0,vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi,++i){
//		attr_vert[vi] = i;		// iterator
//		attr_vert[*vi] = i;		// vertex type
//		attr_vert[&*vi] = i;	// vertex pointer
//		attr_vert[i] = i;		// index
//	}
//	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
//		std::cout << attr_vert[vi] << ", ";
//	}
//	std::cout << std::endl;
	
//	// 頂点削除してみる(フラグ操作をするのでBitFlagsの要素が必要)(無いとassert(0)のエラー出る)
//	// 頂点選択→関連する面を選択→面・頂点の選択要素を削除の順番で処理する(頂点だけ削除しても面が不正な形で残ってしまう)
//	mesh.vert[3].SetS(); // 選択状態にする
//	tri::UpdateSelection<MyMesh>::FaceClear(mesh);
//	tri::UpdateSelection<MyMesh>::FaceFromVertexLoose(mesh); // 関連する面も選択
//	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // auto使うと違うところ指すので不可
//		if(fi->IsS()){
//			vcg::tri::Allocator<MyMesh>::DeleteFace(mesh, *fi);
//		}
//	}
//	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
//		if(vi->IsS()){
//			vcg::tri::Allocator<MyMesh>::DeleteVertex(mesh, *vi);
//		}
//	}
//	vcg::tri::Allocator<MyMesh>::CompactFaceVector(mesh); // メモリから削除
//	vcg::tri::Allocator<MyMesh>::CompactVertexVector(mesh); // メモリから削除
	
//	// 再度表示
//	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
//		size_t fid = tri::Index(mesh, *fi);
//		std::cout << "fid: " << fid << ", vid: ";
//		for(int k=0; k<fi->VN(); ++k){
//			MyMesh::VertexType *pv = fi->cV(k);
//			size_t vid = tri::Index(mesh, pv);
//			std::cout << vid << ", ";
//		}
//		std::cout << std::endl;
//	}
//	// attrも表示
//	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
//		std::cout << attr_vert[vi] << ", ";
//	}
//	std::cout << std::endl;
	return 0;
}

