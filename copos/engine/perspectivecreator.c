/************************************************************************
* Fichier          : perspectivecreator.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/perspectivecreator.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "PerspectiveCreator"

/**
 * Constructor
 */
PerspectiveCreator*  PerspectiveCreator_new ()
{
  /* Variables and pre-cond */
  PerspectiveCreator* ret = (PerspectiveCreator*) g_malloc(sizeof(PerspectiveCreator));
  /* Code */
  ret->focal = 0;
  ret->distCamera = 0.0;
  ret->distCenter = 0.0;
  ret->angleCamera = 0.0;
  ret->CCDwidth = 0;
  ret->CCDheight = 0;

  return ret;
}

  

/**
 * Destructor
 */
void  PerspectiveCreator_destroy (PerspectiveCreator *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

/**
 * Computation of a 3D profile starting from a set of 2D points
 * @param *points2D the 2D points to be projected in 3d 
 */
GSList*  PerspectiveCreator_compute (PerspectiveCreator *this, GSList *points2D)
{
  /* Variables and pre-cond */
  GSList *ret = NULL;
  GSList *iter = NULL;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail((g_slist_length(points2D) != 0), NULL);
  /* Code */
  iter = points2D;
  while(iter != NULL) {
    Point2D *pt2D = (Point2D*) iter->data;
    Point3D *pt3D = PerspectiveCreator_pixelToReal(this, pt2D);
    ret = g_slist_prepend(ret, pt3D);
    iter = g_slist_next(iter);
  }

  return ret;

}


/**
 * It creates a 3D point from a 2D point of the PerspectiveCreator_picture (PerspectiveCreator *this, don't forget to free 
 * the new point)
 * @param *pt The point to be transformed
 */
Point3D*  PerspectiveCreator_pixelToReal (PerspectiveCreator *this, Point2D *pt)
{
  /* Variables and pre-cond */
  gdouble angle = 0.0;
  gdouble alpha = 0.0;
  gdouble longueur = 0.0;
  gdouble Y = 0.0;
  gdouble Z = 0.0;
  Point3D *ret = NULL;
  gdouble w_2 = 0.0;
  gdouble h_2 = 0.0;
  gdouble f = 0.0;
  gdouble x = 0.0;
  gdouble y = 0.0;
  g_return_val_if_fail(pt != NULL, NULL);
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(this->focal > 0, NULL);
  g_return_val_if_fail(this->distCamera > 0, NULL);
  g_return_val_if_fail(this->angleCamera > 0, NULL);
  g_return_val_if_fail(this->CCDwidth > 0, NULL);
  g_return_val_if_fail(this->CCDheight > 0, NULL);
  g_return_val_if_fail(pt->x <= this->CCDwidth, NULL);
  g_return_val_if_fail(pt->y <= this->CCDheight, NULL);
  /* Code */
  w_2 = (gdouble) this->CCDwidth / 2.0;
  h_2 = (gdouble) this->CCDheight / 2.0;
  f = (gdouble) this->focal;
  x = (gdouble) pt->x;
  y = (gdouble) pt->y;
  angle = (this->angleCamera * M_PI) / 180.0;
  /* Compute the depth */
  if(x < w_2) {
    alpha = atan( (w_2 - x) / f );
    Z = tan(angle+alpha) * this->distCamera;
  }
  else {
    alpha = atan( (x - w_2) / f );
    Z = tan(angle - alpha) * this->distCamera;
  }
  /* Compute the height */
  longueur = sqrt( (this->distCamera)*(this->distCamera) + Z*Z );
  Y = ((h_2 - y) * longueur) / f;
  ret = Point3D_new(0.0, Y, Z);

  return ret;
}

/**
 * It creates a 2D point from a 3D PerspectiveCreator_point (PerspectiveCreator *this, 
 * don't forget to free the new point)
 * @param *pt the 3D point to be transformed
 */
Point2D*  PerspectiveCreator_realToPixel (PerspectiveCreator *this, Point3D *pt)
{
  /* Variables and pre-cond */
  gdouble angle = 0.0;
  gdouble longueur = 0.0;
  guint x = 0;
  guint y = 0;
  Point2D *ret = NULL;
  gdouble w_2 = 0.0;
  gdouble h_2 = 0.0;
  gdouble f = 0.0;
  gdouble Y = 0.0;
  gdouble Z = 0.0;
  gdouble alpha = 0.0;
  g_return_val_if_fail(pt != NULL, NULL);
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(this->focal > 0, NULL);
  g_return_val_if_fail(this->distCamera > 0, NULL);
  g_return_val_if_fail(this->angleCamera > 0, NULL);
  g_return_val_if_fail(this->CCDwidth > 0, NULL);
  g_return_val_if_fail(this->CCDheight > 0, NULL);
  /* Code */
  w_2 = (gdouble) this->CCDwidth / 2.0;
  h_2 = (gdouble) this->CCDheight / 2.0;
  f = (gdouble) this->focal;
  Y = (gdouble) pt->y;
  Z = (gdouble) pt->z;
  angle = (this->angleCamera * M_PI) / 180.0;
  
  /* Compute the x coordinate */
  alpha = atan( Z / this->distCamera);
  if(alpha > angle) {
    x = (guint) (w_2 - (tan(alpha - angle) * f));
  }
  else {
    x = (guint) (w_2 + (tan(angle - alpha) * f));
  }
  /* Compute the y coordinate */
  longueur = sqrt( Z*Z + (this->distCamera)*(this->distCamera) );
  y = (guint) (h_2 - (Y * f) / longueur);
  /* Post-conditions */
  g_return_val_if_fail(x <= this->CCDwidth, NULL);
  g_return_val_if_fail(y <= this->CCDheight, NULL);

  ret = Point2D_new(x,y);
  return ret;
}

  

