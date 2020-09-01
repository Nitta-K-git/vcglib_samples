#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

int main(){
	{
		Point3m p(10,10,10);
		Point3m z(0,0,10);
		
		// Quaternion that rotates 90 degrees around the vector z direction (rotation is centered on the origin)
		Quaternionf qua(math::ToRad(90.0f),z);
		Point3m q = qua.Rotate(p);
		printf("(%f,%f,%f)\n", q.X(),q.Y(),q.Z());
	}
	
	// Superposing vector A on vector B
	{
		Point3m A(10,10,10), B(-10,0,0);
		Quaternionf atob(Angle(A,B),A^B);
		Point3m C = atob.Rotate(A);
		printf("(%f,%f,%f)\n", C.X(),C.Y(),C.Z());
	}
	return 1;
}

