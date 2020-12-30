#include "cubemap.h"

CubeMap::CubeMap(const char* basePath) {
    char buffer[100];
    const char* fileNames[6] = {
        "/posx.jpg",
        "/negx.jpg",
        "/posy.jpg",
        "/negy.jpg",
        "/posz.jpg",
        "/negz.jpg",
    };

    for (int i = 0; i < 6; i++) {
        strcpy(buffer, basePath);
        strcat(buffer, fileNames[i]);
        maps_[i] = LoadTexture(buffer);
    }
}

CubeMap::~CubeMap() {
    for (int i = 0; i < 6; i++) {
        delete maps_[i];
        maps_[i] = NULL;
    }
}

Color4 CubeMap::getTexel(Vector3 direction) {
    float tmp, u, v;
    int map = getMapIndex(direction);

    switch (map) {
        case NEGX:
            tmp = (1.0f / abs(direction.x));
            u = (direction.y * tmp + 1) * 0.5f;
            v = (direction.z * tmp + 1) * 0.5f;
            break;

        case NEGY:
            tmp = (1.0f / abs(direction.y));
            u = 1 - (direction.x * tmp + 1) * 0.5f;
            v = (direction.z * tmp + 1) * 0.5f;
            break;

        case NEGZ:
            tmp = (1.0f / abs(direction.z));
            u = (direction.x * tmp + 1) * 0.5f;
            v = (direction.y * tmp + 1) * 0.5f;
            break;

        case POSX:
            tmp = 1.0f / abs(direction.x);
            u = 1 - (direction.y * tmp + 1) * 0.5f;
            v = (direction.z * tmp + 1) * 0.5f;
            break;

        case POSY:
            tmp = 1.0f / abs(direction.y);
            u = (direction.x * tmp + 1) * 0.5f;
            v = (direction.z * tmp + 1) * 0.5f;
            break;

        case POSZ:
            tmp = 1.0f / abs(direction.z);
            u = (direction.x * tmp + 1) * 0.5f;
            v = 1 - (direction.y * tmp + 1) * 0.5f;
            break;

        default:
            break;
    }

    Color4 texel = maps_[map]->get_texel(u, v);
    return texel;
}

int CubeMap::getMapIndex(Vector3 direction) {
    int lgI = direction.LargestComponent(true);

    if (lgI == 0) {
        return direction.x < 0 ? NEGX : POSX;
    } else if (lgI == 1) {
        return direction.y < 0 ? NEGY : POSY;
    } else {
        return direction.z < 0 ? NEGZ : POSZ;
    }
}
