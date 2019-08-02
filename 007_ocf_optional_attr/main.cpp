#include<vcg/complex/complex.h>

#include<vcg/complex/algorithms/create/platonic.h>

using namespace vcg;

class MyVertexOcf;
class MyFaceOcf;

struct MyUsedTypesOcf: public UsedTypes<Use<MyVertexOcf>::AsVertexType,Use<MyFaceOcf>::AsFaceType>{};
// vertex::InfoOcfが必要 + ~Ocfの要素
class MyVertexOcf:public Vertex< MyUsedTypesOcf,vertex::InfoOcf,vertex::Coord3f,vertex::Normal3fOcf>{};
class MyFaceOcf:public Face< MyUsedTypesOcf,face::InfoOcf,face::VertexRef>{};

// the mesh class must make use of the 'vector_ocf' containers instead of the classical std::vector
class MyMeshOcf : public vcg::tri::TriMesh< vcg::vertex::vector_ocf<MyVertexOcf>, vcg::face::vector_ocf<MyFaceOcf>>{};

int main()
{
	MyMeshOcf m;
	vcg::tri::Tetrahedron(m);
	MyMeshOcf::VertexIterator vi = m.vert.begin();
	
	printf("(*vi).IsNormalEnabled():%s\n", (*vi).IsNormalEnabled()?"Yes":"No");
	printf("(*vi).HasNormalOcf():%s\n", (*vi).HasNormalOcf()?"Yes":"No");
//	(*vi).N() = vcg::Point3f(1.0,1.0,1.0); // ERROR
	
	m.vert.EnableNormal();
	printf("(*vi).IsNormalEnabled():%s\n", (*vi).IsNormalEnabled()?"Yes":"No");
	printf("(*vi).HasNormalOcf():%s\n", (*vi).HasNormalOcf()?"Yes":"No");
	(*vi).N() = vcg::Point3f(1.0,1.0,1.0); // OK
	
	m.vert.DisableNormal();
	printf("(*vi).IsNormalEnabled():%s\n", (*vi).IsNormalEnabled()?"Yes":"No");
	printf("(*vi).HasNormalOcf():%s\n", (*vi).HasNormalOcf()?"Yes":"No");
//	(*vi).N() = vcg::Point3f(1.0,1.0,1.0); // ERROR  (again)! 
	return 0;
}
