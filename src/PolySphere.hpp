#ifndef POLYSPHERE_H
#define POLYSPHERE_H

#include <vector>

#include "GeoNode.hpp"
#include <cstdio>

using namespace std;

class PolySphere{
public:
    // radius, segment number
    PolySphere(double radius, int n, double center[3]);
    ~PolySphere();
    // gen layers
    // link layers

    // double pointer with length 9
    // ready for rendering
    vector<double*> getMesh();
    
    // to be able to reposition the nodes
    vector<GeoNode*> getNodes(); // DEPRECATED
    vector<GeoNode*> nodeMesh;

    static void getConnectedTriangles(vector<double*>&,
				      GeoNode*,
				      GeoNode*);
private:
    vector<vector<GeoNode*>> layers;

    
    vector<vector<GeoNode*>> generateHemisphere(double, int, double center[3]);

    void linkHemisphere(vector<vector<GeoNode*>>);

};


#endif
