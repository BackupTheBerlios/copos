/************************************************************************
* Fichier          : collector.c
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#include "engine/collector.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Collector"

/**
 * Constructor
 */
Collector*  Collector_new ()
{
  /* Variables and pre-cond */
  Collector* ret = (Collector*) g_malloc(sizeof(Collector));
  /* Code */
  ret->storage = NULL;
  ret->mat = Matrix_new4x4();
  ret->points = NULL;
  ret->rotCenter = NULL;
  ret->rotAngle = 0.0;
  ret->transDistance = 0.0;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroy
 */
void  Collector_destroy (Collector *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  Matrix_destroy(this->mat);
  if(this->rotCenter != NULL) {
    Point3D_destroy(this->rotCenter);
  }
  g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
  g_slist_free(this->points);
  g_free(this);
}

  

/**
 * Initialize the object to perform the computation (rotation)
 * @param *this The object which computes
 * @param *center The center of the rotation
 * @param rotation The rotation
 * @param *storage The storage object to be analyzed
 */
void  Collector_beginRot (Collector *this, 
			  Point3D *center, 
			  gdouble rotation, 
			  Storage *storage)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(center != NULL);
  g_return_if_fail(rotation > 0);
  g_return_if_fail(storage != NULL);
  /* Code */
  this->storage = storage;
  if(this->points != NULL) {
    g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
    g_slist_free(this->points);
    this->points = NULL;
  }
  Matrix_destroy(this->mat);
  this->mat = Matrix_new4x4();
  this->rotCenter = center;
  this->rotAngle = ((rotation / storage->nbImages) * M_PI) / 180 ;
  this->transDistance = 0.0;
  Storage_readBegin(storage);  
}

  

/**
 * Initialize the object to perform the computation (translation)
 * @param *this The object which computes
 * @param translation The translation
 * @param *storage The storage object to be analyzed
 */
void  Collector_beginTrans (Collector *this, 
			    gdouble translation, 
			    Storage *storage)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(translation > 0.0);
  g_return_if_fail(storage != NULL);
  /* Code */
  this->storage = storage;
  if(this->points != NULL) {
    g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
    g_slist_free(this->points);
    this->points = NULL;
  }
  Matrix_destroy(this->mat);
  this->mat = Matrix_new4x4();
  this->rotCenter = NULL;
  this->rotAngle = 0.0;
  this->transDistance = (translation / storage->nbImages);
  Storage_readBegin(storage); 
}

  

/**
 * One step of the computation
 * @param *this The object which computes
 * @param *imageAnalyse The analyser of image
 * @param *perspectiveCreator The perspective creator
 */
gboolean  Collector_iteration (Collector *this, 
			       ImageAnalyse *imageAnalyse, 
			       PerspectiveCreator *perspectiveCreator)
{
  /* Variables and pre-cond */
  guint width;
  guint height;
  guint bytesPerPixel;
  guchar *tampon;
  gboolean ret;

  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(Storage_isOpen(this->storage), FALSE);
  /* Code */
  Storage_getInfos(this->storage, &width, &height, &bytesPerPixel);
  tampon = (guchar*) g_malloc(sizeof(guchar)*width*height*bytesPerPixel);
  if(Storage_get(this->storage,tampon)) {
    GSList *listPoints2D = NULL;
    GSList *listPoints3D = NULL;
    GSList *iter = NULL;
    listPoints2D = ImageAnalyse_computeImage(imageAnalyse, 
					      tampon, 
					      width,
					      height,
					      bytesPerPixel);

    g_message("list = %d",g_slist_length(listPoints2D));

    listPoints3D = PerspectiveCreator_compute(perspectiveCreator, 
					      listPoints2D);
    g_message("fin list");
    iter =listPoints3D;
    while(iter != NULL) {
      Point3D *pt = (Point3D*) iter->data;
      Point3D *ptNew = Point3D_new(0.0,0.0,0.0);
      Matrix_transform(this->mat, pt, ptNew);
      iter = g_slist_next(iter);
      this->points = g_slist_prepend(this->points, ptNew);
    }
    g_slist_foreach(listPoints3D,(GFunc) Point3D_destroy,NULL);
    g_slist_free(listPoints3D);
    g_slist_foreach(listPoints2D,(GFunc) Point2D_destroy,NULL);
    g_slist_free(listPoints2D);
    
    if(this->rotAngle != 0.0){
      Matrix_translation(this->mat, 
			 this->rotCenter->x,
			 this->rotCenter->y,
			 this->rotCenter->z);	       
      Matrix_rotY(this->mat, this->rotAngle);
      Matrix_translation(this->mat, 
			 -this->rotCenter->x,
			 -this->rotCenter->y,
			 -this->rotCenter->z); 
    }
    else {
      Matrix_translation(this->mat, 
			 this->transDistance,
			 0.0,
			 0.0); 
    }
    ret = TRUE;
  }
  else {
    ret = FALSE;
  }
  g_free(tampon);
  return ret;
}

  

/**
 * Terminate the computation and normalize all the points
 * @param *this The object which compute
 */
void  Collector_end (Collector *this)
{
  /* Variables and pre-cond */
  GSList *iter = NULL;
  GSList *new = NULL;
  gdouble moyX = 0.0;
  gdouble moyY = 0.0;
  gdouble moyZ = 0.0;

  g_return_if_fail(this != NULL);
  /* Code */
  iter = this->points;
  while(iter != NULL) {
    Point3D *pt = (Point3D*) iter->data;
    moyX += pt->x;
    moyY += pt->y;
    moyZ += pt->z;
    iter = g_slist_next(iter);
  }
  g_message("Center all points");
  moyX /= g_slist_length(this->points);
  moyY /= g_slist_length(this->points);
  moyZ /= g_slist_length(this->points);

  iter = this->points;
  while(iter != NULL) {
    Point3D *pt = (Point3D*) iter->data;
    Point3D *ptCenter = Point3D_new((pt->x - moyX), 
				    (pt->y - moyY), 
				    (pt->z - moyZ));
    new = g_slist_prepend(new, ptCenter);
    iter = g_slist_next(iter);
  }
  g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
  g_slist_free(this->points);
  this->points = new;
}

  

/**
 * Save the result in a file
 * @param *this The object to be saved
 * @param *filename The filename where this object will be saved
 */
void  Collector_save (Collector *this, const gchar *filename)
{
  /* Variables and pre-cond */
  FILE *file;
  GSList *iter = NULL;
  g_return_if_fail(this != NULL);
  g_return_if_fail(filename != NULL);
  /* Code */
  file = fopen(filename, "w");
  setlocale(LC_NUMERIC,"C");
  
  iter = this->points;
  while(iter != NULL) {
    Point3D *pt = (Point3D*) iter->data;
    fprintf(file, "Point[%f,%f,%f]\n",pt->x, pt->y, pt->z);
    iter = g_slist_next(iter);
  }
  fclose(file);
}

  

