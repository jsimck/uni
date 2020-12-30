#ifndef SURFACE_H_
#define SURFACE_H_

#include "triangle.h"
#include "material.h"
#include "../math/matrix4x4.h"
#include "vertex.h"
#include <vector>

/*!
 * \class Surface
 * \brief Struktura popisující trojúhelníkovou síť.
 *
 * \author Tomáš Fabián
 * \version 0.9
 * \date 2012
*/
class Surface {
public:
    //! Výchozí konstruktor.
    /*!
     * Inicializuje všechny složky sítě na hodnotu nula.
    */
    Surface();

    //! Obecný konstruktor.
    /*!
     * Inicializuje vertex podle zadaných hodnot parametrů.
     *
     * \param name název plochy.
     * \param n počet trojúhelníků tvořících síť.
    */
    Surface(const std::string &name, const int n);

    //! Destruktor.
    /*!
     * Uvolní všechny alokované zdroje.
    */
    ~Surface();

    //! Vrátí požadovaný trojúhelník.
    /*!
     * \param i index trojúhelníka.
     * \return Trojúhelník.
    */
    Triangle &get_triangle(const int i);

    //! Vrátí pole všech trojúhelníků.
    /*!
     * \return Pole všech trojúhelníků.
    */
    Triangle *get_triangles();

    //! Vrátí název plochy.
    /*!
     * \return Název plochy.
    */
    std::string get_name();

    //! Vrátí počet všech trojúhelníků v síti.
    /*!
     * \return Počet všech trojúhelníků v síti.
    */
    int no_triangles();

    //! Vrátí počet všech vrcholů v síti.
    /*!
     * \return Počet všech vrcholů v síti.
    */
    int no_vertices();

    //! Vrátí ukazatel na matici transformace z lokálního do světového souřadného systému.
    /*!
     * \return Ukazatel na matici transformace z LS do WS.
    */
    Matrix4x4 *transformation();

    //! Nastaví materiál plochy.
    /*!
     * \param material ukazatel na materiál.
    */
    void set_material(Material *material);

    //! Vrátí ukazatel na materiál plochy.
    /*!
     * \return Ukazatel na materiál plochy.
    */
    Material *get_material() const;

protected:

private:
    int n_; /*!< Počet trojúhelníků v síti. */
    Triangle *triangles_; /*!< Trojúhelníková síť. */

    std::string name_; /*!< Název plochy. */

    Matrix4x4 transformation_; /*!< Transformační matice pro přechod z modelového do světového souřadného systému. */
    Material *material_; /*!< Materiál plochy. */
};

/*!
 * \fn Surface * BuildSurface( const std::string & name, std::vector<Vertex> & face_vertices )
 * \brief Sestavení plochy z pole trojic vrcholů.
 * \param name název plochy.
 * \param face_vertices pole trojic vrcholů.
*/
Surface *BuildSurface(const std::string &name, std::vector<Vertex> &face_vertices);

#endif
