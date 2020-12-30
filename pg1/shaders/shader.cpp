#include "shader.h"

void Shader::setCamera(Camera &camera) {
    this->camera = camera;
}

Camera *Shader::getCamera() {
    return &camera;
}

void Shader::setScene(RTCScene &scene) {
    this->scene = scene;
}

RTCScene Shader::getScene() const {
    return scene;
}

Vector3 Shader::getTriangleNormal(Ray &ray) {
    Surface *surface = this->surfaces.at(ray.geomID);
    Triangle triangle = surface->get_triangle(ray.primID);

    Vector3 normal = triangle.normal(ray.u, ray.v);
    normal.Normalize();

    return normal;
}

Material *Shader::getSurfaceMaterial(Ray &ray) {
    Surface *surface = this->surfaces.at(ray.geomID);
    return surface->get_material();
}