## main.cpp

`FaceConnectedFF` expand current selection to cover the whole connected component.

```cpp
tri::UpdateSelection<CMeshO>::FaceConnectedFF(mesh);
```



Select all the vertices that are touched by at least a single selected faces

```
cnt = tri::UpdateSelection<CMeshO>::VertexFromFaceLoose(mesh);
```



Select ONLY the vertices that are touched ONLY by selected faces

```cpp
cnt = tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(mesh);
```



Select all the faces with at least one selected vertex

```cpp
cnt = tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(mesh);
```



Select ONLY the faces with ALL the vertices selected

```cpp
cnt = tri::UpdateSelection<CMeshO>::FaceFromVertexStrict(mesh);
```



## output

```shell
0
1
2
3
VertexFromFaceLoose: 3
VertexFromFaceStrict: 1
FaceFromVertexLoose: 3
FaceFromVertexStrict: 1
```

