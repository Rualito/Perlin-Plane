#include <cstdio>
#include <vector>

#include <SFML/Graphics.hpp>

#include "FastNoise.h"
#include "ColorManager.hpp"

sf::VertexArray getPerlinMap(int windowSize[2], double t, ColorManager& cm){
    FastNoise fst;
    
    fst.SetFrequency(1.5);
    fst.SetFractalOctaves(3);

    int gridN=256;
    sf::VertexArray quads(sf::Quads, 4*(gridN)*(gridN));

    double rectStep[2]{(1.*windowSize[0])/gridN, (1.*windowSize[1])/gridN}; 

    
    for(int i=0; i<gridN; ++i){
	for(int j=0; j<gridN; ++j){
	    double px0 = i*rectStep[0];
	    double px1 = (i+1)*rectStep[0];
	   
	    double py0 = j*rectStep[1];
	    double py1 = (j+1)*rectStep[1];
	    
	    quads[i*gridN*4+j*4+0].position = sf::Vector2f(px0, py0);
	    quads[i*gridN*4+j*4+1].position = sf::Vector2f(px1, py0);
	    quads[i*gridN*4+j*4+2].position = sf::Vector2f(px1, py1);
	    quads[i*gridN*4+j*4+3].position = sf::Vector2f(px0, py1);

	    float c0 = (fst.GetSimplexFractal((2.*i)/gridN-1, (2.*j)/gridN-1, t)+1)/2;
	    float c1 = (fst.GetSimplexFractal((2.*i+2)/gridN-1, (2.*j)/gridN-1, t)+1)/2;
	    float c2 = (fst.GetSimplexFractal((2.*i+2)/gridN-1, (2.*j+2)/gridN-1, t)+1)/2;
	    float c3 = (fst.GetSimplexFractal((2.*i)/gridN-1, (2.*j+2)/gridN-1, t)+1)/2;
	    
	    Vector3f cId0 = 255.f*cm.roundPath(c0);
	    Vector3f cId1 = 255.f*cm.roundPath(c1);
	    Vector3f cId2 = 255.f*cm.roundPath(c2);
	    Vector3f cId3 = 255.f*cm.roundPath(c3);

	    // printf("x: %f; y: %f; z: %f\n", cId0.x/255, cId0.y/255, cId0.z/255);
	    
	    quads[i*gridN*4+j*4+0].color = sf::Color(cId0.x, cId0.y, cId0.z);
	    quads[i*gridN*4+j*4+1].color = sf::Color(cId1.x, cId1.y, cId1.z);
	    quads[i*gridN*4+j*4+2].color = sf::Color(cId2.x, cId2.y, cId2.z);
	    quads[i*gridN*4+j*4+3].color = sf::Color(cId3.x, cId3.y, cId3.z);

	}
    }

    return quads;
}

int main(){
    
    Vector3f color0(255, 59, 228); color0/=255.f;
    Vector3f color1(0, 193, 51); color1/=255.f;
    Vector3f color2(204, 75, 186); color2/=255.f;
    Vector3f color3(96, 204, 124); color3/=255.f;

    std::vector<Vector3f> palette;
    palette.push_back(color2);
    palette.push_back(color1);
    palette.push_back(color2);
    palette.push_back(color3);
    ColorManager cm(palette);

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Perlin Window");

    int windowSize[2]{1200, 800};

    double t=0;
    sf::VertexArray vex = getPerlinMap(windowSize, t, cm);

    while(window.isOpen()){
	sf::Event event;
	while(window.pollEvent(event)) {
	    if(event.type==sf::Event::Closed) window.close();
	}
	t+=0.01;
	vex=getPerlinMap(windowSize, t, cm);
	
	window.clear();
	window.draw(vex);
	window.display();
    }

    return 0;
}
