## main.cpp

### add

```cpp
// add vertices and faces
size_t v_add_num = 3;
CMeshO::VertexIterator vi = tri::Allocator<CMeshO>::AddVertices(mesh, v_add_num);
size_t f_add_num = 1;
CMeshO::FaceIterator fi = tri::Allocator<CMeshO>::AddFaces(mesh, f_add_num);

vi[0].P() = Point3f(1,1,1);
vi[1].P() = Point3f(1,0,0);
vi[2].P() = Point3f(2,2,2);

fi[0].V(0) = &(vi[0]);
fi[0].V(1) = &(vi[1]);
fi[0].V(2) = &(vi[2]);
```

### delete

If you want to delete a vertex, you must first delete the face to which it belongs.

So you need to get the faces that belong to the vertices.

See get adjacent vertices/faces : [CODE](../samples/adjacent)

```cpp
mesh.vert.EnableVFAdjacency();
mesh.face.EnableFFAdjacency();
mesh.face.EnableVFAdjacency();

CMeshO::VertexPointer vp = &(mesh.vert[0]);
tri::Allocator<CMeshO>::DeleteVertex(mesh, *vp);

// if you delete vertices, you need to consider whether the vertices belong to a face.
for (face::VFIterator<CMeshO::FaceType> vfi(vp); !vfi.End(); ++vfi){
    CMeshO::FacePointer fp = vfi.F();
    tri::Allocator<CMeshO>::DeleteFace(mesh, *fp);
}

// you have to update some informations
tri::Allocator<CMeshO>::CompactEveryVector(mesh);
tri::UpdateTopology<CMeshO>::FaceFace(mesh);
tri::UpdateTopology<CMeshO>::VertexFace(mesh);
```

### append mesh

```cpp
CMeshO mesh1, mesh2;
tri::Hexahedron(mesh1);
tri::Tetrahedron(mesh2);

bool selected = false;
bool adjFlag = true;
tri::Append<CMeshO,CMeshO>::Mesh(mesh1, mesh2, selected, adjFlag); // copy with adj information
```



## output

```shell
initial state
vp: 0 1 2 3
fp: 0 1 2 3
added state
vp: 0 1 2 3 4 5 6
fp: 0 1 2 3 4
delete
append mesh 1
append mesh: 12:16
append mesh 2
append mesh: 12:16
```

