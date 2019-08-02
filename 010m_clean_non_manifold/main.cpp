#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>

#include <common/ml_mesh_type.h>

/*
 * non manifoldを検出・選択する
*/

using namespace vcg;

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\nuse octahedron.ply\n");
	}
	
//	const char *fname = "non_manifold_same_vertex.ply"; // >>0,1
	const char *fname = "non_manifold_sharing_edge.ply"; // >>1,0
	
	CMeshO m;
//	m.vert.EnableVFAdjacency();
	m.face.EnableFFAdjacency();
//	m.face.EnableVFAdjacency();
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,(argc<2)?fname:argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<CMeshO>::ErrorMsg(ret));
		return -1;
	}
	
	CMeshO::VertexIterator vi;
	CMeshO::FaceIterator fi;
	
	tri::UpdateTopology<CMeshO>::FaceFace(m);
//	tri::UpdateTopology<CMeshO>::VertexFace(m);
	
	int cntEFF = tri::Clean<CMeshO>::CountNonManifoldEdgeFF(m,true); // 3面以上で共有されている辺の本数,選択する場合は面+共有辺の頂点
	std::cout << "CountNonManifoldEdgeFF:" << cntEFF << std::endl;
	int cntVFF = tri::Clean<CMeshO>::CountNonManifoldVertexFF(m,true); // 1ringが途切れている頂点の数,頂点のみ選択される
	std::cout << "CountNonManifoldVertexFF:" << cntVFF << std::endl;
	// 両方の選択状態は干渉する
	// 頂点選択は面の選択には干渉しないので、辺選択→頂点選択して面と頂点両方削除すればnon manifoldはすべて削除できる
	
	// 選択要素の削除
//	if (cntEFF+cntVFF > 0){ // 頂点の削除
//		tri::UpdateSelection<CMeshO>::FaceClear(m);
//		tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(m);
//		int vvn = m.vn;
//		int ffn = m.fn;
//		for (fi = m.face.begin(); fi != m.face.end(); ++fi)
//			if (!(*fi).IsD() && (*fi).IsS()){
//				tri::Allocator<CMeshO>::DeleteFace(m, *fi);
//			}
//		for (vi = m.vert.begin(); vi != m.vert.end(); ++vi)
//			if (!(*vi).IsD() && (*vi).IsS()){
//				tri::Allocator<CMeshO>::DeleteVertex(m, *vi);
//			}
//		printf("Deleted %i vertices, %i faces.", vvn - m.vn, ffn - m.fn);
//	}
//	if (cntEFF+cntVFF > 0){ // 面の削除
//		int ffn = m.fn;
//		for (fi = m.face.begin(); fi != m.face.end(); ++fi)
//			if (!(*fi).IsD() && (*fi).IsS()){
//				tri::Allocator<CMeshO>::DeleteFace(m, *fi);
//			}
//		printf("Deleted %i faces.", ffn - m.fn);
//	}
	if (cntEFF+cntVFF > 0){ // 頂点と面の両方を削除
		tri::UpdateSelection<CMeshO>::VertexClear(m);
		tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m);
		int vvn = m.VN();
		int ffn = m.FN();
		for (fi = m.face.begin(); fi != m.face.end(); ++fi)
			if (!(*fi).IsD() && (*fi).IsS()){
				tri::Allocator<CMeshO>::DeleteFace(m, *fi);
			}
		for (vi = m.vert.begin(); vi != m.vert.end(); ++vi)
			if (!(*vi).IsD() && (*vi).IsS()){
				tri::Allocator<CMeshO>::DeleteVertex(m, *vi);
			}
		std::cout << "Deleted " << ffn-m.FN() << " faces, " << vvn-m.VN() << " vertices." << std::endl;
	}
	return 0;
}
