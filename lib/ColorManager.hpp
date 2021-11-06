#ifndef COLORMANAGER_H

#define COLORMANAGER_H

//#include <SFML/Vector3.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>
#include <vector>
using namespace sf;


class ColorManager{
public:
    ColorManager();
    ColorManager(Vector3f, Vector3f);
    // rgb starting and end points
    ColorManager(const std::vector<Vector3f>&);

    void setColors(const std::vector<Vector3f>&);
    void setColors(Vector3f, Vector3f);

    void setPathMode(int);

    Vector3f path(float t) const;
    // rgb3 vector
    Vector3f linePath(float t) const;
    Vector3f roundPath(float t)const;
    Vector3f hsvPath(float t) const;
    enum ColorMode {LINE, ROUND, HSV};

private:
    std::vector<Vector3f> p0;
    std::vector<Vector3f> round_C;
    std::vector<Vector3f> round_P0;
    ColorMode cmode=ColorMode::ROUND;
    static Vector3f toSpherical(const Vector3f&);
    static Vector3f toCartesian(const Vector3f&);
};

#endif
