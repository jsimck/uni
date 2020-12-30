#include <vector>
#include <math.h>
#include "shaderpathtracer.h"
#include "../tracing/camera.h"
#include "../tracing/cubemap.h"
#include "shader.h"
#include "../utils/utils.h"

Color4 ShaderPathTracer::getPixel(int x, int y) {
    Color4 color;
    int samples = 100;

    #pragma omp parallel for schedule(dynamic, 5) shared(color)
    for (int i = 0; i < samples; i++) {
        float offsetX = (x + Random()) - 0.5f;
        float offsetY = (y + Random()) - 0.5f;

        Ray ray = this->camera.GenerateRay(offsetX, offsetY);
        color += this->trace(ray);
//        color += this->traceWhite(ray);
    }

    return color / static_cast<float>(samples);
}

Color4 ShaderPathTracer::trace(Ray &ray, int depth) {
    Vector3 dir = ray.getDir();
    rtcIntersect(scene, ray);

    // Check collision
    if (!ray.hasCollided()) {
        return this->cubeMap != NULL ? this->cubeMap->getTexel(dir) : Color4(1.0f);
    }

    // Check normal orientation
    Vector3 normal = this->getTriangleNormal(ray);
    if (-dir.DotProduct(normal) < 0.0f) {
        normal = -normal;
    }

    // Get material
    Material *mat = this->getSurfaceMaterial(ray);

    // Check if we're not passing the limit
    if (depth > 0) {
        // Render transparent material
        if (mat->get_name() == "green_plastic_transparent") {
            Vector3 rd = ray.getDir();

            // Switch IOR
            float n1 = ray.envIor;
            float n2 = switchEnvIor(n1);

            // Calc cos_02
            float cos_02 = (-normal).DotProduct(rd);
            if (cos_02 < 0) {
                normal = -normal;
                cos_02 = (-normal).DotProduct(rd);
            }

            // Vector rs
            Vector3 rs = rd - (2 * normal.DotProduct(rd)) * normal;
            rs.Normalize();

            // Generate reflected ray
            Ray reflectedRay = Ray(ray.getIntersectPoint(), rs, 0.01f);

            // Calc cos_01
            float n_d = n1 / n2;
            float sqrt_d = 1 - SQR(n_d) * (1 - SQR(cos_02));

            // Absolute reflection
            if (sqrt_d < 0.0f) {
                return trace(reflectedRay, depth - 1) * 1.0f * mat->diffuse;
            }

            float cos_01 = sqrt(sqrt_d);

            // Vector rr
            Vector3 rr = -n_d * rd - (n_d * cos_02 + cos_01) * normal;
            rr.Normalize();

            // Vector lr
            Vector3 lr = rr - (2 * normal.DotProduct(rr)) * normal;
            lr.Normalize();
            lr = -lr; // l => lr

            // Fresnel
            float Rs = SQR((n1 * cos_02 - n2 * cos_01) / (n1 * cos_02 + n2 * cos_01));
            float Rp = SQR((n1 * cos_01 - n2 * cos_02) / (n1 * cos_01 + n2 * cos_02));
            float R = 0.5f * (Rs + Rp);

            // Calculate coefficients
            float coefReflect = R;

            if (Random(0.0f, 1.0f) <= coefReflect) {
                // Reflect
                reflectedRay.envIor = ray.envIor;

                return trace(reflectedRay, depth - 1) * mat->diffuse;
            } else {
                // Refract
                Ray refractedRay = Ray(ray.getIntersectPoint(), lr, 0.01f);
                refractedRay.envIor = n2;

                return trace(refractedRay, depth - 1) * mat->diffuse;
            }
        } else if (Random(0.0f, 1.0f) < 0.15f) {
            // Specular
            Vector3 reflectedDir = this->getReflectedDir(normal, dir);
            Ray reflectedRay(ray.getIntersectPoint(), reflectedDir, 0.01f);
            reflectedRay.envIor = ray.envIor;

            return calcSpecular(ray, normal, 3.0f) * this->trace(reflectedRay, depth - 1) * normal.PosDotProduct(reflectedDir); // Mirror ray dir
        } else {
            // Difuse
            Vector3 randVec = this->genRandomDir(normal);
            Ray randRay(ray.getIntersectPoint(), randVec, 0.01f);
            randRay.envIor = ray.envIor;

            return calcDiffuse(ray, normal) * this->trace(randRay, depth - 1) * this->albedo() * 1.0f / this->PDF();
        }
    }

    return Color4(0.0f);
}

float ShaderPathTracer::albedo() {
    return 1.0f / static_cast<float>(M_PI); // albedo
}

Color4 ShaderPathTracer::traceWhite(Ray &ray, int depth) {
    Vector3 dir = ray.getDir();
    rtcIntersect(scene, ray);

    // Check collision
    if (!ray.hasCollided()) {
        return (cubeMap == nullptr) ? Color4(0.9f) : cubeMap->getTexel(dir);
    }

    // Check normal orientation
    Vector3 normal = this->getTriangleNormal(ray);
    if (-dir.DotProduct(normal) < 0.0f) {
        normal = -normal;
    }

    // Get material
    Material *mat = this->getSurfaceMaterial(ray);

    // Check if we're not passing the limit
    if (depth > 0) {
        if (mat->get_name() == "green_plastic_transparent") {
            Vector3 randVec = this->getReflectedDir(normal, dir);
            Ray randRay(ray.getIntersectPoint(), randVec, 0.01f);
            return Vector3(1.0f) * this->traceWhite(randRay, depth - 1) * normal.PosDotProduct(randVec); // Mirror ray dir
        }

        Vector3 randVec = this->genRandomDirProportional(normal);
//        Vector3 randVec = this->genRandomDir(normal);

        Ray randRay(ray.getIntersectPoint(), randVec, 0.01f);
        return Vector3(1.0f) * this->traceWhite(randRay, depth - 1) * normal.PosDotProduct(randVec) * this->albedoWhite() * 1.0f / this->PDFProportional(randVec.DotProduct(normal)); // Uniform ray dir
//        return Vector3(1.0f) * this->traceWhite(randRay, depth - 1) * normal.PosDotProduct(randVec) * this->albedoWhite() * 1.0f / this->PDF(); // Jitted ray dir
    }

    return Color4(0.0f);
}

float ShaderPathTracer::albedoWhite() {
    return 0.5f / static_cast<float>(M_PI); // albedo
}

Vector3 ShaderPathTracer::genRandomDir(Vector3 &normal) {
    // Gen 2 pseudorandom numbers
    float r1 = Random(0.0f, 1.0f);
    float r2 = Random(0.0f, 1.0f);

    // Calculate vector points
    float x = cos(2 * M_PI * r1) * sqrt(1 - SQR(r2));
    float y = sin(2 * M_PI * r1) * sqrt(1 - SQR(r2));
    float z = r2;

    Vector3 dir(x, y, z);
    dir.Normalize();

    // Check if we're in the right hemisphere
    if (normal.DotProduct(dir) < 0.0f) {
        dir = -dir;
    }

    return dir;
}

float ShaderPathTracer::PDF() {
    return 1.0f / (2.0f * static_cast<float>(M_PI));
}

Vector3 ShaderPathTracer::genRandomDirProportional(Vector3 &normal) {
    // Gen 2 pseudorandom numbers
    float r1 = Random(0.0f, 1.0f);
    float r2 = Random(0.0f, 1.0f);

    // Calculate vector points
    float x = cos(2 * M_PI * r1) * sqrt(1 - r2);
    float y = sin(2 * M_PI * r1) * sqrt(1 - r2);
    float z = sqrt(r2);

    Vector3 dir(x, y, z);
    dir.Normalize();

    // Check if we're in the right hemisphere
    if (normal.DotProduct(dir) < 0.0f) {
        dir = -dir;
    }

    return dir;
}

float ShaderPathTracer::PDFProportional(float cosfi) {
    return cosfi / static_cast<float>(M_PI);
}

Vector3 ShaderPathTracer::getReflectedDir(Vector3 &normal, Vector3 &dir) {
    return 2 * (normal.PosDotProduct(-dir)) * normal - (-dir);
}

float ShaderPathTracer::PDFMirror() {
    return 1.0f;
}

Color4 ShaderPathTracer::getDiffuse(Ray &ray) {
    // Get triangle
    Surface *surface = this->surfaces.at(ray.geomID);
    Material *material = surface->get_material();
    Triangle triangle = surface->get_triangle(ray.primID);

    // Get texture u and v coordinates
    const Vector2 tuv = triangle.texture_coord(ray.u, ray.v);

    // Get texture using id for diffuse slot
    Texture *diff_texture = material->get_texture(Material::kDiffuseMapSlot);
    Color4 diff_color = material->diffuse;

    // If material has texture return it, otherwise it's material diffuse color
    if (diff_texture != NULL) {
        diff_color = diff_texture->get_texel(tuv.x, tuv.y);
    }

    return diff_color;
}

Color4 ShaderPathTracer::calcDiffuse(Ray &ray, Vector3 &normal) {
    // Normal and vector to light from point
    Vector3 toLight = this->light.position - ray.getIntersectPoint();
    toLight.Normalize();

    // Get dot product between two vectors
    float dot = normal.PosDotProduct(toLight);

    return clamp(dot, 0.0f, 1.0f) * getDiffuse(ray);
}

float ShaderPathTracer::calcSpecular(Ray &ray, Vector3 &normal, float exp) {
    Vector3 position = ray.getIntersectPoint();

    // Get vector to light
    Vector3 toLight = this->light.position - position;
    toLight.Normalize();

    // Get vector to camera/eye
    Vector3 toCamera = camera.view_from() - ray.getDir();
    toCamera.Normalize();

    // Calculate Half vector
    Vector3 half = toCamera + toLight;
    half.Normalize();

    // Power of result dot product
    return pow(clamp(half.PosDotProduct(normal), 0.0f, 1.0f), exp);
}

float ShaderPathTracer::switchEnvIor(float ior) {
    if (ior == IOR_AIR) {
        return IOR_PLASTIC;
    }

    return IOR_AIR;
}