/************************************************************************
* Fichier          : collector.h
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
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
  * a class which collects the data of all the others to create the a cloud of 3D points
  * 
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
   * The matrix to compute the position of the next points
   */
   Matrix *mat;
  /**
   * The resulted cloud of points
   */
   GSList *points;
  /**
   * The point of the rotation center
   */
   Point3D *rotCenter;
  /**
   * The angle of the rotation of the object between two image
   */
   gdouble rotAngle;
  /**
   * The translation of the scanned object between two image
   */
   gdouble transDistance;
} Collector;


  /**
   * Constructor
   */
  Collector*  Collector_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroy
   */
  void  Collector_destroy (Collector *this);
    
  
  /**
   * Initialize the object to perform the computation (rotation)
   * @param *this The object which computes
   * @param *center The center of the rotation
   * @param rotation The rotation
   * @param *storage The storage object to be analyzed
   */
  void  Collector_beginRot (Collector *this, Point3D *center, gdouble rotation, Storage *storage);
    
  
  /**
   * Initialize the object to perform the computation (translation)
   * @param *this The object which computes
   * @param translation The translation
   * @param *storage The storage object to be analyzed
   */
  void  Collector_beginTrans (Collector *this, gdouble translation, Storage *storage);
    
  
  /**
   * One step of the computation
   * @param *this The object which computes
   * @param *imageAnalyse The analyser of image
   * @param *perspectiveCreator The perspective creator
   */
  gboolean  Collector_iteration (Collector *this, ImageAnalyse *imageAnalyse, PerspectiveCreator *perspectiveCreator);
    
  
  /**
   * Terminate the computation and normalize all the points
   * @param *this The object which compute
   */
  void  Collector_end (Collector *this);
    
  
  /**
   * Save the result in a file
   * @param *this The object to be saved
   * @param *filename The filename where this object will be saved
   */
  void  Collector_save (Collector *this, const gchar *filename);
    
  
#endif //COLLECTOR_H
