#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vertex.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include <stdio.h>

class Surface; // dopředné deklarace třídy

/*!
 * \class Triangle
 * \brief Struktura popisující trojúhelník.
 *
 * \author Tomáš Fabián
 * \version 1.1
 * \date 2013-2015
*/
class Triangle {
public:
    //! Výchozí konstruktor.
    /*!
     * Inicializuje všechny složky trojúhelníku na hodnotu nula.
    */
    Triangle() {}

    //! Obecný konstruktor.
    /*!
     * Inicializuje trojúhelník podle zadaných hodnot parametrů.
     *
     * \param v0 první vrchol trojúhelníka.
     * \param v1 druhý vrchol trojúhelníka.
     * \param v2 třetí vrchol trojúhelníka.
     * \param surface ukazatel na plochu, jíž je trojúhelník členem.
    */
    Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Surface *surface = NULL);

    //void Print();

    //! I-tý vrchol trojúhelníka.
    /*!
     * \param i index vrcholu trojúhelníka.
     *
     * \return I-tý vrchol trojúhelníka.
    */
    Vertex vertex(const int i);

    //! Vypočte normálu trojúhelníka v zadaném bodě.
    /*!
     * \param p bod trojúhelníka, pro který se má vypočítat normála interpolací vrcholových normál.
     * \param texture_coord nepovinný ukazatel na texturovací souřadnice v bodě \a p.
     *
     * \return Normála trojúhelníka v zadaném bodě.
    */
    Vector3 normal(const Vector3 &p, Vector2 *texture_coord = NULL);

    //! Vypočte normálu trojúhelníka v zadaném bodě.
    /*!
     * \param u baricentrická souřadnice bodu, pro který má být normála interpolována.
     * \param v baricentrická souřadnice bodu, pro který má být normála interpolována.
     *
     * \return Normála trojúhelníka v zadaném bodě.
    */
    Vector3 normal(const float u, const float v);

    //! Vypočte texturovací souřadnici trojúhelníka v zadaném bodě.
    /*!
     * \param u baricentrická souřadnice bodu, pro který má být normála interpolována.
     * \param v baricentrická souřadnice bodu, pro který má být normála interpolována.
     *
     * \return Texturovací souřadnice v zadaném bodě.
    */
    Vector2 texture_coord(const float u, const float v);

    //! Baricentr trojúhelníka.
    /*!
     * \see http://mathworld.wolfram.com/topics/TriangleCenters.html
     *
     * \return Baricentr trojúhelníka.
    */
    Vector3 baricenter();

    //! Ukazatel na síť, jíž je trojúhelník členem.
    /*!
     * \return Ukazatel na síť.
    */
    Surface *surface();

protected:

private:
    Vertex vertices_[3]; /*!< Vrcholy trojúhelníka. Nic jiného tu nesmí být, jinak padne VBO v OpenGL! */
};

#endif
