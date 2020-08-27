#include <vcg/complex/complex.h>

using namespace vcg;
using namespace std;

int main(){
	Plane3f plate;
	
	Point3f a(0,0,0),b(10,0,0),c(0,10,0);
	plate.Init(a,b,c);
	
	Point3f p(20,20,20);
	Point3f q = plate.Projection(p);
	float l = SignedDistancePlanePoint(plate, p);
	
	printf("(%f,%f,%f)\n", q.X(), q.Y(), q.Z());
	cout << l << endl;
	
	return 1;
}
