```cpp
// Create a simple triangles mesh using just a vector of coords and a vector of indexes
vector<Point3f> coordVec;
vector<Point3i> indexVec;

coordVec.push_back(Point3f(0,0,0));
coordVec.push_back(Point3f(10,0,0));
coordVec.push_back(Point3f(0,10,0));
coordVec.push_back(Point3f(0,0,10));

indexVec.push_back(Point3i(0,1,3));
indexVec.push_back(Point3i(0,2,1));
indexVec.push_back(Point3i(0,3,2));
indexVec.push_back(Point3i(1,2,3));

tri::BuildMeshFromCoordVectorIndexVector(mesh,coordVec,indexVec);
```

