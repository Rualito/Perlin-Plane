#include "ColorManager.hpp"
ColorManager::ColorManager() {
    
}

ColorManager::ColorManager(const std::vector<Vector3f>& pnts) {
    p0 = pnts;
    for(int i=0;  i<pnts.size()-1; ++i){
	round_P0.push_back(toSpherical(pnts[i]));
	round_C.push_back(toSpherical(pnts[i+1])-round_P0[i]);
    }
}

ColorManager::ColorManager(Vector3f c1, Vector3f c2){
    p0.push_back(c1);
    p0.push_back(c2);
}


Vector3f ColorManager::toSpherical(const Vector3f& p) {
    double r = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);

    double phi;
    if(p.x==0)
	phi = M_PI/2;
    else
	phi = atan(p.y/p.x);

    double th;
    if(p.z==0)
        th = M_PI/2;
    else
	th = atan(sqrt(p.x*p.x + p.y*p.y)/p.z);

    return Vector3f(r, th, phi);
}

Vector3f ColorManager::toCartesian(const Vector3f& p) {
    return Vector3f(p.x*sin(p.y)*cos(p.z),
		    p.x*sin(p.y)*sin(p.z),
		    p.x*cos(p.y));
}

void ColorManager::setColors(const std::vector<Vector3f>& pnts){
    round_P0.clear();
    round_C.clear();
    for(int i=0;  i<pnts.size()-1; ++i){
	round_P0.push_back(toSpherical(pnts[i]));
	round_C.push_back(toSpherical(pnts[i+1])-round_P0[i]);
    }
    
}

// this doesnt work properly because of lazyness
// it will work better when HSV is implemented
void ColorManager::setColors(Vector3f c1, Vector3f c2){
    p0.push_back(c1);
    p0.push_back(c2);
}


void ColorManager::setPathMode(int mode){
    cmode = ColorMode(mode);
}

Vector3f ColorManager::linePath(float t) const {
    float segments = p0.size();
    
    for(int i=0; i<segments; ++i){
	if(i/segments>=t)
	    return p0[i-1]+(p0[i]-p0[i-1])*(t-(i-1)/segments);
    }

    // default color
    printf("oh nooo %f\n", t);
    return p0[0];
}
Vector3f ColorManager::hsvPath(float t) const {

    return Vector3f(0,0,0);
}

Vector3f ColorManager::roundPath(float t)const{
    float segments = round_C.size();

    for(int i=0; i<=segments; ++i){
	if(i/segments>=t){
	    return toCartesian(round_C[i-1]*(t-(i-1)/segments)*segments+round_P0[i-1]);
	}
    }	
    // default if something goes wrong
    printf("noooo\n");
    return toCartesian(round_P0[0]);
}

Vector3f ColorManager::path(float t) const{
    switch(cmode){
    case ColorMode::LINE:
	return linePath(t);
	break;
    case ColorMode::ROUND:
	return roundPath(t);
	break;
    case ColorMode::HSV:
	return hsvPath(t);
	break;
    }
}
