Reference : http://vcg.isti.cnr.it/vcglib/fileformat.html

[By default the following formats are supported for reading:](https://github.com/cnr-isti-vclab/vcglib/blob/devel/wrap/io_trimesh/import.h#L115)

- PLY
- STL
- OFF
- OBJ
- VMI

If you want to read other files, refer to the source file of [each format](https://github.com/cnr-isti-vclab/vcglib/tree/devel/wrap/io_trimesh).



if you use ply file format, you have to add cpp file In CMakeLists.txt like below

```cmake
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
	${VCGLIB_DIR}/wrap/ply/plylib.cpp
)
```



And also you have to add some attribute to data structure like below

```cpp
struct MyUsedTypes : public UsedTypes<	Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType> {};
class MyVertex : public Vertex< MyUsedTypes, vertex::Coord3f, vertex::BitFlags, vertex::VFAdj> {}; // need BitFlags, VFAdj
class MyFace : public Face< MyUsedTypes, face::VertexRef, face::VFAdj, face::BitFlags> {}; // need BitFlags, VFAdj
class MyMesh : public tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};
```



If you forget this, you will get error message like this,

```shell
Missing Component Exception -PerVertexFlags       -
terminate called after throwing an instance of 'vcg::MissingComponentException'
  what():  Missing Component
```



# Result sample

```shell
$ read_write_file.exe ..\..\..\sample_data\octahedron.obj
Removed 0 duplicate and 0 unreferenced vertices from mesh ..\..\..\sample_data\octahedron.obj
face num : 8
vert num : 6
result ply: 0
result stl: 0
```