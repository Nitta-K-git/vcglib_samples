## main.cpp

```cpp
#include <vcg/complex/complex.h>

using namespace vcg;
using namespace std;

int main(){
	Point3f a(0,0,0), b(1,1,0);
	
	float l2 = vcg::SquaredDistance(a, b); // 二乗距離
	float l = vcg::Distance(a, b);
	
	cout << l2 << endl;
	cout << l << endl;
	
	return 1;
}
```



## output

```shell
2
1.41421
```

