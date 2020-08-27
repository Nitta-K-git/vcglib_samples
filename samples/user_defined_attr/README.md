http://vcg.isti.cnr.it/vcglib/attributes.html



You can add original attribute to each vertices, faces, edges or meshes.

```cpp
// define original attribute per vertex, face, mesh
CMeshO::PerVertexAttributeHandle<int> vert_ex_id;
vert_ex_id = tri::Allocator<CMeshO>::GetPerVertexAttribute<int>(mesh, string("vert_ex_id"));

CMeshO::PerFaceAttributeHandle<bool> face_ex_flag;
face_ex_flag = tri::Allocator<CMeshO>::GetPerFaceAttribute<bool>(mesh, string("face_ex_flag"));

CMeshO::PerMeshAttributeHandle<vector<int>> mesh_ex_vect;
mesh_ex_vect = tri::Allocator<CMeshO>::GetPerMeshAttribute<vector<int>>(mesh, string("mesh_ex_vect"));

// you can set/get user-defined attr via iterator, pointer, index, Vertex/FaceType
cout << "vert attr" << endl;
tri::Allocator<CMeshO>::ClearPerVertexAttribute<int>(mesh, vert_ex_id, -1);
vert_ex_id[1] = 30;
CMeshO::VertexIterator vi = mesh.vert.begin();
vert_ex_id[vi] = 10;
CMeshO::VertexPointer vp = &(mesh.vert[3]);
vert_ex_id[vp] = 100;
for(CMeshO::VertexType &vt : mesh.vert)
    cout << vert_ex_id[vt] << endl;

cout << "face attr" << endl;
for(auto &&f : mesh.face)
    face_ex_flag[f] = false;
CMeshO::FaceIterator fi = mesh.face.begin();
face_ex_flag[fi] = true;
CMeshO::FacePointer fp = &(mesh.face[3]);
face_ex_flag[fp] = true;
for(CMeshO::FaceType &ft : mesh.face)
    cout << face_ex_flag[ft] << endl;

// use vector attribute
cout << "mesh attr" << endl;
vector<int> *pMeshVect = nullptr;
pMeshVect = &mesh_ex_vect();
pMeshVect->clear();
for(int i=5; i<10; ++i)
    pMeshVect->push_back(i);
for(auto &&a : *pMeshVect)
    cout << a << endl;
```

## output

```shell
vert attr
10
30
-1
100
face attr
1
0
0
1
mesh attr
5
6
7
8
9
delete attr
1
0
```

