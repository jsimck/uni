#ifndef PG1_SHADERLAMBERT_H
#define PG1_SHADERLAMBERT_H


#include "shader.h"
#include "../tracing/camera.h"
#include "../tracing/cubemap.h"
#include "../tracing/intersect.h"

class ShaderLambert : public Shader {
public:
    ShaderLambert(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials,
                      bool renderAnalytic, CubeMap *cubeMap)
            : Shader(camera, scene, surfaces, materials, renderAnalytic, cubeMap) {}

    Color4 getPixel(int x, int y) override;
private:
    Color4 calculate(Ray &ray);
};


#endif //PG1_SHADERLAMBERT_H