#ifndef GEONODE_H
#define GEONODE_H

#include <cmath>
#include <cassert>

#include <vector>
#include <cstdio>

using namespace std;

// geometric node 
class GeoNode{
public:
    
    GeoNode(double coords0[3]);
    GeoNode(double, double, double);

    GeoNode(const GeoNode&);
    const GeoNode& operator=(const GeoNode&);
    
    void setPosition(double coords0[3]);
    void setPosition(double, double, double);

    bool isLinked(GeoNode*);
    
    void linkNode(GeoNode*);
    void unlinkNode(GeoNode*);
    
    GeoNode* getClosest();
    GeoNode* getClosest(const vector<GeoNode*>&);
    int getClosest_index(const vector<GeoNode*>&);
    
    double coords[3];
    vector<GeoNode*> links;

    static double getDistance(GeoNode*, GeoNode*);

    void printCoordinates();
    
private:
    // static uint linkID;

    int findLinkedPos(GeoNode*);  
    
};

#endif
