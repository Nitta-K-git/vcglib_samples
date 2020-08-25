# hello_mesh

CMakeLists.txt

```cmake
cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

message(${ProjectId})

# If you use visual studio, set the character code to UTF8
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# In debug mode, add 'd' to the end of the executable file name
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# set output file directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    C:/Users/Public/Documents/GitHub/vcglib
    C:/eigen-3.3.7
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
```

main.cpp

```cpp
#include<vcg/complex/algorithms/create/platonic.h>
#include<vcg/complex/allocate.h>

class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(){
	MyMesh mesh;
	vcg::tri::Octahedron(mesh); // create preset mesh data
	
	MyMesh::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // iterate each faces
		std::cout << vcg::tri::Index(mesh, *fi) << ": "; // get face index
		for(int i=0; i<fi->VN(); ++i){ // iterate each vertex of face
			std::cout << vcg::tri::Index(mesh, fi->V(i)) << ","; // get vertex index
		}
		std::cout << std::endl;
	}
	return 1;
}
```

# build

## MinGW

```shell
$ cmake .. -G"MinGW Makefiles"
-- The CXX compiler identification is GNU 9.2.0
-- Check for working CXX compiler: C:/MinGW/bin/g++.exe
-- Check for working CXX compiler: C:/MinGW/bin/g++.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
hello_mesh
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/Public/Documents/GitHub/vcglib_samples/samples/hello_mesh/build

$ cmake --build . --config release
Scanning dependencies of target hello_mesh
[ 50%] Building CXX object CMakeFiles/hello_mesh.dir/main.cpp.obj
[100%] Linking CXX executable hello_mesh.exe
[100%] Built target hello_mesh

$ hello_mesh.exe
0: 0,1,2,
1: 0,2,4,
2: 0,4,5,
3: 0,5,1,
4: 3,1,5,
5: 3,5,4,
6: 3,4,2,
7: 3,2,1,
```

## Visual Studio 2017

```shell
$ cmake .. -G"Visual Studio 15 2017 Win64" -DCMAKE_CXX_FLAGS="/utf-8 /EHsc"
-- Selecting Windows SDK version 10.0.17763.0 to target Windows 10.0.19041.
-- The CXX compiler identification is MSVC 19.16.27042.0
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
hello_mesh
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/Public/Documents/GitHub/vcglib_samples/samples/hello_mesh/build

$ cmake --build . --config release
.NET Framework 向け Microsoft (R) Build Engine バージョン 15.9.21+g9802d43bc3
Copyright (C) Microsoft Corporation.All rights reserved.

  Checking Build System
  Building Custom Rule C:/Users/Public/Documents/GitHub/vcglib_samples/samples/hello_mesh/CMakeLists.txt
  main.cpp
  hello_mesh.vcxproj -> C:\Users\Public\Documents\GitHub\vcglib_samples\samples\hello_mesh\build\Release\hello_mesh.exe
  Building Custom Rule C:/Users/Public/Documents/GitHub/vcglib_samples/samples/hello_mesh/CMakeLists.txt

$ Release\hello_mesh.exe
0: 0,1,2,
1: 0,2,4,
2: 0,4,5,
3: 0,5,1,
4: 3,1,5,
5: 3,5,4,
6: 3,4,2,
7: 3,2,1,
```

