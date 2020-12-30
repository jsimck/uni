#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <opencv2/opencv.hpp>
#include "../math/color4.h"

/*!
 * \class Texture
 * \brief Třída popisující texturu.
 *
 * \author Tomáš Fabián
 * \version 0.9
 * \date 2012
*/
class Texture {
public:
    //! Výchozí konstruktor.
    /*!
     * Inicializuje všechny složky na hodnotu nula.
    */
    Texture();

    //! Specializovaný konstruktor.
    /*!
     * Inicializuje texturu ze zadaného obrázku.
     *
     * \param image obrázek načtený pomocí OpenCV.
    */
    Texture(cv::Mat &image);

    //! Destruktor.
    /*!
     * Uvolní všechny alokované zdroje.
    */
    ~Texture();

    //! Vrátí šířku textury v pixelech.
    /*!
     * \return Šířku textury v pixelech.
    */
    int width() const;

    //! Vrátí výšku textury v pixelech.
    /*!
     * \return Výška textury v pixelech.
    */
    int height() const;

    //! Vrátí ukazatel na pole pixelů formátu 8UC4.
    /*!
     * Délka řádku je \a width pixelů a počet řádku je roven \a height pixelů.
     *
     * \return Výška textury v pixelech.
    */
    unsigned char *get_data() const;

    //! Vrátí texel o relativních souřadnicích \a u a \a v, kde \f$(u,v)\in\left<0,1\right>^2\f$.
    /*!
     * Hodnota barvy texelu je vypočtena bilinární interpolací.
     *
     * \return Barva texelu.
    */
    Color4 get_texel(const float u, const float v);

protected:

private:
    int pixel_size_; //*!< Velikost jednoho pixelu v bytech, musí se shodovat s Color4. */
    int row_size_; //*!< Délka jednoho řádku obrazu v bytech. */
    int width_; //*!< Šířka obrazu v pixelech. */
    int height_; //*!< Výška obrazu v pixelech. */

    unsigned char *data_; //*!< Pole pixelů formátu 8UC4. */
};

/*!
 * \fn Texture * LoadTexture( const char * file_name )
 * \brief Načte texturu z obrazového souboru \a file_name.
 * \param file_name úplná cesta k obrazovému souboru včetně přípony.
 * \param flip 0 vertikální nebo 1 horizontální flip obrazu
 * \param single_channel vynutí načtení jednokanálové obrazu.
*/
Texture *LoadTexture(const char *file_name, const int flip = -1, const bool single_channel = false);

#endif