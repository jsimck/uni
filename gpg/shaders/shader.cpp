#include "shader.h"

gpg::SphereArea Shader::sphere = gpg::SphereArea(Vector3(0.0f), 1.0f);
//gpg::Quadric Shader::quadric = gpg::Quadric(1.0f, 1.0f, 1.0f, 0, 0, 0, 0, 0, 0, -1.0f); // Sphere
gpg::Quadric Shader::quadric = gpg::Quadric(1.0f, 1.0f, 0.0f, 0, 0, 0, 0, 0, 0, -1.0f); // Cylinder
//gpg::Quadric Shader::quadric = gpg::Quadric(1.0f, 0.5f, 1.0f, 0, 0, 0, 0, 0, 0, -1.0f); // Ellipsoid
//gpg::Quadric Shader::quadric = gpg::Quadric(1.0f, 1.0f, -1.0f, 0, 0, 0, 0, 0, 0, -1.0f); // Paraboloid
//gpg::Quadric Shader::quadric = gpg::Quadric(1.0f, 1.0f, -1.0f, 0, 0, 0, 0, 0, 0, 1.0f); // Two part Paraboloid

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
    // Check if surface exists
    if (surfaces.size() <= ray.geomID) { // doesn't exist, return ray normal
        return ray.getNormal();
    }

    // Surface exists return triangle normal
    Surface *surface = this->surfaces.at(ray.geomID);
    Triangle triangle = surface->get_triangle(ray.primID);

    Vector3 normal = triangle.normal(ray.u, ray.v);
    normal.Normalize();

    return normal;
}

Material *Shader::getSurfaceMaterial(Ray &ray) {
    // Check if surface exists for given material
    if (surfaces.size() > ray.geomID) {
        Surface *surface = this->surfaces.at(ray.geomID);
        return surface->get_material();
    }

    // Return default material
    return defaultMaterial;
}

Shader::~Shader() {
    if (defaultMaterial != nullptr) {
        delete defaultMaterial;
        defaultMaterial = nullptr;
    }
}
