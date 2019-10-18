# vcglib_sample
vcglib basic samples

# 概要

公式サイト

- [VCG Library: The VCG Library](http://vcg.isti.cnr.it/vcglib/)
- [cnr-isti-vclab/vcglib: The VCGlib is a C++, templated, no dependency, library for manipulation, processing and cleaning of triangle meshes](https://github.com/cnr-isti-vclab/vcglib)

オープンソースのメッシュデータ処理用のライブラリ。GPLライセンス。

三角形のメッシュデータを主に想定している。データ構造はOpenMeshに似ているが、機能はvcglibの方が豊富である。

meshlabはvcglibベースに作成されている。

# 環境構築

以下の環境で動作確認をした。

- Windows10 64bit
- [Qt 5.13](https://www.qt.io/download)
- cmake 3.15.0
- Visual Studio 2017

## ダウンロード

vcglibのgitから最新版をダウンロードする。branchはmasterではなくdevelにする(masterブランチは更新が止まっている。meshlabでもdevelが使用されている)。

```
C:\Users\nitta\Documents\GitLab>git clone -b devel https://github.com/cnr-isti-vclab/vcglib.git
Cloning into 'vcglib'...
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (5/5), done.
remote: Total 42007 (delta 0), reused 1 (delta 0), pack-reused 42002
Receiving objects: 100% (42007/42007), 17.24 MiB | 1.23 MiB/s, done.
Resolving deltas: 100% (28087/28087), done.
Checking out files: 100% (1063/1063), done.

C:\Users\nitta\Documents\GitLab>cd vcglib

C:\Users\nitta\Documents\GitLab\vcglib>git branch
* devel
```

## cmakeの作成(サンプル)

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.0)
project(simple_template CXX) # (プロジェクト名 言語指定(CXXはC++))

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") # Visual Studio用設定。ソースファイルの文字コードをUTF-8に変更

set( VCGLIB_DIR C:/Users/nitta/Documents/GitLab/vcglib) # ディレクトリのパスは各自の環境に合わせる
include_directories(${VCGLIB_DIR})
message(${VCGLIB_DIR})

set( EIGEN_ROOT C:/Users/nitta/Documents/GitLab/vcglib/eigenlib) # ディレクトリのパスは各自の環境に合わせる
include_directories(${EIGEN_ROOT})
message(${EIGEN_ROOT})

add_executable(vcg_test # exe name
	main.cpp	# 必要なcpp,hファイルを入れていく
)
```

## サンプルプログラムの作成・動作確認

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
	vcg::tri::Octahedron(mesh); // 正八面体のデータを作成
	
	MyMesh::FaceIterator fi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // メッシュの各面を巡回する
		std::cout << vcg::tri::Index(mesh, *fi) << ": "; // 面のインデックスを取得
		for(int i=0; i<fi->VN(); ++i){ // 面の各頂点を巡回する
			std::cout << vcg::tri::Index(mesh, fi->V(i)) << ","; // 頂点のインデックスを取得
		}
		std::cout << std::endl;
	}
	return 1;
}
```

以下のフォルダ構成で置く。

```
.
├── CMakeLists.txt
├── main.cpp
└── build/
```

buildフォルダ内でコンパイルする。

```
C:\Users\nitta\Documents\GitHub\vcglib_samples\000_simple_template\build>cmake .. -G"Visual Studio 15 2017 Win64"
-- Selecting Windows SDK version 10.0.17763.0 to target Windows 10.0.17134.
-- The CXX compiler identification is MSVC 19.16.27031.1
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
C:/Users/nitta/Documents/GitLab/vcglib
C:/Users/nitta/Documents/GitLab/vcglib/eigenlib
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/nitta/Documents/GitHub/vcglib_samples/000_simple_template/build

C:\Users\nitta\Documents\GitHub\vcglib_samples\000_simple_template\build>cmake --build . --config Debug
.NET Framework 向け Microsoft (R) Build Engine バージョン 15.9.21+g9802d43bc3
Copyright (C) Microsoft Corporation.All rights reserved.

  Checking Build System
  Building Custom Rule C:/Users/nitta/Documents/GitHub/vcglib_samples/000_simple_template/CMakeLists.txt
  main.cpp
C:\Users\nitta\Documents\GitLab\vcglib\wrap/callback.h(67): warning C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for deta
ils. [C:\Users\nitta\Documents\GitHub\vcglib_samples\000_simple_template\build\vcg_test.vcxproj]
  C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt\string.h(133): note: 'strcpy' の宣言を確認してください
  vcg_test.vcxproj -> C:\Users\nitta\Documents\GitHub\vcglib_samples\000_simple_template\build\Debug\vcg_test.exe
  Building Custom Rule C:/Users/nitta/Documents/GitHub/vcglib_samples/000_simple_template/CMakeLists.txt
```

動作確認

```
C:\Users\nitta\Documents\GitHub\vcglib_samples\000_simple_template\build>Debug\vcg_test.exe
0: 0,1,2,
1: 0,2,4,
2: 0,4,5,
3: 0,5,1,
4: 3,1,5,
5: 3,5,4,
6: 3,4,2,
7: 3,2,1,
```

---

# 使い方

## ファイルの入出力

参考：[VCG Library: File Formats](http://vcg.isti.cnr.it/vcglib/fileformat.html])

対応しているファイル形式

- import: PLY, STL, OFF, OBJ, 3DS, COLLADA, PTX, V3D, PTS, APTS, XYZ, GTS, TRI, ASC, X3D, X3DV, VRML, ALN
- export: PLY, STL, OFF, OBJ, 3DS, COLLADA, VRML, DXF, GTS, U3D, IDTF, X3D

### PLYファイルの入出力のサンプル



## データアクセス方法

### 面、頂点、エッジの各要素にアクセスする方法

以下のように複数の方法が用意されているので、適宜使い分ける。

- イテレータで巡回
- インデックスで直接指定
- ポインタで直接指定
- 隣接関係を使用

#### イテレータ

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
	vcg::tri::Octahedron(mesh); // 正八面体のデータを作成
	
	MyMesh::FaceIterator fi;
	MyMesh::VertexIterator vi;
	for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
		std::cout << vcg::tri::Index(mesh, *fi) << ",";
	}
	std::cout << std::endl;
	for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
		std::cout << vcg::tri::Index(mesh, *vi) << ",";
	}
	std::cout << std::endl;
	return 1;
}
```

#### インデックスとポインタ

```cpp
	MyMesh::FacePointer fp;
	MyMesh::VertexPointer vp;
	fp = &mesh.face[1];
	vp = &mesh.vert[1];
	std::cout << vcg::tri::Index(mesh, fp) << "," << std::endl;
	std::cout << vcg::tri::Index(mesh, vp) << "," << std::endl;
```

#### 隣接関係

PosとVFIteratorの2種類ある。

##### Pos

メッシュ上の位置は面・頂点・辺で一意に決まる。これを表すデータ型がPosである。



##### VFIterator

頂点が属する面を巡回するためのもの。



## データの可視化

ライブラリ自体にビューワーは付属していないが、OpenGLとQtのQGLWidgetで使用するためのライブラリが用意されているため、それを使うのがおすすめ。





## データ処理のサンプル

vcglibで出来るデータ処理の例

- Non Manifoldになっている箇所の検出
- 頂点・面の追加・削除
- スムージング
- 曲率計算
- 境界線の特定
- 任意の頂点とエッジでつながっている頂点の探索
- 任意の面とエッジを共有する面の探索
- 各頂点・面・メッシュなどに対してユーザーの定義したデータ型を付加することが可能(頂点や面が追加・削除されると付属データも自動的に同期する)
  - フラグ変数などを自由に追加することができる
- 穴埋め
- Qtとの連携前提で
  - 面・頂点の範囲選択
  - テキスト描画

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

## プログラムの雛形

### vcglib only

- 環境変数に以下を設定しておく(パスは各自の環境に合わせること)
  - VCGLIB_DIR C:/Users/nitta/Documents/GitLab/vcglib
  - EIGEN_ROOT C:/Users/nitta/Documents/GitLab/vcglib/eigenlib

```cmake
cmake_minimum_required(VERSION 3.0)
project(proj_name CXX)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${VCGLIB_DIR})
message(${VCGLIB_DIR})

include_directories(${EIGEN_ROOT})
message(${EIGEN_ROOT})

add_executable(exe_name
	main.cpp
)
```

```cpp
#include<vcg/complex/allocate.h>

class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType> {};
class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::BitFlags, vcg::vertex::VFAdj> {};
class MyFace : public vcg::Face< MyUsedTypes, vcg::face::VertexRef, vcg::face::VFAdj> {};
class MyMesh : public vcg::tri::TriMesh<vector<MyVertex>, vector<MyFace> > {};

int main(){
	MyMesh mesh;
	return 1;
}
```

### with QGLWidget



## 頂点、面のインデックス取得

```cpp
std::cout << vcg::tri::Index(mesh, fp) << std::endl;
std::cout << vcg::tri::Index(mesh, vp) << std::endl;
```

## イテレータ処理

```cpp
for(CMeshO::VertexIterator vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
    // vi->
}
for(CMeshO::FaceIterator fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){
    // fi->
}
for(auto &&vp:mesh.vert){ // vertex pointer
    // vp->
}
for(auto &&fp:mesh.face){ // face pointer
    // fp->
}
```

## 法線計算



## Non manifoldの削除



## プリセットオブジェクトの生成

```cpp
#include<vcg/complex/algorithms/create/platonic.h>

MyMesh mesh;
vcg::tri::Octahedron(mesh); // 正八面体のデータを作成
```



