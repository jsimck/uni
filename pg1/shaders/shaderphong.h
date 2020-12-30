#ifndef PG1_SHADERPHONG_H
#define PG1_SHADERPHONG_H


#include <vector>
#include "shader.h"
#include "../geom/material.h"
#include "../geom/surface.h"
#include "../geom/omnilight.h"

class ShaderPhong : public Shader {
public:
    ShaderPhong(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials, OmniLight &light, CubeMap *cubeMap = nullptr)
            : Shader(camera, scene, surfaces, materials, cubeMap), light(light) {}

    Color4 getPixel(int x, int y) override;
private:
    OmniLight light;

    Color4 trace(Ray &ray, int depth = 15);

    float switchEnvIor(float ior);
    Color4 getDiffuse(Ray &ray);
    float calcDiffuse(Ray &ray, Vector3 &normal);

    Color4 calcColor(Ray &ray, Material *mat, Vector3 &normal);
    float calcSpecular(Ray &ray, Vector3 &normal, float exp = 3.0f);
    float calcShadow(Ray &ray);
};


#endif //PG1_SHADERPHONG_H
