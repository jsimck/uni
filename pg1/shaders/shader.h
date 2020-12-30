#ifndef PG1_SHADERINTERFACE_H
#define PG1_SHADERINTERFACE_H


#include "../math/color4.h"
#include "../geom/ray.h"
#include "../geom/surface.h"
#include "../geom/triangle.h"
#include "../tracing/camera.h"
#include "../tracing/cubemap.h"

class Shader {
public:
    Shader(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials, CubeMap *cubeMap = NULL)
        : camera(camera), cubeMap(cubeMap), scene(scene), surfaces(surfaces), materials(materials) {}

    void setCamera(Camera &camera);
    Camera *getCamera();
    void setScene(RTCScene &scene);
    RTCScene getScene() const;

    // Gets interpolated normal of triangle we hit
    Vector3 getTriangleNormal(Ray &ray);
    Material *getSurfaceMaterial(Ray &ray);
    virtual Color4 getPixel(int x, int y) = 0;
protected:
    Camera camera;
    RTCScene scene;
    CubeMap *cubeMap;
    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;
};


#endif // PG1_SHADERLAMBERT_H