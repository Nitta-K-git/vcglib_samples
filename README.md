# vcglib_samples
sample codes of vcglib and MeshLab function.

# Introduction

```
The Visualization and Computer Graphics Library (VCG for short) is a open source portable C++ templated library for manipulation, processing and displaying with OpenGL of triangle and tetrahedral meshes.
```

- [VCG Library](http://vcg.isti.cnr.it/vcglib/)([Github](https://github.com/cnr-isti-vclab/vcglib))
- [meshlab](https://www.meshlab.net/)([Github](https://github.com/cnr-isti-vclab/meshlab))

# Get started

## Requirements

- [cmake](https://cmake.org/)
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- C++ compiler(visual studio, gcc, ... etc.)
- [Qt](https://www.qt.io/jp) (not necessary but it's easy to visualize)

## Get library from github

Clone vcglib from github. You should select `devel`branch (`master` branch is too old).

```shell
$ git clone -b devel https://github.com/cnr-isti-vclab/vcglib.git
Cloning into 'vcglib'...
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (5/5), done.
remote: Total 42007 (delta 0), reused 1 (delta 0), pack-reused 42002
Receiving objects: 100% (42007/42007), 17.24 MiB | 1.23 MiB/s, done.
Resolving deltas: 100% (28087/28087), done.
Checking out files: 100% (1063/1063), done.

$ cd vcglib

$ git branch
* devel
```

## How to build with cmake

First, you have to set below environment values

- `VCGLIB_DIR` : path/to/vcglib (e.g. `C:/Users/Public/Documents/GitHub/vcglib`)
- `MESHLAB_DIR` : path/to/MeshLab/source (e.g. `C:/Users/Public/Documents/GitHub/meshlab/src`)
- `EIGEN3_INCLUDE_DIR` : path/to/Eigen3 (e.g. `C:/eigen-3.3.7`)

see this [sample](./samples/hello_mesh) for details.

# Samples

## Basic data structure

- template data structure : [CODE](./samples/template)
- template data structure from MeshLab : [CODE](./samples/template_meshlab)
- access mesh vertices/faces : [CODE](./samples/iterator)
- set flag of vertices/faces : [CODE](./samples/flags)
- expand select function : [CODE](./samples/expand_select)
- get adjacent vertices/faces : [CODE](./samples/adjacent)
-  create mesh using just a vector of coords and a vector of indexes : [CODE](./samples/create_mesh_manually)
-  user defined attribute : [CODE](./samples/user_defined_attr)
-  add and delete vertices/faces : [CODE](./samples/add_del_vert_face)
-  set color each vertex/face and save as ply: [CODE](./samples/set_color)
-  set vertex value and save as ply format : [CODE](./samples/ply_with_vert_value)
-  copy and merge mesh data : [CODE](samples/copy_mesh)



## Read/Write file

-  read and write basic file formats :  [CODE](./samples/read_write_file)
-  create preset object : [CODE](./samples/create_preset_obj)




## Basic algorithm (vcglib functions)

-  convert degree and radian: [CODE](./samples/conv_deg_and_rad)
-  distance between two points : [CODE](./samples/dist_two_points)
-  distance between point and line : [CODE](./samples/dist_point_line)
-  distance between point and plane : [CODE](./samples/dist_point_plane)
-  angle between two vectors : [CODE](./samples/angle_two_vector)
-  quaternion : [CODE](./samples/quaternion)
-  transformation matrix : [CODE](./samples/transformation_matrix)
-  translate matrix with Eigen : [CODE](./samples/matrix_with_eigen)
-  remove non manifold vertices and faces : [CODE](samples/remove_non_manifold) 未完成



## Extra algorithm (implemented in MeshLab)

use MeshLab data structure.

-  template data structure from MeshLab : [CODE](./samples/template_meshlab)
-  calculate curvature : [CODE](samples/curvature)
-  hole filing : [CODE]
-  mesh simplification : [CODE](samples/simplification) 未完成
-  calculate Hausdorff distance : [CODE](samples/Hausdorff_distance) 内容整理 sample target
-  mesh intersection : [CODE]
-  select boundary line : [CODE]
-  simplification : [CODE](./samples/simplification)
-  subdivision : [CODE](./samples/subdivision)



## Visualize with Qt



## Visualize with three.js



# Error handling

`Missing Component Exception -FFAdjacency-`のようなケース

```cpp
// adjacency関連を有効にする(頂点と面でそれぞれ有効化しないといけないものもある)
mesh.vert.EnableVFAdjacency();
mesh.face.EnableFFAdjacency();
mesh.vert.EnableCurvature();
mesh.vert.EnableCurvatureDir();
```

`Assertion failed: f.cFFp(j) != 0, file C:/Users/Public/Documents/GitHub/vcglib/vcg/simplex/face/topology.h, line 39` のようなケース

```cpp
// 初期化時と関係性が変わった場合は更新が必要
tri::UpdateTopology<CMeshO>::FaceFace(mesh);
tri::UpdateTopology<CMeshO>::VertexFace(mesh);
tri::UpdateNormal<CMeshO>::PerFace(mesh);
tri::UpdateNormal<CMeshO>::NormalizePerFace(mesh);
```



# How to port the function of MeshLab

MeshLab implements many functions in a plugin format.
A list of plugins can be found [here](https://github.com/cnr-isti-vclab/meshlab/tree/master/src/meshlabplugins).

Let's take a look at the structure of the [mlsplugin.cpp](https://github.com/cnr-isti-vclab/meshlab/blob/master/src/meshlabplugins/filter_mls/mlsplugin.cpp) file as an example.
The name of the implemented function is defined [here](https://github.com/cnr-isti-vclab/meshlab/blob/master/src/meshlabplugins/filter_mls/mlsplugin.cpp#L87:#L96).
The function for plugins is the [applyFilter](https://github.com/cnr-isti-vclab/meshlab/blob/master/src/meshlabplugins/filter_mls/mlsplugin.cpp#L360) function.

The process branches according to the ID contained in the `filter` variable.
The `applyFiter` function is the starting point of each plug-in process, so if you follow the process from here, you can check all the implementation details.

When searching for the function you want to know how to be implemented on MeshLab, it is recommended way that search function name by using grep command in source directory.

---

# 残り

## 020_trackball

トラックボール表示するだけのサンプル

視点操作はトラックボール使うと楽にできるが、固定シェーダ前提になる

## 021_trackball_mesh

トラックボールとメッシュデータ表示するサンプル

光源用の処理も追加

### メッシュ間の干渉判定

```cpp
// mesh1においてmesh2との干渉面が選択される(mesh2はそのまま)
mesh2.face.EnableMark();
int a = vcg::tri::Clean<CMeshO>::SelectIntersectingFaces(mesh1, mesh2);
```

### end



