#ifndef COLOR4_H_
#define COLOR4_H_

#include "vector3.h"

/*!
 * \struct Color4
 * \brief RGBA barva.
 *
 * Implementace čtyřsložkového pixelu.
 *
 * \code{.cpp}
 * Color4 pixel = Color4( 0.5f, 0.25f, 1.0f, 1.0f );
 * \endcode
 *
 * \author Tomáš Fabián
 * \version 0.9
 * \date 2007-2013
*/
struct /*ALIGN*/ Color4 {
public:
    union    // anonymní unie
    {
        struct {
            float r; /*!< Červený kanál. */
            float g; /*!< Zelený kanál. */
            float b; /*!< Modrý kanál. */
            float a; /*!< Alfa kanál. */
        };

        float data[3]; /*!< Pole složek vektoru. */
    };

    //! Výchozí konstruktor.
    /*!
     * Inicializuje složky na černou neprůhlednou barvu.
    */
    Color4() : r(0), g(0), b(0), a(0) {}

    //! Obecný konstruktor.
    /*!
     * Inicializuje složky na hodnotu c.
    */
    Color4(const float c) : r(c), g(c), b(c), a(c) {}

    //! Obecný konstruktor.
    /*!
     * Inicializuje složky na hodnotu Vektoru Color s alpha = 0.
    */
    Color4(const Vector3 color) : r(color.x), g(color.y), b(color.z), a(0) {}

    Color4(const Vector3 color, const float alpha) : r(color.x), g(color.y), b(color.z), a(alpha) {}

    //! Obecný konstruktor.
    /*!
     * Inicializuje barevné složky podle zadaných hodnot parametrů.
     *
     * \param r červený kanál.
     * \param g zelený kanál.
     * \param b modrý kanál.
     * \param a alfa kanál.
    */
    Color4(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) {}

    void Print();
    // --- Operátory ------

    //friend Color4 operator-( const Color4 & v );
    friend Color4 operator+(const Color4 &c1, const Color4 &c2);
    //friend Color4 operator-( const Color4 & u, const Color4 & v );

    friend Color4 operator*(const Color4 &c, const float a);
    friend Color4 operator*(const float a, const Color4 &c);
    friend Color4 operator*(const Vector3 &v, const Color4 &c);
    friend Color4 operator*(const Color4 &u, const Color4 &v);

    friend Color4 operator/( const Color4 & v, const float a );
    friend void operator+=(Color4 &c1, const Color4 &c2);
    //friend void operator-=( Color4 & u, const Color4 & v );
    //friend void operator*=( Color4 & v, const float a );
    friend void operator/=(Color4 &c, const float a);
};

#endif