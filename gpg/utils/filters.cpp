#include "filters.h"
#include "../geom/surface.h"

void filter_intersection(void *user_ptr, Ray &ray) {
    /*  All hit information inside the ray is valid.
        The filter function can reject a hit by setting the geomID member of the ray to
        RTC_INVALID_GEOMETRY_ID, otherwise the hit is accepted.The filter function is not
        allowed to modify the ray input data (org, dir, tnear, tfar), but can modify
        the hit data of the ray( u, v, Ng, geomID, primID ). */

    Surface *surface = reinterpret_cast<Surface *>( user_ptr );
    printf("intersection of: %s, ", surface->get_name().c_str());
    const Vector3 p = ray.eval(ray.tfar);
    printf("at: %0.3f (%0.3f, %0.3f, %0.3f)\n", ray.tfar, p.x, p.y, p.z);

    ray.geomID = RTC_INVALID_GEOMETRY_ID; // reject hit
}

void filter_occlusion(void *user_ptr, Ray &ray) {
    /*  All hit information inside the ray is valid.
    The filter function can reject a hit by setting the geomID member of the ray to
    RTC_INVALID_GEOMETRY_ID, otherwise the hit is accepted.The filter function is not
    allowed to modify the ray input data (org, dir, tnear, tfar), but can modify
    the hit data of the ray( u, v, Ng, geomID, primID ). */

    Surface *surface = reinterpret_cast<Surface *>( user_ptr );
    printf("occlusion of: %s, ", surface->get_name().c_str());
    const Vector3 p = ray.eval(ray.tfar);
    printf("at: %0.3f (%0.3f, %0.3f, %0.3f)\n", ray.tfar, p.x, p.y, p.z);

    ray.geomID = RTC_INVALID_GEOMETRY_ID; // reject hit
}