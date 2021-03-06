#include "PolyPlane.hpp"


PolyPlane::PolyPlane(double center[3], double range[2], int gridN0){
    vector<GeoNode*> tmp;

    int gridN = (gridN0%2!=0? gridN0: gridN0+1);

    
    double xstep = range[0]/gridN;
    double ystep = range[1]/gridN;

    // generating the nodes
    for(int j=0; j<gridN; ++j){
	vector<GeoNode*> line;
	for(int i=0; i<gridN; ++i){
	    GeoNode *p = new GeoNode(center[0]-range[0]/2+i*xstep,
				     center[1]-range[1]/2+(i%2==0 ? j : j+0.5)*ystep, center[2]);
	    line.push_back(p);
	    nodeMesh.push_back(p);
	}

        nodeArray.push_back(line);
    }

    // doing the linking
    int nj = nodeArray.size();
    for(int j=0; j<nj; ++j){
	int ni = nodeArray[j].size();

	if((j+1)<nj){
	    nodeArray[j][0]->linkNode(nodeArray[j+1][0]);
	    nodeArray[j][ni-1]->linkNode(nodeArray[j+1][ni-1]);
	}
	
	for(int i=0; i<ni; ++i){
	    if((i+1)<ni){
	        nodeArray[j][i]->linkNode(nodeArray[j][i+1]);
		if(i>0 && j+1<nj && i%2==1){
		    nodeArray[j][i]->linkNode(nodeArray[j+1][i-1]);
		    nodeArray[j][i]->linkNode(nodeArray[j+1][i+1]);
		}		    
	    }
	    if((i+2)<ni)
		nodeArray[j][i]->linkNode(nodeArray[j][i+2]);
	}
    }
}

vector<double*> PolyPlane::getMesh(){
    vector<double*> triangles;
    
    int nj = nodeArray.size();
    for(int j=0; j<nj; ++j){
	int ni = nodeArray[j].size();
	if(j+1<nj){
	    // SOmething is wrong somewhere
	    PolySphere::getConnectedTriangles(triangles, nodeArray[j][0], nodeArray[j+1][0]);
	    PolySphere::getConnectedTriangles(triangles, nodeArray[j][ni-1], nodeArray[j+1][ni-1]);
	}
	for(int i=0; i<ni; ++i){
	    if(i+2<ni){
		PolySphere::getConnectedTriangles(triangles, nodeArray[j][i], nodeArray[j][i+2]);
	    }
	}
    }

    return triangles;
}

