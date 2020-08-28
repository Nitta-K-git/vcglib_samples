## main.cpp

```cpp
// if you use face color, you have to enable color because meshlab has Color4bOcf
mesh.face.EnableColor();

mesh.vert[0].C() = Color4b(0xff,0,0,0xff);
mesh.vert[1].C() = Color4b::Cyan;
mesh.face[0].C() = Color4b::Yellow;

auto flags = tri::io::Mask::IOM_VERTCOLOR|
    tri::io::Mask::IOM_FACECOLOR;
bool binary = false;
int result = tri::io::ExporterPLY<CMeshO>::Save(mesh, "out.ply", flags, binary);
if(result != ply::PlyError::E_NOERROR){
    cout << "error" << endl;
}
```

