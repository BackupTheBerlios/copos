/************************************************************************
* Fichier          : point2d.h
* Date de Creation : mer aoû 11 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mer aoû 11 2004 at 16:29:33 with umbrello
**************************************************************************/

#ifndef POINT2D_H
#define POINT2D_H

#include <stdlib.h>
#include <glib.h>

/**
  * Class Point2D
  * A class managing the 2D point
  * 
  */
typedef struct Point2D
{
  /**
   * Fields
   */
  /**
   * Coordinate X on the image 
   */
   guint x;
  /**
   * Coordinate Y on the image 
   */
   guint y;
} Point2D;


  /**
   * The constructor
   * @param x Co-ordinate X on the image 
   * @param y Co-ordinate y on the image 
   */
  Point2D*  Point2D_new (guint x, guint y);
    
  
  /**
   * Destructor
   * @param *this The object to be destroy
   */
  void  Point2D_destroy (Point2D *this);
    
  
#endif //POINT2D_H
