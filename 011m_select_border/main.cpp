#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>

#include <common/ml_mesh_type.h>

/*
 * 境界線を検出・選択する
*/

using namespace vcg;

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\nuse octahedron.ply\n");
	}
	const char *fname = "oct_del.ply";
	
	CMeshO m;
//	m.vert.EnableVFAdjacency();
//	m.face.EnableFFAdjacency();
//	m.face.EnableVFAdjacency();
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,(argc<2)?fname:argv[1]);
	if(ret!=0){
		printf("Unable to open %s for '%s'\n",argv[1],tri::io::ImporterPLY<CMeshO>::ErrorMsg(ret));
		return -1;
	}
	
	// ポインタからインデックス返す配列作成
	vcg::SimpleTempData<typename CMeshO::VertContainer, unsigned int> vert_indices(m.vert);
	vcg::SimpleTempData<typename CMeshO::FaceContainer, unsigned int> face_indices(m.face);
	unsigned int i=0;
	CMeshO::VertexIterator vi;
	for(i=0,vi=m.vert.begin(); vi!=m.vert.end(); ++vi,++i){
		vert_indices[vi] = i; // vertex point to vertex index
	}
	CMeshO::FaceIterator fi;
	for(i=0,fi=m.face.begin(); fi!=m.face.end(); ++fi,++i){
		face_indices[fi] = i; // face point to face index
	}
	
//	tri::UpdateTopology<CMeshO>::FaceFace(m);
//	tri::UpdateTopology<CMeshO>::VertexFace(m);
	
	// 境界線要素選択
	tri::UpdateFlags<CMeshO>::FaceBorderFromNone(m);
	tri::UpdateFlags<CMeshO>::VertexBorderFromFaceBorder(m);
	tri::UpdateSelection<CMeshO>::FaceFromBorderFlag(m);
	tri::UpdateSelection<CMeshO>::VertexFromBorderFlag(m);
	
	// 選択要素数をカウント(meshlabのml_selection_buffers.cppで実装されているが、これimportするとmeshlabのデータ丸ごと入れないといけないので、最低限の部分を自前で実装する)
	m.sfn = 0;
	m.svn = 0;
	for(CFaceO &ff:m.face){
		if(!ff.IsD() && ff.IsS()){
			m.sfn++;
		}
	}
	for(CVertexO &vv:m.vert){
		if(!vv.IsD() && vv.IsS()){
			m.svn++;
		}
	}
	std::cout << "selected face:vertex = " << m.sfn << ":" << m.svn << std::endl;
	return 0;
}
