Reference : http://vcg.isti.cnr.it/vcglib/adjacency.html



```cpp
// Around the adjacent faces centering on the vertex vp (The order of rotation is undefined)
cout << "VFIterator" << endl;
for (face::VFIterator<CMeshO::FaceType> vfi(vp); !vfi.End(); ++vfi){
    cout << vfi.F()->Index() << endl;
}

// Around the adjacent vertices centering on the vertex vp (The order of rotation is counter clock wise in default option)
cout << "VVOrderedStarFF" << endl;
vector<CMeshO::VertexPointer> vecvt;
face::VFIterator<CMeshO::FaceType> vfi(vp); // set VFIterator from vertex
face::Pos<CMeshO::FaceType> fpos;
fpos.Set(vfi.f, vp); // set face pos from VFIterator
face::VVOrderedStarFF<CMeshO::FaceType>(fpos, vecvt); // get adjacent vertices of vp (counter clock wise)
for(auto &&vp : vecvt){
    cout << vp->Index() << endl;
}

// Around the adjacent faces centering on the vertex vp (The order of rotation is counter clock wise in default option)
cout << "VFOrderedStarFF" << endl;
vector<face::Pos<CMeshO::FaceType>> vecfpos;
face::VFOrderedStarFF<CMeshO::FaceType>(fpos, vecfpos);
cout << "  vert" << endl;
for(auto &&fps : vecfpos){
    cout << fps.V()->Index() << endl; // vert is always same because it's center of ratation
}
cout << "  face" << endl;
for(auto &&fps : vecfpos){
    cout << fps.F()->Index() << endl;
}
```



index of sample data

![Octahedron](../../images/oct_index.PNG)



output

```shell
VFIterator
3
2
1
0
VVOrderedStarFF
1
2
4
5
VFOrderedStarFF
  vert
0
0
0
0
  face
3
0
1
2
```

