#include "intersect.h"

void gpg::intersect(gpg::SphereArea &s, Ray &ray) {
    // Get all used variables
    Vector3 u(ray.dir), A(ray.org);
    u.Normalize();

    // Calc coefficients
    float b = 2 * (u.x * (A.x - s.S.x) + u.y * (A.y - s.S.y) + u.z * (A.z - s.S.z));
    float c = SQR(A.x - s.S.x) + SQR(A.y - s.S.y) + SQR(A.z - s.S.z) - SQR(s.r);

    // a = 1.0 since it's normalized vector
    float D = discriminant(1.0f, b, c);

    if (D < 0) {
        ray.geomID = RTC_INVALID_GEOMETRY_ID;
        return;
    } else {
        float t = 0;

        if (D == 0) {
            t = -b / 2.0f;
        } else {
            float D_s = sqrt(D);
            float t1 = (-b + D_s) / 2.0f;
            float t2 = (-b - D_s) / 2.0f;
            t = MIN(t1, t2);

            if (t < ray.tnear) {
                ray.geomID = RTC_INVALID_GEOMETRY_ID;
                return;
            }
        }

        // Update ray data
        ray.tfar = t;
        ray.geomID = 10000;
        ray.setNormal(s.getNormal(ray.getIntersectPoint()));
    }
}

void gpg::intersectQuadric(gpg::Quadric &q, Ray &ray) {
    // Get all used variables
    Vector3 u(ray.dir), A(ray.org);
    u.Normalize();

    // Calc coefficients
    float a = (q.a11 * SQR(u.x)) + (q.a22 * SQR(u.y)) + (q.a33 * SQR(u.z)) + 2 * ((q.a12 * u.x * u.y) + (q.a13 * u.x * u.z) + (q.a23 * u.y * u.z));
    float b = 2 * ((q.a11 * A.x * u.x) + (q.a22 * A.y * u.y) + (q.a33 * A.z * u.z) + (q.a12 * A.x * u.y) + (q.a12 * A.y * u.x) + (q.a13 * A.x * u.z) + (q.a13 * A.z * u.x)
                   + (q.a23 * A.y * u.z) + (q.a23 * A.z * u.y) + (q.a14 * u.x) + (q.a24 * u.y) + (q.a34 * u.z));
    float c = (q.a11 * SQR(A.x)) + (q.a22 * SQR(A.y)) + (q.a33 * SQR(A.z)) + 2 * ((q.a12 * A.x * A.y) + (q.a13 * A.x * A.z) + (q.a23 * A.y * A.z)
              + (q.a14 * A.x) + (q.a24 * A.y) + (q.a34 * A.z)) + q.a44;

    // Calc discriminant
    float D = discriminant(a, b, c);

    if (D < 0) {
        ray.geomID = RTC_INVALID_GEOMETRY_ID;
        return;
    } else {
        float t = 0;

        if (D == 0) {
            t = -b / (2.0f * a);
        } else {
            float D_s = sqrt(D);
            float t1 = (-b + D_s) / (2.0f * a);
            float t2 = (-b - D_s) / (2.0f * a);
            t = MIN(t1, t2);

            if (t < ray.tnear) {
                ray.geomID = RTC_INVALID_GEOMETRY_ID;
                return;
            }
        }

        // Update ray
        ray.tfar = t;
        ray.geomID = 10000;
        ray.setNormal(q.getNormal(ray.getIntersectPoint()));
    }
}

void gpg::intersectQuadricPlane(gpg::Quadric &q, Ray &ray) {
    // Get all used variables
    Vector3 u(ray.dir), A(ray.org);
    u.Normalize();

    // Calculate quadratic cylinder
    // Calc coefficients
    float a = (q.a11 * SQR(u.x)) + (q.a22 * SQR(u.y)) + (q.a33 * SQR(u.z)) + 2 * ((q.a12 * u.x * u.y) + (q.a13 * u.x * u.z) + (q.a23 * u.y * u.z));
    float b = 2 * ((q.a11 * A.x * u.x) + (q.a22 * A.y * u.y) + (q.a33 * A.z * u.z) + (q.a12 * A.x * u.y) + (q.a12 * A.y * u.x) + (q.a13 * A.x * u.z) + (q.a13 * A.z * u.x)
                   + (q.a23 * A.y * u.z) + (q.a23 * A.z * u.y) + (q.a14 * u.x) + (q.a24 * u.y) + (q.a34 * u.z));
    float c = (q.a11 * SQR(A.x)) + (q.a22 * SQR(A.y)) + (q.a33 * SQR(A.z)) + 2 * ((q.a12 * A.x * A.y) + (q.a13 * A.x * A.z) + (q.a23 * A.y * A.z)
                                                                                  + (q.a14 * A.x) + (q.a24 * A.y) + (q.a34 * A.z)) + q.a44;

    // Calc discriminant
    float D = discriminant(a, b, c);

    if (D < 0) {
        ray.geomID = RTC_INVALID_GEOMETRY_ID;
        return;
    } else {
        // Planes
        Plane planeTop(Vector3(1.0f), Vector3(0.0f, 0.0f, 1.0f));
        Plane planeBottom(Vector3(-1.0f), Vector3(0.0f, 0.0f, -1.0f));
        bool usePlaneNormal = false;

        // Calculate plane intersects
        float r1 = planeTop.eval(A, u);
        float r2 = planeBottom.eval(A, u);

        // Quadratic intersect
        float t = 0;

        if (D == 0) {
            t = -b / (2.0f * a);
        } else {
            float D_s = sqrt(D);
            float t1 = (-b + D_s) / (2.0f * a);
            float t2 = (-b - D_s) / (2.0f * a);

            // Order
            sort(t1, t2);
            sort(r1, r2);

            // Check if all are smaller than tnear
            if (t1 < ray.tnear || t2 < ray.tnear || r1 < ray.tnear || r2 < ray.tnear) {
                ray.geomID = RTC_INVALID_GEOMETRY_ID;
                return;
            }

            // Use T based on sort
            if (r1 < t1 && t1 < r2 && r2 < t2) {
                t = t1;
            } else if (r1 < t1 && t1 < t2 && t2 < r2) {
                t = t1;
            } else if (t1 < r1 && r1 < t2 && t2 < r2) {
                usePlaneNormal = true;
                t = r1;
            } else if (t1 < r1 && r1 < r2 && r2 < t2) {
                usePlaneNormal = true;
                t = r1;
            } else {
                ray.geomID = RTC_INVALID_GEOMETRY_ID;
                return;
            }
        }

        // Update ray
        ray.tfar = t;
        ray.geomID = 10000;

        // Set normal
        Vector3 normal = usePlaneNormal ? planeTop.n : q.getNormal(ray.getIntersectPoint());
        ray.setNormal(normal);
    }
}

float gpg::discriminant(float a, float b, float c) {
    return SQR(b) - (4.0f * a * c);
}

void gpg::sort(float &t1, float &t2) {
    if (t1 > t2) {
        float tmp = t1;
        t1 = t2;
        t2 = tmp;
    }
}
