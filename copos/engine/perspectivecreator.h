/************************************************************************
* Fichier          : perspectivecreator.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef PERSPECTIVECREATOR_H
#define PERSPECTIVECREATOR_H
#include <math.h>
#include <glib.h>
#include "engine/point2d.h"
#include "engine/point3d.h"

/**
  * Class PerspectiveCreator
  * A class which allows the projection in 3d with the data of the webcam
  *  and other parameters
  * 
  */
typedef struct PerspectiveCreator
{
  /**
   * Fields
   */
  /**
   * It is the focal distance of the camera/webcam
   */
   guint focal;
  /**
   * The distance between camera and the start of the laser
   */
   gdouble distCamera;
  /**
   * The distance between the start of the laser and the center of object
   */
   gdouble distCenter;
  /**
   * The angle between camera and the start of the laser (see picture)
   */
   gdouble angleCamera;
  /**
   * The width of the CCD
   */
   guint CCDwidth;
  /**
   * The height of the CCD
   */
   guint CCDheight;
  
} PerspectiveCreator;


  /**
   * Constructor
   */
  PerspectiveCreator*  PerspectiveCreator_new ();
    
  
  /**
   * Destructor
   */
  void  PerspectiveCreator_destroy (PerspectiveCreator *this);
    
  
  /**
   * Computation of a 3D profile starting from a set of 2D points
   * @param *points2D the 2D points to be projected in 3d 
   */
  GSList*  PerspectiveCreator_compute (PerspectiveCreator *this, GSList *points2D);
    
  
  /**
   * It creates a 3D point from a 2D point of the PerspectiveCreator_picture (PerspectiveCreator *this, don't forget to free 
   * the new point)
   * @param *pt The point to be transformed
   */
  Point3D*  PerspectiveCreator_pixelToReal (PerspectiveCreator *this, Point2D *pt);
    
  
  /**
   * It creates a 2D point from a 3D PerspectiveCreator_point (PerspectiveCreator *this, don't forget to free the new point)
   * @param *pt the 3D point to be transformed
   */
  Point2D*  PerspectiveCreator_realToPixel (PerspectiveCreator *this, Point3D *pt);
    
  
#endif //PERSPECTIVECREATOR_H
