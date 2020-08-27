## main.cpp

```cpp
#include <vcg/complex/complex.h>

using namespace vcg;
using namespace std;

int main(){
	float a = vcg::math::ToRad(90.0f);
	float b = vcg::math::ToDeg(a);
	
	cout << a << endl;
	cout << b << endl;
	
	return 1;
}
```



## output

```shell
1.5708
90
```

