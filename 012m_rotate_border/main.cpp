#include<vcg/complex/complex.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>
#include <vcg/simplex/face/pos.h>
#include <vcg/complex/algorithms/hole.h> // 境界線特定用の関数がある

#include <iostream>

#include <common/ml_mesh_type.h>

/*
 * 境界線を検出・選択する
*/

using namespace vcg;

int main(int argc,char ** argv){
	const char *fname = "2hole.ply";
	
	CMeshO m;
	m.vert.EnableVFAdjacency();
	m.face.EnableFFAdjacency();
	m.face.EnableVFAdjacency();
	
	//open ply
	int ret= tri::io::ImporterPLY<CMeshO>::Open(m,fname);
	if(ret!=0){return -1;}
	
	tri::UpdateTopology<CMeshO>::FaceFace(m);
	
	std::vector<tri::Hole<CMeshO>::Info> vinfo;
	tri::Hole<CMeshO>::GetInfo(m, false, vinfo);
	std::vector<tri::Hole<CMeshO>::Info>::iterator ith;
	
	for(ith = vinfo.begin(); ith!= vinfo.end(); ++ith){
		std::cout << "boundary size: " << (*ith).size << std::endl;
		std::vector<CMeshO::FacePointer *> facePtrToBeUpdated;
		vcg::face::Pos<CFaceO> ip = (*ith).p;
		do{
			std::cout << tri::Index(m, ip.V()) << ", ";
			ip.NextB();
		}while(ip != (*ith).p);
		std::cout << std::endl;
	}
	return 0;
}
