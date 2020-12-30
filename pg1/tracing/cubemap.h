#ifndef CUBEMAP_H_
#define CUBEMAP_H_


#include "../math/color4.h"
#include "../geom/texture.h"

class CubeMap {
public:
    static enum {
        POSX = 0,
        NEGX = 1,
        POSZ = 2,
        NEGZ = 3,
        POSY = 4,
        NEGY = 5,
    } position;

    CubeMap(const char* basePath);
    ~CubeMap();

    Color4 getTexel(Vector3 direction);
private:
    int getMapIndex(Vector3 direction);
    Texture *maps_[6];
};


#endif