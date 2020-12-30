#include "shaderphong.h"
#include "../utils/utils.h"

Color4 ShaderPhong::getPixel(int x, int y) {
    // Uniform supersampling
    Color4 colorSum(0);
    int size = 3;

    float offsetX = -0.5f;
    float offsetY = -0.5f;
    float offsetAddition = 1.0f / size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Generate ray
            Ray ray = camera.GenerateRay(x + offsetX, y + offsetY);
            colorSum += this->trace(ray);

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

float ShaderPhong::switchEnvIor(float ior) {
    if (ior == IOR_AIR) {
        return IOR_GLASS;
    }

    return IOR_AIR;
}

Color4 ShaderPhong::trace(Ray &ray, int depth) {
    // Get ray dir
    Vector3 dir = ray.getDir();

    // Enable one of 2 render types
    if (this->renderAnalytic) {
        gpg::intersectQuadricPlane(quadric, ray);
    } else {
        rtcIntersect(this->scene, ray);
    }

    // If we didn't hit anything, we'll return color of cube map
    if (!ray.hasCollided()) {
        return this->cubeMap != nullptr ? this->cubeMap->getTexel(dir) : Color4(0);
    }

    // Get triangle normal
    Vector3 normal = getTriangleNormal(ray);

    // Generate new reflected ray and send it
    Vector3 reflected = 2 * (normal.PosDotProduct(-dir)) * normal - (-dir);
    Ray reflectedRay = Ray(ray.getIntersectPoint(), reflected, 0.01f);

    // Get triangle material
    Material *mat = this->getSurfaceMaterial(ray);
    Color4 specular = calcSpecular(ray, normal) * mat->specular;

    // Check if we're not passing limit
    if (depth <= 0) {
        return this->light.ambient + calcColor(ray, normal, mat->reflectivity) * mat->reflectivity * specular;
    }

    // Compute refracted ray
    if (mat->get_name() == "green_plastic_transparent" || mat->get_name() == "wire_214229166" || mat->get_name() == "initialShadingGroup" || mat->get_name() == "default") {
        Vector3 rd = ray.getDir();

        // Set lighter green color
//        Vector3 diffuse = Vector3(0.749019608f, 0.941176471f, 0.2f);
        Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);

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
        reflectedRay = Ray(ray.getIntersectPoint(), rs, 0.01f);

        // Calc cos_01
        float n_d = n1 / n2;
        float sqrt_d = (1 - SQR(n_d) * (1 - SQR(cos_02)));

        // Absolute reflection
        if (sqrt_d < 0.0f) {
            return trace(reflectedRay, depth - 1) * 1.0f * diffuse;
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
        float coefRefract = 1.0f - coefReflect;

        // Generate refracted ray
        Ray refractedRay = Ray(ray.getIntersectPoint(), lr, 0.01f);

        // Set IOR
        refractedRay.envIor = n2;
        reflectedRay.envIor = n1;

        // Check if we're not passing limit
        return trace(reflectedRay, depth - 1) * coefReflect * diffuse +
               trace(refractedRay, depth - 1) * coefRefract * diffuse;
    }

    return this->light.ambient + calcColor(ray, normal, mat->reflectivity) + trace(reflectedRay, depth - 1) * mat->reflectivity * specular;
}

Color4 ShaderPhong::calcColor(Ray &ray, Vector3 &normal, float reflectivity) {
    return (calcDiffuse(ray, normal) * getDiffuse(ray)) * (1.0f - reflectivity);
//    return (calcDiffuse(ray, normal) * getDiffuse(ray) * calcShadow(ray)) * (1.0f - reflectivity);
}

float ShaderPhong::calcDiffuse(Ray &ray, Vector3 &normal) {
    // Normal and vector to light from point
    Vector3 toLight = this->light.position - ray.getIntersectPoint();
    toLight.Normalize();

    // Get dot product between two vectors
    float dot = normal.PosDotProduct(toLight);

    return clamp(dot, 0.0f, 1.0f);
}

float ShaderPhong::calcSpecular(Ray &ray, Vector3 &normal, float exp) {
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

float ShaderPhong::calcShadow(Ray &ray) {
    // Get direction to light and it's vector length
    Vector3 toLight = this->light.position - ray.getIntersectPoint();

    // Send ray to light and check if there's anything in the way
    Ray rayToLight = Ray(ray.getIntersectPoint(), toLight, 0.01f, toLight.L2Norm());
    rtcOccluded(this->scene, rayToLight);

    // Check if we hit something, we did => return 0
    if (rayToLight.hasCollided()) {
        return 0.0f;
    }

    // We didn't hit anything => 1
    return 1.0f;
}

Color4 ShaderPhong::getDiffuse(Ray &ray) {
    // Check if surface exists
    if (surfaces.size() <= ray.geomID) { // doesn't exist, return default material
        return this->defaultMaterial->diffuse;
    }

    // Get triangle
    Surface *surface = this->surfaces.at(ray.geomID);
    Material *material = surface->get_material();
    Triangle triangle = surface->get_triangle(ray.primID);

    // Get texture u and v coordinates
    const Vector2 tuv = triangle.texture_coord(ray.u, ray.v);

    // Get texture using id for diffuse slot
    Texture *diffTexture = material->get_texture(Material::kDiffuseMapSlot);
    Color4 diffColor = material->diffuse;

    // If material has texture return it, otherwise it's material diffuse color
    if (diffTexture != nullptr) {
        diffColor = diffTexture->get_texel(tuv.x, tuv.y);
    }

    return diffColor;
}