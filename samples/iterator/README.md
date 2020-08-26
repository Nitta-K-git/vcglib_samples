```cpp
// Access to face and vertex via iterator
cout << "iterator" << endl;
MyMesh::FaceIterator fi;
for(fi=mesh.face.begin(); fi!=mesh.face.end(); ++fi){ // iterate each faces
    cout << "face : " << tri::Index(mesh, *fi) << endl; // get face index
}
MyMesh::VertexIterator vi;
for(vi=mesh.vert.begin(); vi!=mesh.vert.end(); ++vi){
    cout << "vert : " << tri::Index(mesh, *vi) << endl;
}

// via pointer
cout << "pointer" << endl;
MyMesh::FacePointer fp;
for(int i=0; i<mesh.FN(); ++i){
    fp = &(mesh.face[i]);
    cout << "face : " << tri::Index(mesh, fp) << endl;
}
MyMesh::VertexPointer vp;
for(int i=0; i<mesh.VN(); ++i){
    vp = &(mesh.vert[i]);
    cout << "vert : " << tri::Index(mesh, vp) << endl;
}
```

