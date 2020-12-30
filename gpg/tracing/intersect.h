#ifndef VSB_GPG_INTERSECT_H
#define VSB_GPG_INTERSECT_H

#include "../geom/ray.h"

namespace gpg {
    struct SphereArea {
        Vector3 S;
        float r;

        SphereArea(Vector3 S, float r) : S(S), r(r) {}

        Vector3 getNormal(Vector3 intersectPoint) {
            return S - intersectPoint;
        }
    };

    struct Quadric {
        union {
            struct {
                float a11;
                float a22;
                float a33;

                float a12;
                float a13;
                float a23;

                float a14;
                float a24;
                float a34;
                float a44;
            };

            float data[10];
        };

        Quadric(float a11, float a22, float a33, float a12, float a13, float a23, float a14, float a24, float a34, float a44) :
                a11(a11), a22(a22), a33(a33), a12(a12), a13(a13), a23(a23), a14(a14), a24(a24), a34(a34), a44(a44) {}

        Vector3 getNormal(Vector3 intersectPoint) {
            return Vector3 (
                2 * (a11 * intersectPoint.x + a12 * intersectPoint.y + a13 * intersectPoint.z + a14),
                2 * (a22 * intersectPoint.y + a12 * intersectPoint.x + a23 * intersectPoint.z + a24),
                2 * (a33 * intersectPoint.z + a13 * intersectPoint.x + a23 * intersectPoint.y + a34)
            );
        }
    };

    struct Plane {
        Vector3 S;
        Vector3 n;

        Plane(Vector3 S, Vector3 n) : S(S), n(n) {}

        float eval(Vector3 &A, Vector3 &u) {
            float dNU = n.DotProduct(u);
            if (dNU != 0) {
                return n.DotProduct(S - A) / dNU;
            }

            return 0;
        }
    };

    void intersect(SphereArea &s, Ray &ray);
    void intersectQuadric(Quadric &q, Ray &ray);
    void intersectQuadricPlane(Quadric &q, Ray &ray);

    float discriminant(float a, float b, float c);
    void sort(float &t1, float &t2);
}


#endif //VSB_GPG_INTERSECT_H
