#ifndef PG1_SHADERINTERFACE_H
#define PG1_SHADERINTERFACE_H


#include "../math/color4.h"
#include "../geom/ray.h"
#include "../geom/surface.h"
#include "../geom/triangle.h"
#include "../tracing/camera.h"
#include "../tracing/cubemap.h"
#include "../tracing/intersect.h"

class Shader {
public:
    Shader(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials,
           bool renderAnalytic, CubeMap *cubeMap)
        : camera(camera), cubeMap(cubeMap), scene(scene), surfaces(surfaces), materials(materials),
          renderAnalytic(renderAnalytic), defaultMaterial(new Material()) {}
    ~Shader();

    void setCamera(Camera &camera);
    Camera *getCamera();
    void setScene(RTCScene &scene);
    RTCScene getScene() const;

    // Gets interpolated normal of triangle we hit
    Vector3 getTriangleNormal(Ray &ray);
    Material *getSurfaceMaterial(Ray &ray);
    virtual Color4 getPixel(int x, int y) = 0;

    static gpg::SphereArea sphere;
    static gpg::Quadric quadric;
protected:
    Camera camera;
    RTCScene scene;
    CubeMap *cubeMap;

    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;

    Material *defaultMaterial;
    bool renderAnalytic;
};


#endif // PG1_SHADERLAMBERT_H