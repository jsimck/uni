#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/core/persistence.hpp>
#include <math.h>
#include "../utils/macros.h"
#include "../math/vector3.h"
#include "../math/matrix4x4.h"
#include "../geom/ray.h"

/*!
 * \class Camera
 * \brief Základní dírková kamera.
 *
 * \author Tomáš Fabián
 * \version 1.0
 * \date 2014
*/
class Camera {
public:
    // Spacecraft
//    Camera() : Camera(320, 240, Vector3(-140.0f, -175.0f, 110.0f), Vector3(0.0f, 0.0f, 40.0f), DEG2RAD(42.185f)) {}
//    Camera() : Camera(640, 480, Vector3(-140.0f, -175.0f, 110.0f), Vector3(0.0f, 0.0f, 40.0f), DEG2RAD(42.185f)) {}
    Camera() : Camera(1024, 768, Vector3(-140.0f, -175.0f, 110.0f), Vector3(0.0f, 0.0f, 40.0f), DEG2RAD(42.185f)) {}

    // Sphere
//    Camera() : Camera(1024, 768, Vector3(3.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), DEG2RAD(42.185f)) {}
    Camera(const int width, const int height, const Vector3 &view_from,
           const Vector3 &view_at, const float fov_y);

    float aspect_ratio() const;

    float fov_y() const;

    int width() const;

    void set_width(const int width);

    int height() const;

    void set_height(const int width);

    Vector3 view_from() const;

    Vector3 view_at() const;

    // Camera movement
    void ZoomIn(float factor = 0.75f);
    void ZoomOut(float factor = 1.25f);
    void Rotate(float angle);
    void MoveUp(float offset);
    void MoveDown(float offset);
    void ResetPosition();

    void BuildViewMatrix();

    void Update();

    Ray GenerateRay(const float sx, const float sy) const;

    float orthogonal_depth(const Vector3 &p) const;
    Vector3 ws2es(const Vector3 &p) const;
    Vector3 normal_ws2es(const Vector3 &p) const;

    void Save(const char *file_name);
    void Save(cv::FileStorage &fs);

    void Print();

    static const char scanline_hilbert_4x4[];

protected:

private:
    int width_; /*!< Šířka obrazu [px]. */
    int height_; /*!< Výška obrazu [px]. */

    Vector3 view_from_backup_; /*!< Oko [m]. */
    Vector3 view_from_; /*!< Oko [m]. */
    Vector3 view_at_; /*!< Bod, na který se díváme [m]. */

    float fov_y_; /*!< Zorný úhel ve vertikální rovině [rad]. */
    float d_; /*!< Vzdálenost promítací roviny od středu promítání (oka) [m]. */
    float pixel_size_; /*!< Velikost 1 pixelu snímače [m/px]. */

    Matrix4x4 view_t_; /*!< Matice přechodu eye-space do world-space. */
    Matrix4x4 view_; /*!< Matice přechodu z world-space do eye-space. */
    Matrix4x4 view_normal_; /*!< Matice přechodu z world-space do eye-space pro transformaci normál. */

};

#endif