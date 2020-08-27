#include <vcg/complex/complex.h>

using namespace vcg;
using namespace std;

int main(){
	vcg::Point3f a(0,0,1), b(1,1,0);
	float ang = vcg::Angle(a, b);
	cout << vcg::math::ToDeg(ang) << endl;
	
	return 1;
}
