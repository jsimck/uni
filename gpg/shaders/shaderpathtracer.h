#ifndef VSB_PG1_PATHTRACERSHADER_H
#define VSB_PG1_PATHTRACERSHADER_H

#include <vector>
#include <omnilight.h>
#include "shader.h"
#include "../tracing/cubemap.h"

class ShaderPathTracer : public Shader {
public:
    ShaderPathTracer(Camera &camera, RTCScene &scene, std::vector<Surface *> surfaces, std::vector<Material *> Materials, OmniLight light,
                     bool renderAnalytic, CubeMap *cubeMap = nullptr) :
            Shader(camera, scene, surfaces, materials, renderAnalytic, cubeMap), light(light) {}

    virtual Color4 getPixel(int x, int y);
private:
    OmniLight light;

    float albedo();
    float albedoWhite();

    Color4 traceWhite(Ray &ray, int depth = 15);
    Color4 trace(Ray &ray, int depth = 15);

    Color4 getDiffuse(Ray &ray);
    Color4 calcDiffuse(Ray &ray, Vector3 &normal);
    float calcSpecular(Ray &ray, Vector3 &normal, float exp);

    float switchEnvIor(float ior);

    float PDF();
    float PDFMirror();
    float PDFProportional(float cosfi);

    Vector3 genRandomDir(Vector3 &normal);
    Vector3 genRandomDirProportional(Vector3 &normal);
    Vector3 getReflectedDir(Vector3 &normal, Vector3 &dir);
};


#endif //VSB_PG1_PATHTRACERSHADER_H
