Reference : http://vcg.isti.cnr.it/vcglib/flags.html



```cpp
// set selected flag manually
MyMesh::VertexPointer vp;
vp = &(mesh.vert[0]);
vp->SetS();
vp = &(mesh.vert[2]);
vp->SetS();

// get all selected flag state
MyMesh::VertexIterator vi;
for(auto &&vi : mesh.vert){
    cout << vi.IsS() << endl;
}
```

