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

```
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
- `MESHLAB_DIR` : path/to/MeshLab/source (e.g. `C:/Users/Public/Documents/GitHub/meshlab_latest/src`)
- `EIGEN3_INCLUDE_DIR` : path/to/Eigen3 (e.g. `C:/eigen-3.3.7`)

see this [sample](./samples/hello_mesh) for details.

# Samples

## Data structure

- template data structure : [CODE](./samples/template)
- template data structure from MeshLab : [CODE](./samples/template_meshlab)
- access mesh vertices/faces : [CODE](./samples/iterator)
- set flag of vertices/faces : [CODE](./samples/flags)
- get adjacent vertices/faces : [CODE](./samples/adjacent)
-  create mesh using just a vector of coords and a vector of indexes : [CODE](./samples/create_mesh_manually)
-  user defined attribute : [CODE](./samples/user_defined_attr)



## Read/Write file

-  read and write basic file formats :  [CODE](./samples/read_write_file)
-  create preset object : [CODE](./samples/create_preset_obj)




## Basic algorithm (vcglib functions)

-  convert degree and radian: [CODE](./samples/conv_deg_and_rad)
-  distance between two points : [CODE](./samples/dist_two_points)
-  distance between point and line : [CODE](./samples/dist_point_line)
-  distance between point and plane : [CODE](./samples/dist_point_plane)
-  angle between two vectors : [CODE](./samples/angle_two_vector)
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()



## Extra algorithm (implemented in MeshLab)

use MeshLab data structure.

-  template data structure from MeshLab : [CODE](./samples/template_meshlab)
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()
-  : [CODE]()



## Visualize with Qt







---

# サンプルファイル

## 015_attr_sample

参考リンク

- [trimesh_attribute.cpp](http://www.vcglib.net/trimesh__attribute_8cpp_source.html)
- [Allocating and Deleting mesh elements](http://www.vcglib.net/allocation.html)

## 020_trackball

トラックボール表示するだけのサンプル

視点操作はトラックボール使うと楽にできるが、固定シェーダ前提になる

## 021_trackball_mesh

トラックボールとメッシュデータ表示するサンプル

光源用の処理も追加

---

# Tips

## 頂点、面の追加

```cpp
void appendMesh(MyMesh &m1, MyMesh &m2){ // dst, src
	auto vi = vcg::tri::Allocator<MyMesh>::AddVertices(m1, m2.VN()); // 最初に追加する頂点数を丸ごと確保してから、座標値を代入
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.VN(); ++i){
		vi[i].P() = m2.vert[i].P();
	}
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	auto fi = vcg::tri::Allocator<MyMesh>::AddFaces(m1, m2.FN()); // fiは追加する開始点から始まるなので、追加元のデータインデックスをそのまま使える
	std::cout << m1.vn << ":" << m1.fn << std::endl;
	for(int i=0; i<m2.FN(); ++i){
		int idx0 = vcg::tri::Index(m2, m2.face[i].V(0));
		int idx1 = vcg::tri::Index(m2, m2.face[i].V(1));
		int idx2 = vcg::tri::Index(m2, m2.face[i].V(2));
		std::cout << idx0 << ":" << idx1 << ":" << idx2 << std::endl;
		fi[i].V(0) = &vi[idx0];
		fi[i].V(1) = &vi[idx1];
		fi[i].V(2) = &vi[idx2];
	}
}
```

## 選択した面につながっている面を再帰的に選択

```cpp
vcg::tri::UpdateSelection<CMeshO>::FaceConnectedFF(mesh);
```

## 面と頂点の間の選択状態を共有

```cpp
// 選択された面に属する頂点を選択(OR) (Select all the vertices that are touched by at least a single selected faces)
cnt = vcg::tri::UpdateSelection<CMeshO>::VertexFromFaceLoose(mesh); 
// 選択された面に属する頂点を選択(AND) (Select all the vertices having all the faces incident on them selected)
cnt = vcg::tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(mesh); 
```

## 面、頂点の削除

```cpp
vcg::tri::Allocator<CMeshO>::DeleteFace(mesh, *fi);
vcg::tri::Allocator<CMeshO>::DeleteVertex(mesh, *vi);
```



## 変換行列の作成例

```cpp
// C:\Users\nitta\Documents\GitHub\meshlab\src\meshlabplugins\filter_meshing\meshfilter.cpp
case FP_ROTATE :
{
    Matrix44m trRot, trTran, trTranInv, transfM;
    Point3m axis, tranVec;

    switch(par.getEnum("rotAxis"))
    {
        case 0: axis=Point3m(1,0,0); break;
        case 1: axis=Point3m(0,1,0);break;
        case 2: axis=Point3m(0,0,1);break;
        case 3: axis=par.getPoint3m("customAxis");break;
    }
    switch(par.getEnum("rotCenter"))
    {
        case 0: tranVec=Point3m(0,0,0); break;
        case 1: tranVec= m.cm.Tr * m.cm.bbox.Center(); break;
        case 2: tranVec=par.getPoint3m("customCenter");break;
    }

    float angleDeg= par.getDynamicFloat("angle");
    float snapAngle = par.getFloat("snapAngle");
    if(par.getBool("snapFlag"))
    { 
        angleDeg = floor(angleDeg / snapAngle)*snapAngle;
        par.setValue("angle", DynamicFloatValue(angleDeg));
    }

    trRot.SetRotateDeg(angleDeg,axis);
    trTran.SetTranslate(tranVec);
    trTranInv.SetTranslate(-tranVec);
    transfM = trTran*trRot*trTranInv;

    ApplyTransform(md,transfM,par.getBool("allLayers"),par.getBool("Freeze"));  

} break;
```

## Eigenとの変換

```cpp
template <typename T>
using RMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
typedef RMatrix<MESHLAB_SCALAR> RMatrixm;
using RefRMatrix = Eigen::Ref<const RMatrixm>;
using EVector = Eigen::Vector3f;
```

```cpp
vcg::Quaternionf qua(vcg::Angle(y01,Y),y01^Y);
Matrix44m M;
RMatrixm eigenM = RMatrixm::Identity(4,4); // 初期化時にサイズを指定すること
qua.ToMatrix(M);
M.ToEigenMatrix<RMatrixm>(eigenM);
```

```cpp
Matrix44m M;
M.FromEigenMatrix<Eigen::Ref<const RMatrixm>>(ME);
vcg::tri::UpdatePosition<CMeshO>::Matrix(a.mesh, M);
vcg::tri::UpdateBounding<CMeshO>::Box(a.mesh);
```

## 変換行列の適用

```cpp
tri::UpdatePosition<CMeshO>::Matrix(m->cm, m->cm.Tr,true);
tri::UpdateBounding<CMeshO>::Box(m->cm);
m->cm.Tr.SetIdentity();
```

### 点と面の距離

```cpp
vcg::Plane3f plate;

Point3m a(0,0,10),b(10,0,10),c(0,10,10);
plate.Init(a,b,c);

Point3m p(10,10,0);
Point3m q = plate.Projection(p); // 点から面への垂線の交点
float l = vcg::SignedDistancePlanePoint(plate, p); // 点と面の距離
```

### メッシュ間の干渉判定

```cpp
// mesh1においてmesh2との干渉面が選択される(mesh2はそのまま)
mesh2.face.EnableMark();
int a = vcg::tri::Clean<CMeshO>::SelectIntersectingFaces(mesh1, mesh2);
```

### Quaternion

```cpp
Point3m p(10,10,10);
Point3m z(0,0,10);
vcg::Quaternionf qua(vcg::math::ToRad(90.0f),z); // ベクトルz方向を中心に90度回転するクォータニオン(回転は原点中心)
Point3m q = qua.Rotate(p);

// ベクトルAをベクトルBに重ねる処理
vcg::Quaternionf qua(vcg::Angle(A,B),A^B);
Point3m q = qua.Rotate(A);
```

メッシュの座標変換

```cpp
Matrix44m M;
M.SetTranslate(Point3m(1,1,1));
vcg::tri::UpdatePosition<CMeshO>::Matrix(mesh, M);
vcg::tri::UpdateBounding<CMeshO>::Box(mesh);

```





