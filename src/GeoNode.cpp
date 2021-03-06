#include "GeoNode.hpp"



GeoNode::GeoNode(double coords0[3]) {
    for(int i=0; i<3; ++i){
	coords[i] = coords0[i];
    }
    links.clear();
}
GeoNode::GeoNode(double x, double y, double z) {
    coords[0]=x;
    coords[1]=y;
    coords[2]=z;
    links.clear();
}


GeoNode::GeoNode(const GeoNode& node){
    coords[0]=node.coords[0];
    coords[1]=node.coords[1];
    coords[2]=node.coords[2];

    links = node.links;
}

const GeoNode& GeoNode::operator=(const GeoNode& node){
    coords[0]=node.coords[0];
    coords[1]=node.coords[1];
    coords[2]=node.coords[2];

    links.clear();
    links = node.links;

    return *this;
}

void GeoNode::setPosition(double coords0[3]){
    for(int i=0; i<3; ++i){
	coords[i] = coords0[i];
    }
}
void GeoNode::setPosition(double x, double y, double z){
    coords[0]=x;
    coords[1]=y;
    coords[2]=z;

}


int GeoNode::findLinkedPos(GeoNode* node){
    for(int i=0; i<links.size(); ++i){
	if(links[i] == node){
	    return i;
	}
    }
    return -1;
}

bool GeoNode::isLinked(GeoNode* node){
    return (findLinkedPos(node)>-1);
}

void GeoNode::linkNode(GeoNode* node){
    if(isLinked(node) && node!=this) // cant link to an already linked node or to itself
	return;
    // printf("%p now linking to %p\n", this, node);
    links.push_back(node);
    node->linkNode(this);
}

void GeoNode::unlinkNode(GeoNode* node){
    int index = findLinkedPos(node);
    if(index>-1){
	links.erase(links.begin()+index);
	node->unlinkNode(this);
    }
}

double GeoNode::getDistance(GeoNode *node1, GeoNode* node2){
    return sqrt( (node1->coords[0]-node2->coords[0])*(node1->coords[0]-node2->coords[0]) +
		 (node1->coords[1]-node2->coords[1])*(node1->coords[1]-node2->coords[1]) +
		 (node1->coords[2]-node2->coords[2])*(node1->coords[2]-node2->coords[2]));
}

GeoNode* GeoNode::getClosest(){
    if(links.size()==0)
	return NULL;
    
    double min = getDistance(this, links[0]);
    int index = 0;
    
    for(int i=1; i<links.size(); ++i){
	
	double dist = getDistance(this, links[i]);
	if( dist<min){
	    min = dist;
	    index = i;
	}
    }
    return links[index];
}

GeoNode* GeoNode::getClosest(const vector<GeoNode*>& links_){
    if(links_.size()==0)
	return NULL;
    
    double min = getDistance(this, links_[0]);
    int index = 0;
    
    for(int i=1; i<links_.size(); ++i){
	double dist = getDistance(this, links_[i]);
	// printf("dist to %p is %f\n", links_[i], dist);
	if(dist<min){
	    min = dist;
	    index = i;
	}
    }
    return links_[index];
}
int GeoNode::getClosest_index(const vector<GeoNode*>& links_){
    if(links_.size()==0)
	return -1;
    
    double min = getDistance(this, links_[0]);
    int index = 0;
    
    for(int i=1; i<links_.size(); ++i){
	double dist = getDistance(this, links_[i]);
	// printf("dist to %p is %f\n", links_[i], dist);
	if(dist<min){
	    min = dist;
	    index = i;
	}
    }
    return index;
}

void GeoNode::printCoordinates(){
    printf("x: %0.2f; y: %0.2f; z: %0.2f\n",coords[0], coords[1], coords[2]);
}
