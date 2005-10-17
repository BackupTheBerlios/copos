/************************************************************************
* Fichier          : imageanalyse.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef IMAGEANALYSE_H
#define IMAGEANALYSE_H

#include <math.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "engine/point2d.h"

/**
  * Class ImageAnalyse
  * Class which allow to analyze an RGB image and to recover a set of point
  * 
  */
typedef struct ImageAnalyse
{
  /**
   * Fields
   */
  /**
   * Minimum of red in the image 
   */
   guchar edgeOfRed;
  /**
   * Minimum of green in the image 
   */
   guchar edgeOfGreen;
  /**
   * Minimum of blue in the image 
   */
   guchar edgeOfBlue;
} ImageAnalyse;


  /**
   * Constructor
   */
  ImageAnalyse*  ImageAnalyse_new ();
    
  
  /**
   * Destructor
   */
  void  ImageAnalyse_destroy (ImageAnalyse *this);
    
  
  /**
   * Takes an RGB ImageAnalyse_image (ImageAnalyse *this, pixbuf) and returns over an array of 2D points 
   * @param *pixbuf The pixbuf to be analyzed
   */
  GSList*  ImageAnalyse_computePixbuf (ImageAnalyse *this, GdkPixbuf *pixbuf);
    
  
  /**
   * Takes an RGB image and returns over an array of 2D points
   * @param *image the image to be analyze
   * @param width the width of the image
   * @param height  the height of the image
   * @param bytesPerPixel the number of bytes of the image
   */
  GSList*  ImageAnalyse_computeImage (ImageAnalyse *this, guchar *image, guint width, guint height , guint bytesPerPixel);
    
  
#endif //IMAGEANALYSE_H
