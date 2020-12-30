#ifndef PG1_SHADERNORMAL_H
#define PG1_SHADERNORMAL_H


#include "shader.h"

class ShaderNormal : public Shader {
public:
    ShaderNormal(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials, CubeMap *cubeMap = nullptr)
        : Shader(camera, scene, surfaces, materials, cubeMap) {}

    Color4 getPixel(int x, int y) override;
private:
    Color4 calculate(Ray &ray);
};


#endif //PG1_SHADERNORMAL_H
