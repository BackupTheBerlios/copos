/************************************************************************
* Fichier          : point3d.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/point3d.h"
/**
 * Constructor
 * @param x Coordinate X
 * @param y Coordinate Y
 * @param z Coordinate Z
 */
Point3D*  Point3D_new (gdouble x, gdouble y, gdouble z)
{
  /* Variables and pre-cond */
  Point3D* ret = (Point3D*) g_malloc(sizeof(Point3D));
  /* Code */
  ret->x = x;
  ret->y = y;
  ret->z = z;

  return ret;
}

  

/**
 * Destructor
 */
void  Point3D_destroy (Point3D *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

