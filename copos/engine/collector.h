/************************************************************************
* Fichier          : collector.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef COLLECTOR_H
#define COLLECTOR_H
#include <glib.h>
#include <locale.h>
#include "engine/storage.h"
#include "engine/imageanalyse.h"
#include "engine/perspectivecreator.h"
#include "engine/matrix.h"
/**
  * Class Collector
  * A class which collects the data of all the others to create the cloud of 3D points
  * 
  */
typedef struct Collector
{
  /**
   * Fields
   */
  /**
   * The set of images which will be analyze
   */
   Storage *storage;
  /**
   * The object to analyse each image
   */
   ImageAnalyse *imageAnalyse;
  /**
   * The object to transform a 2D point to 3D point
   */
   PerspectiveCreator *perspectiveCreator;
  /**
   *  The computed cloud of points
   */
   GSList *points;
  /**
   * each lines of the object
   */
   GSList *lines;
} Collector;


  /**
   * Constructor
   */
  Collector*  Collector_new ();
    
  
  /**
   * Destructor
   */
  void  Collector_destroy (Collector *this);
    
  
  /**
   * Initialize the object to perform the Collector computation
   * @param *imageAnalyse the analyser of image
   * @param *perspectiveCreator the perspective creator
   * @param *storage The storage of images
   */
  void  Collector_begin (Collector *this, 
			 ImageAnalyse *imageAnalyse, 
			 PerspectiveCreator *perspectiveCreator,
			 Storage *storage);
  /**
   * One step of the computation
   */
  gboolean  Collector_iteration (Collector *this);
    
  
  /**
   * Terminate the computation and normalize all the points (Rotation mode)
   */
  void  Collector_endRotation (Collector *this, const Point3D *center, gdouble rotation);
    
  /**
   * Terminate the computation and normalize all the points (Translation mode)
   */
  void  Collector_endTranslation (Collector *this, gdouble translation);
  /**
   * Save the result in a file
   * @param *filename the filename where this object will be saved
   */
  void  Collector_save (Collector *this, const gchar *filename);
    
  
#endif //COLLECTOR_H
