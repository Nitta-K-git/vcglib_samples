#include<vcg/complex/complex.h>

#include<vcg/complex/algorithms/clean.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

#include<iostream>
#include <vcg/complex/allocate.h>

#include <vcg/complex/append.h>

using namespace vcg;
using namespace std;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<	Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes, vertex::Coord3f, vertex::BitFlags, vertex::VFAdj> {}; // need BitFlags, VFAdj
class MyFace : public Face< MyUsedTypes, face::VertexRef, face::VFAdj> {}; // need VFAdj
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};


/*
 * http://vcg.isti.cnr.it/vcglib/allocation.html
 *		Allocとmeshのdeepcopy
 * 頂点や面を追加するとvectorが再確保される場合がある
 * 再確保の有無とその位置を把握するためにPointUpdaterクラスを使える
 * 
 * メッシュを丸ごとコピーする場合は
 * 1.頂点をalloc
 * 2.座標をコピー(必要なら他の要素も)
 * 3.面をalloc
 * 4.頂点ポインタを面の各要素に割当(頂点ポインタは同じメッシュデータでないといけないので、元データのインデックスだけ取得する)
 *		頂点確保時のイテレータがあれば、頂点インデックスはそのまま使える
 * 
 * 選択要素など特定の要素のみコピーしたい場合は、deepcopy→compactなどしてからメッシュ丸ごとコピーが一番楽
 *  MyMesh m2;
	vcg :: tri :: Append <MyMesh、MyMesh> :: MeshCopy（m2、m）;
*/

template<class MyMesh>
void appendMesh(MyMesh &m1, MyMesh &m2){ // dst, src
	auto vi = vcg::tri::Allocator<MyMesh>::AddVertices(m1, m2.VN());
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.VN(); ++i){
		vi[i].P() = m2.vert[i].P();
	}
	
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	auto fi = vcg::tri::Allocator<MyMesh>::AddFaces(m1, m2.FN());
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.FN(); ++i){
		int idx0 = vcg::tri::Index(m2, m2.face[i].V(0));
		int idx1 = vcg::tri::Index(m2, m2.face[i].V(1));
		int idx2 = vcg::tri::Index(m2, m2.face[i].V(2));
		std::cout << idx0 << ":" << idx1 << ":" << idx2 << std::endl;
		fi[i].V(0) = &vi[idx0];
		fi[i].V(1) = &vi[idx1];
		fi[i].V(2) = &vi[idx2];
	}
}

int main(int argc, char ** argv){
	MyMesh m1, m2;
	
	vcg::tri::Tetrahedron(m1);
	vcg::tri::Octahedron(m2);
	
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	
	// ひとつだけ追加するならAddVerticesで頂点座標や法線、色をセットで代入する
	// 巨大なデータを入れる場合は、最初にサイズまとめて確保してから順番に代入していく(fpはそのまま代入しても駄目→インデックス経由でコピーすればいける)
	appendMesh(m1, m2);
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	
	tri::io::ExporterPLY<MyMesh>::Save(m1,"outm1.ply",tri::io::Mask::IOM_NONE, false);
	
	
	MyMesh m3;
	vcg::tri::Append<MyMesh, MyMesh>::MeshCopy(m3, m1, true, false); // 上書きされる
	vcg::tri::Append<MyMesh, MyMesh>::Mesh(m3, m1, true, false); // 追加される
	
	return 0;
}


/*
int main(int argc, char ** argv)
{	MyMesh m1, m2;
	
	vcg::tri::Tetrahedron(m1);
	vcg::tri::Octahedron(m2);
	
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	
	// ひとつだけ追加するならAddVerticesで頂点座標や法線、色をセットで代入する
	// 巨大なデータを入れる場合は、最初にサイズまとめて確保してから順番に代入していく(fpはそのまま代入しても駄目→インデックス経由でコピーすればいける)
	auto vi = vcg::tri::Allocator<MyMesh>::AddVertices(m1, m2.VN());
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.VN(); ++i){
		vi[i].P() = m2.vert[i].P();
	}
	
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	auto fi = vcg::tri::Allocator<MyMesh>::AddFaces(m1, m2.FN());
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.FN(); ++i){
		int idx0 = vcg::tri::Index(m2, m2.face[i].V(0));
		int idx1 = vcg::tri::Index(m2, m2.face[i].V(1));
		int idx2 = vcg::tri::Index(m2, m2.face[i].V(2));
		std::cout << idx0 << ":" << idx1 << ":" << idx2 << std::endl;
		fi[i].V(0) = &vi[idx0];
		fi[i].V(1) = &vi[idx1];
		fi[i].V(2) = &vi[idx2];
	}
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	
	tri::io::ExporterPLY<MyMesh>::Save(m1,"outm1.ply",tri::io::Mask::IOM_NONE, false);
	
	return 0;
}
*/



