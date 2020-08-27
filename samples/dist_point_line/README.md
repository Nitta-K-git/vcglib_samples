## main.cpp

```cpp
#include <vcg/complex/complex.h>

using namespace vcg;
using namespace std;

int main(){
	Point3f A0(0,0,0), A1(10,0,0);
	vcg::Line3f line(A0, A1-A0);
	
	Point3f p(20,10,0);
	
	Point3f B = line.ClosestPoint(p);
	float a = vcg::Distance(line, p);
	
	printf("(%f,%f,%f)\n", B.X(), B.Y(), B.Z());
	cout << a << endl;
	
	return 1;
}
```



## output

```shell
(20.000000,0.000000,0.000000)
10
```

