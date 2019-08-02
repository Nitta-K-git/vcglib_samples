#include<vcg/complex/complex.h>

#include<vcg/complex/algorithms/clean.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

using namespace vcg;
using namespace std;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes<	Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes, vertex::Coord3f, vertex::BitFlags, vertex::VFAdj> {}; // need BitFlags, VFAdj
class MyFace : public Face< MyUsedTypes, face::VertexRef, face::VFAdj> {}; // need VFAdj
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(int argc, char ** argv)
{
	if (argc < 2){
		printf("Usage: <filename> \n");
		return 0;
	}
	MyMesh m;

	//open a mesh
	int err = tri::io::Importer<MyMesh>::Open(m, argv[1]); // 多分ライブラリにあるファイルなら何でも開ける
	if (err) { // all the importers return 0 in case of success
		printf("Error in reading %s: '%s'\n", argv[1], tri::io::Importer<MyMesh>::ErrorMsg(err));
		exit(-1);
	}

	// some cleaning to get rid of bad file formats like stl that duplicate vertexes..
	int dup = tri::Clean<MyMesh>::RemoveDuplicateVertex(m);			// STLとかの頂点が重複定義されるものは統合する
	int unref = tri::Clean<MyMesh>::RemoveUnreferencedVertex(m);	// 面から参照されていない頂点は削除する
	printf("Removed %i duplicate and %i unreferenced vertices from mesh %s\n", dup, unref, argv[1]);

	tri::UpdateTopology<MyMesh>::VertexFace(m);
	return 0;
}

