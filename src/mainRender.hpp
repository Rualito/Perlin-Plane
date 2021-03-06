void renderTriangle(double *triag, GLenum mode){

    glBegin(mode);

    // glBegin(GL_TRIANGLE_STRIP);
    glColor4f((triag[0]+1)/2, (triag[1]+1)/2, (triag[2]+1)/2, (triag[2]+1)/2);
    glVertex3f(triag[0], triag[1], triag[2]);

    glColor4f((triag[3]+1)/2, (triag[4]+1)/2, (triag[5]+1)/2, (triag[5]+1)/2);
    glVertex3f(triag[3], triag[4], triag[5]);

    glColor4f((triag[6]+1)/2, (triag[7]+1)/2, (triag[8]+1)/2, (triag[8]+1)/2);
    glVertex3f(triag[6], triag[7], triag[8]);
    glColor3f(1,1,1);
    glEnd();
}

void renderTriangle(double *triag, double* color, GLenum mode){

    glBegin(mode);

    // glBegin(GL_TRIANGLE_STRIP);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(triag[0], triag[1], triag[2]+1);

    glColor3f(color[3], color[4], color[5]);
    glVertex3f(triag[3], triag[4], triag[5]+1);

    glColor3f(color[6], color[7], color[8]);
    glVertex3f(triag[6], triag[7], triag[8]+1);

    glColor3f(1,1,1);
    glEnd();
}


void renderMesh(vector<double*> mesh, GLenum mode){
    for(int i=0; i<mesh.size(); ++i)
	renderTriangle(mesh[i], mode);
}

void renderZColorMesh(vector<double*> mesh, ColorManager cm,GLenum mode){
    
    for(int i=0; i<mesh.size(); ++i){
	double *colors = new double[9];

	for(int k=0; k<3; ++k){
	    Vector3f cvs = cm.linePath((mesh[i][3*k+2]+1)*0.2);
	    // printf("h %f\n", (mesh[i][3*k+2]+1)*0.5);
	    colors[3*k+0]=cvs.x;
	    colors[3*k+1]=cvs.y;
	    colors[3*k+2]=cvs.z;
	}
	
	renderTriangle(mesh[i], colors, mode);

	delete[] colors;
    }
}

void renderNodeMesh(vector<GeoNode*> nodeMesh){
    // Needs to be ordered
    glBegin(GL_LINE_STRIP);
    // glBegin(GL_POINTS);
    for(int i=0; i<nodeMesh.size(); ++i){
	glVertex3f(nodeMesh[i]->coords[0],
		   nodeMesh[i]->coords[1],
		   nodeMesh[i]->coords[2]);
    }
    glEnd();
}

void renderLinks(vector<GeoNode*> nodeMesh){
    glBegin(GL_LINES);
    for(int i=0; i<nodeMesh.size(); ++i){
	for(int j=0; j<nodeMesh[i]->links.size(); ++j){
	    glVertex3f(nodeMesh[i]->coords[0],
		       nodeMesh[i]->coords[1],
		       nodeMesh[i]->coords[2]);
	    glVertex3f(nodeMesh[i]->links[j]->coords[0],
		       nodeMesh[i]->links[j]->coords[1],
		       nodeMesh[i]->links[j]->coords[2]);
	    
	}
    }
    glEnd();
}

void renderAxis(){
    double length = 2;
    // red x
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(2,0,0);
    glEnd();

    // green y
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,2,0);
    glEnd();
    
    // blue z
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,2);
    glEnd();

    glColor3f(1,1,1);
}


void changeSize(int w, int h){
    if(h==0)
	h=1;

    double ratio = w*1./h;
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glViewport(0,0,w,h);

    gluPerspective(45.,ratio, 0.1, 100.);

    glMatrixMode(GL_MODELVIEW);
}

void processSpecialKeys(int key, int xx, int yy) {

        double fraction = 0.1;

	switch (key) {

	case GLUT_KEY_LEFT :
	    phi+=0.1;
	    break;
	case GLUT_KEY_RIGHT :
	    phi-=0.1;
	    break;
	case GLUT_KEY_UP :
	    th-=0.1;
	    break;
	case GLUT_KEY_DOWN :
	    th+=0.1;
	    break;
	}
}
void processNormalKeys(unsigned char key, int x, int y) {

    switch (key){

    case 27: // escape key
	exit(0);
	break;
    case 'w':
	r+=0.1;
	break;
    case 's':
	r-=0.1;
	break;
    }
}
