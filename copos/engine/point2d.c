/************************************************************************
* Fichier          : point2d.c
* Date de Creation : mer aoû 11 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mer aoû 11 2004 at 16:29:33 with umbrello
**************************************************************************/

#include "engine/point2d.h"
/**
 * The constructor
 * @param x Co-ordinate X on the image 
 * @param y Co-ordinate y on the image 
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
 * @param *this The object to be destroy
 */
void  Point2D_destroy (Point2D *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

