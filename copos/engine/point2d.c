/************************************************************************
* Fichier          : point2d.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/point2d.h"
/**
 * Constructor
 * @param x Co-ordinate X on the image 
 * @param y Co-ordinate Y on the image 
 */
Point2D*  Point2D_new (guint x, guint y)
{
  /* Variables and pre-cond */
  Point2D* ret = (Point2D*) g_malloc(sizeof(Point2D));
  /* Code */
  ret->x = x;
  ret->y = y;
  return ret;
}

  

/**
 * Destructor
 */
void  Point2D_destroy (Point2D *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

