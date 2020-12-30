#ifndef RAY_H_
#define RAY_H_

#include <cmath>
#include <cfloat>
#include <embree2/rtcore_ray.h>
#include "../math/vector3.h"
#include "triangle.h"
#include "../utils/macros.h"
#include "material.h"

/*!
 * \struct Ray
 * \brief Rozšíření paprsku z Embree od další pay-load.
 *
 * \f$\mathbf{r}(t) = O + \hat{\mathbf{d}}t\f$
 *
 * \author Tomáš Fabián
 * \version 0.8
 * \date 2015S
*/
struct Ray : RTCRay {
    float transparency;
    float envIor = IOR_AIR;

    Ray(const Vector3 &origin, Vector3 direction, const float t_near = 0.0f, const float t_far = FLT_MAX) {
        org[0] = origin.x;
        org[1] = origin.y;
        org[2] = origin.z;

        direction.Normalize(); // embree smerovy paprsek nenormalizuje
        dir[0] = direction.x;
        dir[1] = direction.y;
        dir[2] = direction.z;

        tnear = t_near;
        tfar = t_far;

        geomID = RTC_INVALID_GEOMETRY_ID;
        primID = RTC_INVALID_GEOMETRY_ID;
        instID = RTC_INVALID_GEOMETRY_ID;

        mask = 0xFFFFFFFF;
        time = 0.0f;

        // --- payload ---
        transparency = 3.14f;
    }

    Vector3 getIntersectPoint() {
        return this->eval(this->tfar);
    }

    Vector3 getDir() {
        Vector3 dir = this->dir;
        dir.Normalize();

        return dir;
    }

    Vector3 getNormal() {
        Vector3 normal = -Vector3(this->Ng);
        normal.Normalize();

        return normal;
    }

    Vector3 eval(const float t) const {
        return Vector3(
                org[0] + dir[0] * t,
                org[1] + dir[1] * t,
                org[2] + dir[2] * t);
    }

    bool hasCollided() {
        return this->geomID != RTC_INVALID_GEOMETRY_ID;
    }
};


/*!
 * \struct Ray
 *\brief Paprsek.
 *
 *\f$\mathbf{r}(t) = O + \hat{\mathbf{d}}t\f$
 *
 *\author Tomáš Fabián
 *\version 1.0
 *\date 2011-2013
*/
struct RayOld {
    Vector3 origin; /*!< Počátek paprsku. */
    Vector3 direction; /*!< Směrový vektor (jednotkový). */
    Vector3 inv_direction; /*!< Invertovaný směrový vektor. */
    float t; /*!< Reálný parametr \f$tf$. */
    Triangle *target; /*!< Ukazatel na zasažený trojúhelníky. */
    float env_ior; /*!< Index lomu prostředí, kterým se paprsek aktuálně pohybuje. */
    char direction_sign[3]; /*!< Znaménko směrového vektoru. */

    //! Specializovaný konstruktor.
    /*!
     * Inicializuje paprsek podle zadaných hodnot a jdoucí do nekonečna.
     *
     * \param origin počátek.
     * \param direction jednotkový směr.
    */
    RayOld(const Vector3 &origin, const Vector3 &direction, const float bounce = EPSILON,
           const float env_ior = IOR_AIR) {
        this->origin = origin;
        set_direction(direction);

        this->origin += bounce * this->direction;

        t = REAL_MAX;
        target = NULL;

        this->env_ior = env_ior;

#pragma omp atomic
        ++no_rays_;
    }

    //! Vypočte \f$\mathbf{r}(t)\f$.
    //! \return Souřadnice bodu na paprsku pro zadaný parametr \f$t\f$.
    Vector3 eval(const float t) {
        return origin + direction * t;
    }

    //! Vypočte \f$r(t)\f$.
    //! \return True je-li \a t menší než .
    bool closest_hit(const float t, Triangle *const triangle) {

        bool hit_confirmed = false;

        //#pragma omp critical ( make_hit )
        {
            if ((t < this->t) && (t > 0)) {
                this->t = t;
                hit_confirmed = true;
                target = triangle;
            }
        }

        return hit_confirmed;
    }

    bool is_hit() const {
        return ((t > 0) && (t < REAL_MAX) && (target != NULL));
    }

    void set_direction(const Vector3 &direction) {
        this->direction = direction;
        this->direction.Normalize();
        inv_direction = Vector3(1 / this->direction.x, 1 / this->direction.y, 1 / this->direction.z);
        direction_sign[0] = static_cast<char>( inv_direction.x < 0 ); // 0 pro <0,+inf> a 1 pro <-inf,0)
        direction_sign[1] = static_cast<char>( inv_direction.y < 0 );
        direction_sign[2] = static_cast<char>( inv_direction.z < 0 );
    }

    static void no_rays_reset() {
        no_rays_ = 0;
    }

    static long long no_rays() {
        return no_rays_;
    }

private:
    static long long no_rays_;
};

#endif
