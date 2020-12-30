#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <iostream>
#include "../math/vector3.h"
#include "../geom/texture.h"

/*!
 * \def NO_TEXTURES
 * \brief Maximální počet textur přiřazených materiálu
*/
#define NO_TEXTURES 4

#define IOR_AIR static_cast<float>( 1.000293 )
#define IOR_WATER static_cast<float>( 1.33 )
#define IOR_GLASS static_cast<float>( 1.51714 )
#define IOR_PLASTIC static_cast<float>( 1.460 )

/*!
 * \class Material
 * \brief Difuzní materiál.
 *
 * \author Tomáš Fabián
 * \version 0.9
 * \date 2011-2012
*/
class Material {
public:
    //! Implicitní konstruktor.
    //! Inicializuje všechny složky materiálu na výchozí matný šedý materiýl.
    Material();

    //! Specializovaný konstruktor.
    /*!
     * Inicializuje materiál podle zadaných hodnot parametrů
     * \param name název materiálu.
     * \param ambient barva prostředí.
     * \param diffuse barva rozptylu.
     * \param specular barva odrazu.
     * \param emission  barva emise.
     * \param shininess lesklost.
     * \param ior index lomu.
     * \param textures pole ukazatelů na textury.
     * \param no_textures dálka pole \a textures. Maximální \a NO_TEXTURES - 1.
    */
    Material(std::string &name, const Vector3 &ambient, const Vector3 &diffuse,
             const Vector3 &specular, const Vector3 &emission, const float reflectivity,
             const float shininess, const float ior,
             Texture **textures = NULL, const int no_textures = 0);

    //! Destruktor.
    //! Uvolní všechny alokovaný zdroje.
    ~Material();

    //void Print();

    //! Nastaví název materiálu.
    //! \param name název materiálu.
    void set_name(const char *name);

    //! Vrátí název materiálu.
    //! \return název materiálu.
    std::string get_name() const;

    //! Nastaví texturu.
    /*!
     * \param slot číslo slotu, do kterého bude textura přiřazena. Maximální \a NO_TEXTURES - 1.
     * \param texture ukazatel na texturu.
    */
    void set_texture(const int slot, Texture *texture);

    //! Vrátí texturu.
    /*!
     * \param slot číslo slotu textury. Maximálního \a NO_TEXTURES - 1.
     * \return Ukazatel na zvolenou texturu.
    */
    Texture *get_texture(const int slot) const;

public:
    Vector3 ambient; /*!< RGB barva prost�ed� \f$\left<0, 1\right>^3\f$. */
    Vector3 diffuse; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
    Vector3 specular; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */

    Vector3 emission; /*!< RGB barva emise \f$\left<0, 1\right>^3\f$. */

    float shininess; /*!< Koeficient lesklosti (\f$\ge 0\f$). ��m je hodnota v�t��, t�m se jev� povrch lesklej��. */

    float reflectivity; /*!< Koeficient odrazivosti. */
    float ior; /*!< Index lomu. */

    static const char kDiffuseMapSlot; /*!< číslo slotu difuzn� textury. */
    static const char kSpecularMapSlot; /*!< číslo slotu spekul�rn� textury. */
    static const char kNormalMapSlot; /*!< číslo slotu norm�lov� textury. */
    static const char kOpacityMapSlot; /*!< číslo slotu transparentn� textury. */

private:
    /*!
     * < Pole ukazatel� na RGBA 8UC4 textury.
     * slot 0 - diffuse map + alpha
     * slot 1 - specular map + opaque alpha
     * slot 2 - normal map
     * slot 3 - transparency map
    */
    Texture *textures_[NO_TEXTURES];

    std::string name_; /*!< název plochy. */
};

#endif
