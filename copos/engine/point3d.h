/************************************************************************
* Fichier          : point3d.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef POINT3D_H
#define POINT3D_H
#include <glib.h>

/**
  * Class Point3D
  * A class managing the 3D point
  * 
  */
typedef struct Point3D
{
  /**
   * Fields
   */
  /**
   * Coordinate X
   */
   gdouble x;
  /**
   * Coordinate Y
   */
   gdouble y;
  /**
   * Coordinate Z
   */
   gdouble z;
} Point3D;


  /**
   * Constructor
   * @param x Coordinate X
   * @param y Coordinate Y
   * @param z Coordinate Z
   */
  Point3D*  Point3D_new (gdouble x, gdouble y, gdouble z);
    
  
  /**
   * Destructor
   */
  void  Point3D_destroy (Point3D *this);
    
  
#endif //POINT3D_H
