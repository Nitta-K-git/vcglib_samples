#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>

#include <iostream>

#include <common/ml_mesh_type.h>

/*
 * 1ring頂点の巡回をPosクラスを用いて実装する 時計回り
 * 面巡回で実装してから指定頂点の反対側の頂点を取得してまた戻る
*/

using namespace vcg;

int main(int argc,char ** argv){
	if(argc < 2 ){
		printf("Usage: trimesh_harmonic mesh.ply\nuse octahedron.ply\n");
	}
	
	CMeshO m;
	m.vert.EnableVFAdjacency();
	m.face.EnableFFAdjacency();
	m.face.EnableVFAdjacency();
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,(argc<2)?"octahedron.ply":argv[1]);
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
	
	tri::UpdateTopology<CMeshO>::FaceFace(m);
	tri::UpdateTopology<CMeshO>::VertexFace(m); // 初期化必要
	
	CMeshO::VertexType *v = &(m.vert[0]);
	CMeshO::FaceType *pf = v->VFp();
	face::Pos<CMeshO::FaceType> p;
	
	pf = v->VFp();
	p.Set(pf, v);
	std::cout << "face::" << face_indices[pf] << ",";
	std::cout << "center::" << vert_indices[v] << std::endl;
	while(true){
		p.FlipV();
		std::cout << vert_indices[p.V()] << ",";
		p.FlipV(); // 元に戻る
		p.NextE();
		if(pf == p.F()){break;}
	}
	std::cout << std::endl;
	
	return 0;
}
