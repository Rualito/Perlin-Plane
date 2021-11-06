#ifndef POLYPLANE_H
#define POLYPLANE_H

#include "GeoNode.hpp"
#include "PolySphere.hpp"

#include <vector>

using namespace std;

class PolyPlane{
public:
    PolyPlane(double center[3], double range[2], int gridN);
    
    
    vector<double*> getMesh();
    
    vector<GeoNode*> nodeMesh;
    
private:
    vector<vector<GeoNode*>> nodeArray;
};

#endif
