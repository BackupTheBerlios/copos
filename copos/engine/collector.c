/************************************************************************
* Fichier          : collector.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
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
  ret->imageAnalyse = NULL;
  ret->perspectiveCreator = NULL;
  ret->points = NULL;
  ret->lines = NULL;
  return ret;
}

  

/**
 * Destructor
 */
void  Collector_destroy (Collector *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
  g_slist_free(this->points);
  if(this->lines != NULL) {
    GSList *iter = this->lines;
    while(iter != NULL) {
      GSList *line = (GSList*) iter->data;
      g_slist_foreach(line,(GFunc) Point3D_destroy,NULL);
      g_slist_free(line);
      iter = g_slist_next(iter);
    }
  }
  g_free(this);
}

  

/**
 * Initialize the object to perform the Collector computation
 * @param *imageAnalyse the analyser of image
 * @param *perspectiveCreator the perspective creator
 * @param *storage The storage of images
 */
void  Collector_begin (Collector *this, 
		       ImageAnalyse *imageAnalyse, 
		       PerspectiveCreator *perspectiveCreator,
		       Storage *storage)
{
  /* Variables and pre-cond */  
  g_return_if_fail(this != NULL);
  g_return_if_fail(imageAnalyse != NULL);
  g_return_if_fail(perspectiveCreator != NULL);
  g_return_if_fail(storage != NULL);
  /* Code */
  this->imageAnalyse = imageAnalyse;
  this->perspectiveCreator = perspectiveCreator;
  this->storage = storage;
  if(! Storage_isLoaded(storage)) {
    g_return_if_fail(Storage_load(storage));
  }
  Storage_seek(storage,0);
  
  if(this->points != NULL) {
    g_slist_foreach(this->points,(GFunc) Point3D_destroy,NULL);
    g_slist_free(this->points);
    this->points = NULL;
  }
  if(this->lines != NULL) {
    GSList *iter = this->lines;
    while(iter != NULL) {
      GSList *line = (GSList*) iter->data;
      g_slist_foreach(line,(GFunc) Point3D_destroy,NULL);
      g_slist_free(line);
      iter = g_slist_next(iter);
    }
  }
}

/**
 * One step of the computation
 * @param *imageAnalyse the analyser of image
 * @param *perspectiveCreator the perspective creator
 */
gboolean  Collector_iteration (Collector *this)
{
  /* Variables and pre-cond */
  guint width;
  guint height;
  guint bytesPerPixel;
  guchar *tampon;
  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(Storage_isLoaded(this->storage), FALSE);
  /* Code */
  Storage_getInfos(this->storage, &width, &height, &bytesPerPixel, NULL);
  tampon = (guchar*) g_malloc(sizeof(guchar)*width*height*bytesPerPixel);
  if(Storage_get(this->storage,tampon)) {
    GSList *listPoints2D = NULL;
    listPoints2D = ImageAnalyse_computeImage(this->imageAnalyse, 
					     tampon, 
					     width,
					     height,
					     bytesPerPixel);
    this->lines = g_slist_prepend(this->lines,
				  PerspectiveCreator_compute(this->perspectiveCreator, listPoints2D));

    g_slist_foreach(listPoints2D,(GFunc) Point2D_destroy,NULL);
    g_slist_free(listPoints2D);
    g_free(tampon);
    return TRUE;
  }
  else {
    g_free(tampon);
    return FALSE;
  }
}

  
void terminate(Collector *this, 
	       const Point3D *center, 
	       gdouble rotation, 
	       gdouble translation,
	       gboolean isRotation)
{
  /* Variables and pre-cond */
  GSList *iterLines = NULL;
  GSList *iterPoints = NULL;
  GSList *new = NULL;
  Matrix *mat;
  gdouble rot = 0.0;
  gdouble trans = 0.0;
  gdouble moyX = 0.0;
  gdouble moyY = 0.0;
  gdouble moyZ = 0.0;
  g_return_if_fail(this != NULL);  
  g_return_if_fail(this->lines != NULL);
  g_return_if_fail(this->points == NULL);
  
  /* Code */
  mat = Matrix_new4x4();
  if(isRotation) {
    g_return_if_fail(center != NULL);
    g_return_if_fail(rotation > 0);
    rot = ((rotation / g_slist_length(this->lines)) * M_PI) / 180 ;
  }
  else {
    g_return_if_fail(translation > 0);
    trans = (translation / g_slist_length(this->lines));
  }
  iterLines = this->lines;
  while(iterLines != NULL) {
    GSList *line = (GSList*) iterLines->data;
    iterPoints = line;
    while(iterPoints != NULL) {
      Point3D *pt = (Point3D*) iterPoints->data;
      Point3D *ptNew = Point3D_new(0.0,0.0,0.0);
      Matrix_transform(mat, pt, ptNew);
      moyX += ptNew->x;
      moyY += ptNew->y;
      moyZ += ptNew->z;
      this->points = g_slist_prepend(this->points, ptNew);
      iterPoints = g_slist_next(iterPoints);
    }
    if(isRotation) {
      Matrix_translation(mat, 
			 center->x,
			 center->y,
			 center->z);	       
      Matrix_rotY(mat, rot);
      Matrix_translation(mat, 
			 -center->x,
			 -center->y,
			 -center->z);
    }
    else {
      Matrix_translation(mat, 
			 trans,
			 0.0,
			 0.0); 
    }
    iterLines = g_slist_next(iterLines);
  }

  moyX /= g_slist_length(this->points);
  moyY /= g_slist_length(this->points);
  moyZ /= g_slist_length(this->points);

  iterPoints = this->points;
  while(iterPoints != NULL) {
    Point3D *pt = (Point3D*) iterPoints->data;
    Point3D *ptCenter = Point3D_new((pt->x - moyX),
				    (pt->y - moyY),
				    (pt->z - moyZ));
    new = g_slist_prepend(new, ptCenter);
    iterPoints = g_slist_next(iterPoints);
  }
  g_slist_foreach(this->points,(GFunc) Point3D_destroy, NULL);
  g_slist_free(this->points);
  this->points = new;
  Matrix_destroy(mat);
}

/**
 * Terminate the computation and normalize all the points (Rotation mode)
 */
void  Collector_endRotation (Collector *this, const Point3D *center, gdouble rotation)
{
  terminate(this, center, rotation, 0.0, TRUE);
}

/**
 * Terminate the computation and normalize all the points (Translation mode)
 */
void  Collector_endTranslation (Collector *this, gdouble translation)
{
  terminate(this, NULL, 0.0, translation, FALSE);
}
 
  

/**
 * Save the result in a file
 * @param *filename the filename where this object will be saved
 */
void  Collector_save (Collector *this, const gchar *filename)
{
  /* Variables and pre-cond */
  FILE *file;
  GSList *iter = NULL;
  g_return_if_fail(this != NULL);
  g_return_if_fail(filename != NULL);
  g_return_if_fail(this->points != NULL);
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

  

