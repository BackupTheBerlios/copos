/************************************************************************
* Fichier          : point2d.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
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
   * Constructor
   * @param x Co-ordinate X on the image 
   * @param y Co-ordinate Y on the image 
   */
  Point2D*  Point2D_new (guint x, guint y);
    
  
  /**
   * Destructor
   */
  void  Point2D_destroy (Point2D *this);
    
  
#endif //POINT2D_H
