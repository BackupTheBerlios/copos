/************************************************************************
* Fichier          : imageanalyse.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "engine/imageanalyse.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ImageAnalyse"

static gint 
analyse_line(ImageAnalyse* this, 
	     guchar *line, 
	     guint width, 
	     guint bytes_per_pixel);

/**
 * Constructor
 */
ImageAnalyse*  ImageAnalyse_new ()
{
  /* Variables and pre-cond */
  ImageAnalyse* ret = (ImageAnalyse*) g_malloc(sizeof(ImageAnalyse));
  /* Code */
  ret->edgeOfRed = 0;
  ret->edgeOfGreen = 0;
  ret->edgeOfBlue = 0;

  return ret;
}

  

/**
 * Destructor
 */
void  ImageAnalyse_destroy (ImageAnalyse *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  g_free(this);
}

  

/**
 * Takes an RGB ImageAnalyse_image (ImageAnalyse *this, pixbuf) and returns over an array of 2D points 
 * @param *pixbuf The pixbuf to be analyzed
 */
GSList*  ImageAnalyse_computePixbuf (ImageAnalyse *this, GdkPixbuf *pixbuf)
{
  /* Variables and pre-cond */
  guint y, width, height, rowstride, bytes_per_pixel;
  guchar *start;
  GSList *ret  = NULL;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(pixbuf != NULL, NULL);
  if((this->edgeOfRed == 0) &&
     (this->edgeOfGreen == 0) &&
     (this->edgeOfBlue == 0)) {
    return NULL;
  }
  /* Code */
  bytes_per_pixel = gdk_pixbuf_get_has_alpha(pixbuf) ? 4 : 3;
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  start = gdk_pixbuf_get_pixels(pixbuf);
  width = gdk_pixbuf_get_width(pixbuf);
  height = gdk_pixbuf_get_height(pixbuf);

  for(y=0;y<height;++y) {
    gint x = analyse_line(this,start+(y*rowstride),width, bytes_per_pixel);
    if(x != -1) {
      Point2D *pt = Point2D_new(x,y);
      ret = g_slist_append(ret,pt);
    } 
  }
  return ret;

}

  

/**
 * Takes an RGB image and returns over an array of 2D points
 * @param *image the image to be analyze
 * @param width the width of the image
 * @param height  the height of the image
 * @param bytesPerPixel the number of bytes of the image
 */
GSList*  ImageAnalyse_computeImage (ImageAnalyse *this, guchar *image, guint width, guint height , guint bytesPerPixel)
{
  /* Variables and pre-cond */
  guint y;
  GSList *ret = NULL;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(image != NULL, NULL);
  g_return_val_if_fail(width > 0, NULL);
  g_return_val_if_fail(height > 0, NULL);
  g_return_val_if_fail(bytesPerPixel > 0, NULL);
  /* Code */
  for(y=0;y<height;++y) {
    gint x = analyse_line(this, 
			  image+(width*bytesPerPixel*y), 
			  width, 
			  bytesPerPixel);
    if(x != -1) {
      Point2D *pt = Point2D_new(x,y);
      ret = g_slist_append(ret,pt);
    }
  }
  return ret;
}

static gint 
analyse_line(ImageAnalyse* this, 
	     guchar *line, 
	     guint width, 
	     guint bytes_per_pixel)
{
  
  guint index=0;
  gint index_previous=-1;
  GSList *list = NULL; /* the list of zone */
  GSList *iter = NULL; /* iterator */
  GArray *zone = NULL; /* array of adjacent pixels */
  GArray *array_max = NULL; /* a pointer to the largest array */
  guint nb_max = 0;
  gint ret = -1;

  /* First find the corresponding zone in the line */
  zone = g_array_new(FALSE, TRUE, sizeof(guint));
  list = g_slist_append(list,zone);
  while(index < width) {
    guchar *pixel = &line[index*bytes_per_pixel];
    if((pixel[0] >= this->edgeOfRed) &&
       (pixel[1] >= this->edgeOfGreen) && 
       (pixel[2] >= this->edgeOfBlue)) {
      /* if there's more than 1 pixel between two good pixel
	 start a new zone */
      if(index_previous != ((gint) index-1)) {
	zone = g_array_new(FALSE, TRUE, sizeof(guint));
	list = g_slist_append(list,zone);
      }
      g_array_append_val(zone,index);
      index_previous = index;
    }
    ++index;
  }
  /* Find the biggest zone */
  iter = list;
  while(iter != NULL) {
    GArray *tmp = (GArray*) iter->data;
    if(tmp->len > nb_max) {
      array_max = tmp;
      nb_max = tmp->len;
    }
    iter = g_slist_next(iter);
  }
  /* If there is a selected table, it makes the average */
  if(nb_max != 0) {
    guint i;
    gdouble moy = 0;
    for(i=0;i<array_max->len;++i) {
      moy += g_array_index(array_max,guint,i);
    }
    moy = moy / array_max->len;
    ret = (gint) moy;
  }

  /* free memories */
  iter = list;
  while(iter != NULL) {
    GArray *tmp = (GArray*) iter->data;
    g_array_free(tmp, TRUE);
    iter = g_slist_next(iter);
  }
  g_slist_free(list);
  
  return ret;
}
