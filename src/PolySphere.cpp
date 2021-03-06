#include "PolySphere.hpp"


PolySphere::PolySphere(double radius, int n, double center[3]){
    assert(n>0);

    vector<vector<GeoNode*>> topHemi = generateHemisphere(radius, n, center);
    vector<vector<GeoNode*>> botHemi = generateHemisphere(-radius, n, center);

    vector<GeoNode*> topLastLayer = topHemi.back();
    vector<GeoNode*> botLastLayer = botHemi.back();

    
    linkHemisphere(topHemi);

    // replace last layer pointers
    for(int i=0; i<topLastLayer.size(); ++i){
	// delete botLastLayer[i]; // deleting where the pointer goes to, to leave everything clean
	botLastLayer[i] = topLastLayer[i];
    }
    printf("After dangerous loop\n");
    
    linkHemisphere(botHemi);
    
    // putting all the pointers in the right place
    for(int i=0; i<topHemi.size(); ++i){
	layers.push_back(topHemi[i]);
    }
    // and putting the last ones in reverse to keep it clean
    for(int j=botHemi.size()-2; j>=0; --j){
	layers.push_back(botHemi[j]);
    }


}

vector<vector<GeoNode*>> PolySphere::generateHemisphere(double radius0, int n, double center[3]){

    vector<vector<GeoNode*>> temp;
    GeoNode *p0 = new GeoNode(center[0], center[1], center[2]+radius0);
    vector<GeoNode*> l0;
    l0.push_back(p0);
    
    temp.push_back(l0);

    double radius = fabs(radius0);
    
    // hemisphere simetric 
    double deltaTH = M_PI/(4*(n-1));

    // generates all of the layer points
    for(int i=0; i<(1?2*n:4); ++i){ // n (+1) hemisphere layers
	vector<GeoNode*> currentLayer;

	double th_i = i*deltaTH;
	int nlayer = floor(2*n*(sin(th_i)+1));
	
	for(int j=0; j<nlayer; ++j){ // points on the layer
	    double deltaPHI = 2*M_PI/nlayer;
	    
	    double phi_j = j*deltaPHI;
	    double phi_offset = 0*i*deltaPHI/2;
	    
	    GeoNode *p = new GeoNode(center[0]+radius0*sin(th_i)*cos(phi_j+phi_offset),
				     center[1]+radius0*sin(th_i)*sin(phi_j+phi_offset),
				     center[2]+radius0*cos(th_i));
	    currentLayer.push_back(p);
	    nodeMesh.push_back(p);
	}
	// link inside layer - linked ring thing
	for(int k=0; k<nlayer; ++k){
	    currentLayer[k]->linkNode(currentLayer[(k+1)%(nlayer)]);
	}
	
	temp.push_back(currentLayer);
    }

    return temp;
}


PolySphere::~PolySphere(){
    layers.clear();
}

void PolySphere::linkHemisphere(vector<vector<GeoNode*>> hemisphere){

    // linking 0th layer to 1st layer
    for(int j=0; j<hemisphere[1].size(); ++j){
	hemisphere[1][j]->linkNode(hemisphere[0][0]);
    }
    // printf("Safe\n");
    // linking the folowing layers
    for(int i=2; i<hemisphere.size(); ++i){
	// link layers i-1 (l1) and i (l)
	vector<GeoNode*> l1 = hemisphere[i-1]; // top layer on index i 
	vector<GeoNode*> l2 = hemisphere[i]; // bottom layer on index i
	int n1 = l1.size();
	int n2 = l2.size();
	assert(n1*n2!=0);
	// printf("hi\n");
	// linking all the closest nodes first
	for(int k=0; k<n2; ++k){
	    // printf("I am %p \n", l2[k]);
	    l2[k]->linkNode(l2[k]->getClosest(l1));
	}
	// printf("Safe\n");
	// for(int k=0; k<n1; ++k){
	//     // printf("I am %p \n", l2[k]);
	//     l1[k]->linkNode(l1[k]->getClosest(l2));
	// }

	// Check if theres any node on the top layer that doesnt have any connections on the bottom one
	// for(int topl=0; topl<n1; ++topl){
	//     bool test = false;
	//     for(int botl=0; botl<n2; ++botl){
	// 	test = l1[topl]->isLinked(l2[botl]);
	// 	if(test) break;
	//     }
	    
	//     if(!test){ // then theres a node that doesnt connect to any on the bottom layer
	// 	printf("node %d on layer %d not connected to anything\n", topl, i);
	//     }
	// }
	
	for(int k=0; k<n2; ++k){
	    // GeoNode* p0 = l2[k];
	    // GeoNode* p1 = l2[(k+1)%n2];

	    // index at which the current node k from layer l2 is connected to
	    int l1p0_index=l2[k]->getClosest_index(l1);
	    
	    // printf("boom\n");
	    // continue;
	    // each node has only made 1 connection so far
	    if(!l2[(k+1)%n2]->isLinked(l1[l1p0_index]) &&
	       !l2[k]->isLinked(l1[(l1p0_index+1)%n1])){
		// so if the next node on the layer (p1) is not connected to the node which the current node (p0) is connected to (from the previous layer), then its necessary to add a new connection

		// then there are 2 options, either
		// p1 to connect to l1p0, or
		// p0 to connect with l1p1  
		double dist0 = GeoNode::getDistance(l2[k], l1[(l1p0_index+1)%n1]);
		double dist1 = GeoNode::getDistance(l2[(k+1)%n2], l1[l1p0_index]);

		if(dist0<dist1){
		    l2[k]->linkNode(l1[(l1p0_index+1)%n1]);
		} else{
		    l2[(k+1)%n2]->linkNode(l1[l1p0_index]);
		}
		
	    }
	    
	}
	
    }
}

vector<double*> PolySphere::getMesh(){
    vector<double*> triangles;
    for(int i=1; i<layers.size(); ++i){
	if(layers[i].size()>1)
	    for(int j=0; j<layers[i].size(); ++j){
		// printf("testing %p %p size: %ld\n", layers[i][j], layers[i][(j+1)%layers[i].size()], layers[i].size());
		assert(layers[i][j]->isLinked(layers[i][(j+1)%layers[i].size()]));
	    
		getConnectedTriangles(triangles,
				      layers[i][j],
				      layers[i][(j+1)%layers[i].size()]);
	    }
    }
    return triangles;
}

vector<GeoNode*> PolySphere::getNodes(){
    vector<GeoNode*> nodes;
    for(int i=0; i<layers.size(); ++i){
	for(int j=0; j<layers[i].size(); ++j)
	    nodes.push_back(layers[i][j]);
    }

    return nodes;
}

void PolySphere::getConnectedTriangles(vector<double*>& triangles,
					   GeoNode* from,
					   GeoNode* to){
    assert(to->isLinked(from));
     
    // printf("sz: %ld\n",from->links.size());
    for(int i=0; i<from->links.size(); ++i){
	for(int j=0; j<to->links.size(); ++j){
	    if(from->links[i] == to->links[j]){ // then there is a triangle
	        // order: 1st vertex, 2nd vertex, 3rd vertex
	        double* triag = new double[9];

		for(int k=0; k<3; ++k){
		    triag[k] = from->coords[k];
		    triag[k+3] = to->coords[k];
		    triag[k+6] = from->links[i]->coords[k]; 
		}
		triangles.push_back(triag);
	    }
	}
    }

}

