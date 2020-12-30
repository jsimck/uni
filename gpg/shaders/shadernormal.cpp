#include <intersect.h>
#include "shadernormal.h"

Color4 ShaderNormal::getPixel(int x, int y) {
    // Uniform supersampling
    Color4 colorSum(0);
    int size = 3;

    float offsetX = -0.5f;
    float offsetY = -0.5f;
    float offsetAddition = 1.0f / size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Generate rays
            Ray ray = camera.GenerateRay(x - offsetX, y - offsetY);

            // Intersect
            if (this->renderAnalytic) {
                gpg::intersectQuadricPlane(quadric, ray);
            } else {
                rtcIntersect(this->scene, ray);
            }

            // Check if ray collided
            if (!ray.hasCollided()) {
                if (this->cubeMap != NULL) {
                    colorSum += this->cubeMap->getTexel(ray.getDir());
                }

                colorSum += Color4(0);
            } else {
                colorSum += this->calculate(ray);
            }

            // Increase offset
            offsetX += offsetAddition;
        }

        // Reset offset
        offsetX = -0.5f;
        // Increase row offset
        offsetY += offsetAddition;
    }

    return colorSum / SQR(size);
}

Color4 ShaderNormal::calculate(Ray &ray) {
    // Vector from point to cam
    Vector3 t = this->camera.view_from() - ray.getIntersectPoint();
    t.Normalize();

    // Triangle normal
    Vector3 normal = this->getTriangleNormal(ray);
    if (normal.DotProduct(t) < 0) {
        normal = -normal;
    }

    // Calculate normal shader
    return Color4((normal / 2) + Vector3(0.5));
}