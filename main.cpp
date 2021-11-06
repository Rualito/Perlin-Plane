
#include "FastNoise.h"

#include <cstdio>
#include <cstdlib>

#include <GL/glut.h>
#include <GL/gl.h>

// #include "PolySphere.hpp"
#include "PolyPlane.hpp"
// using namespace glm;


vector<double*> sphereMesh;
vector<GeoNode*> nodeMesh;

vector<GeoNode*> planeNodeMesh;
vector<double*> planeMesh;

double r=5, th=M_PI/4, phi=0;

#include "ColorManager.hpp"
#include "mainRender.hpp"


void applyPerlinToMesh(vector<GeoNode*> nodes){
    // figure out how perlin works

    FastNoise fst;

    fst.SetFrequency(0.2);
    fst.SetFractalOctaves(16);
    
    for(int i=0; i<nodes.size(); ++i){
	nodes[i]->coords[2] = fst.GetSimplexFractal(nodes[i]->coords[0],
						    nodes[i]->coords[1]);
    }
}

ColorManager cm;

void renderScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // Set the camera
    gluLookAt(	r*sin(th)*cos(phi),
		r*sin(th)*sin(phi),
		r*cos(th),
		0, 0,  0,
     		0,    0,  1);
    // glBegin(GL_TRIANGLES);
    // glVertex3f(-1.0f,-1.0f, 0.0f);
    // glVertex3f( 1.0f, 0.0f, 0.0);
    // glVertex3f( 0.0f, 1.0f, 0.0);
    // glEnd();
    renderAxis();

    
    // glColor3f(0.5,0.5,0.5);
    // renderMesh(sphereMesh, GL_TRIANGLE_STRIP);
    // glColor3f(0.9,0.9,0.9);
    // renderNodeMesh(nodeMesh);
    // renderMesh(sphereMesh, GL_LINE_STRIP);
    // glColor3f(1,1,1);

    // renderLinks(nodeMesh);
    
    renderMesh(planeMesh, GL_TRIANGLE_STRIP);
    // renderNodeMesh(planeNodeMesh);

    // glColor3f(0,0,0);
    // renderLinks(planeNodeMesh);	// 
    // renderNodeMesh(nodeMesh);
    
    glutSwapBuffers();
}




int main(int argc, char **argv){
    
    printf("hello everyone\n");

    cm.setColors(Vector3f(2, 189, 120)/255.f,
		 //Vector3f(2, 189, 120)/255.f);
		  Vector3f(255, 247, 0)/255.f);
    
    double center[3]{0,0,0}; 	// 
    // PolySphere sphere(1, 6, center);
    
    // Note: probably better to use GL_TRIANGLE_STRIP
    // sphereMesh = sphere.getMesh();
    // nodeMesh = sphere.nodeMesh;

    double range[2]{20,20};
    double gridN = 200;

    printf("Part1\n");
    
    PolyPlane plane(center, range, gridN);
    planeNodeMesh = plane.nodeMesh;
    applyPerlinToMesh(plane.nodeMesh);
    planeMesh = plane.getMesh();


    glutInit(&argc, argv);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(1200,800);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    printf("Part1\n");

    
    glutCreateWindow("Test Window");

    // Register Callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}


// vector<GeoNode*> generatePlaneMesh(double center[3], double range[2], int gridN){
//     vector<vector<GeoNode*>> temp0;
//     vector<GeoNode*> tmp;
    
//     double xstep = range[0]/gridN;
//     double ystep = range[1]/gridN;

//     // generating the nodes
//     for(int j=0; j<gridN; ++j){
// 	vector<GeoNode*> line;
// 	for(int i=0; i<gridN; ++i){
// 	    GeoNode *p = new GeoNode(center[0]-range[0]/2+i*xstep,
// 				     center[1]-range[1]/2+(i%2==0 ? j : j+0.5)*ystep, center[2]);
// 	    line.push_back(p);
// 	    tmp.push_back(p);
// 	}

// 	temp0.push_back(line);
//     }

//     int nj = temp0.size();
//     // doing the linking
//     for(int j=0; j<nj; ++j){
// 	int ni = temp0[j].size();

// 	if((j+1)<nj){
// 	    temp0[j][0]->linkNode(temp0[j+1][0]);
// 	    temp0[j][ni-1]->linkNode(temp0[j+1][ni-1]);
// 	}
	
// 	for(int i=0; i<ni; ++i){
	    
// 	    if((i+1)<ni){
// 		temp0[j][i]->linkNode(temp0[j][i+1]);
		
// 		if(i>0 && j+1<nj && i%2==1){
// 		    temp0[j][i]->linkNode(temp0[j+1][i-1]);
// 		    temp0[j][i]->linkNode(temp0[j+1][i+1]);
// 		}		    
// 	    }
// 	    if((i+2)<ni)
// 		temp0[j][i]->linkNode(temp0[j][i+2]);
	    
	    
// 	}
//     }

//     return tmp;
// }
