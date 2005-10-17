/************************************************************************
* Fichier          : global.c
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#include "global.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Global"

/**
 * Singleton of Global
 */
static  Global *global = NULL;


/**
 * Return the static global object
 */
Global*  Global_get ()
{
  /* Variables and pre-cond */
  /* Code */
  if(global == NULL) {
    global = (Global*) g_malloc(sizeof(Global));
    global->storage = NULL;
    global->imageAnalyse = NULL;
    global->perspectiveCreator = NULL;
    global->collector = NULL; 
    global->parent = NULL;
    return global;
  }
  else {
    return global;
  }
}

/**
 * Destroy the global object and all of its pointers
 */
void  Global_destroy ()
{
  /* Variables and pre-cond */
  /* Code */
  Global *this = Global_get();
  
  if(this->storage != NULL) {
    Storage_destroy(this->storage);
  }
  if(this->imageAnalyse != NULL) {
    ImageAnalyse_destroy(this->imageAnalyse);
  }
  if(this->perspectiveCreator != NULL) {
    PerspectiveCreator_destroy(this->perspectiveCreator);
  }
  if(this->collector != NULL) {
    Collector_destroy(this->collector);
  }
  g_object_unref(this->parent);
  g_free(this);
}

  

/**
 * Display an error message to the user
 * @param *message the message to be displayed
 */
void  Global_errMessage (const gchar *message)
{
  /* Variables and pre-cond */
  GtkWidget *dialog;
  /*  gchar* sUtf8;*/
  g_return_if_fail(message != NULL);
  /* Code */
  /*  sUtf8 = g_locale_to_utf8(message,-1, NULL, NULL, NULL); */
  dialog = gtk_message_dialog_new(NULL,
				  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  message);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  /*  g_free(sUtf8);*/
}

  

/**
 * A function to put a pixel in a pixbuf
 * @param *pixbuf the pixbuf to be modified
 * @param x coordinate x
 * @param y coordinate y
 * @param red the red value of the pixel
 * @param green the green value of the pixel
 * @param blue the blue value of the pixel
 */
void  Global_putPixel (GdkPixbuf *pixbuf, gint x, gint y, guchar red, guchar green, guchar blue)
{
  /* Variables and pre-cond */
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (pixbuf));
  g_assert (n_channels == 3);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);
  /* Code */
  
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
}

  

/**
 * A function to transform an array of guchar to a pixbuf
 * @param *src the source image
 * @param *dst the initialized pixbuf destination
 */
void  Global_gucharToGdkPixbuf (guchar *src, GdkPixbuf *dst)
{
  /* Variables and pre-cond */
  guint width, height, n_channels, bytesPerPixel;
  guint x=0;
  guint y=0;
  g_return_if_fail(src != NULL);
  g_return_if_fail(dst != NULL);
  
  n_channels = gdk_pixbuf_get_n_channels (dst);
  
  g_assert (gdk_pixbuf_get_colorspace (dst) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (dst) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (dst));
  g_assert (n_channels == 3);
  /* Code */
  width = gdk_pixbuf_get_width(dst);
  height = gdk_pixbuf_get_height(dst);
  bytesPerPixel = gdk_pixbuf_get_has_alpha(dst) ? 4 :3;
  
  for(y=0;y<height;++y) {
    for(x=0;x<width;++x) {
      guchar red = 0;
      guchar green = 0;
      guchar blue = 0;
      red = src[(y*width*bytesPerPixel)+(x*bytesPerPixel)];
      green = src[(y*width*bytesPerPixel)+(x*bytesPerPixel)+1];
      blue = src[(y*width*bytesPerPixel)+(x*bytesPerPixel)+2];
      Global_putPixel(dst,x,y,red,green,blue);
    }
  }
}
  
/**
   * A function to get the next image of the storage class
   */
GdkPixbuf*  Global_getPixbufFromStorage ()
{
/* Variables and pre-cond */
  guint width;
  guint height;
  guint bytesPerPixel;
  guchar *tampon;
  GdkPixbuf *pixbuf = NULL;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_val_if_fail(storage != NULL, NULL);
  /* Code */
  if(! Storage_isLoaded(storage)) {
    g_return_val_if_fail(Storage_load(storage), NULL);
  }
  Storage_getInfos(storage, &width, &height, &bytesPerPixel,NULL);
  tampon = (guchar*) g_malloc(sizeof(guchar)*width*height*bytesPerPixel);

  if(Storage_get(storage,tampon)) {
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			    FALSE,
			    8,
			    width,
			    height);
    Global_gucharToGdkPixbuf(tampon,pixbuf);
  }
  g_free(tampon);
  return pixbuf;
}

/**
 * Store the imageAnalyse
 * @param *imageAnalyse the ImageAnalyse to store
 */
void  Global_setImageAnalyse (ImageAnalyse *imageAnalyse)
{
  /* Variables and pre-cond */
  /* Code */
  Global *this = Global_get();
  if(this->imageAnalyse != NULL) {
    ImageAnalyse_destroy(this->imageAnalyse);
  }
  this->imageAnalyse = imageAnalyse;
}

  

/**
 * Store the perspectiveCreator
 * @param *perspectiveCreator the perspectiveCreator to store
 */
void  Global_setPerspectiveCreator (PerspectiveCreator *perspectiveCreator)
{
  /* Variables and pre-cond */
  /* Code */
  Global *this = Global_get();
  if(this->perspectiveCreator != NULL) {
    PerspectiveCreator_destroy(this->perspectiveCreator);
  }
  this->perspectiveCreator = perspectiveCreator;
}

  

/**
 * Store the collector
 * @param *collector the collector to store
 */
void  Global_setCollector (Collector *collector)
{
  /* Variables and pre-cond */
  /* Code */
  Global *this = Global_get();
  if(this->collector != NULL) {
    Collector_destroy(this->collector);
  }
  this->collector = collector;
}

  

/**
 * Store the storage object
 * @param *storage the storage to be stored
 */
void  Global_setStorage (Storage *storage)
{
  /* Variables and pre-cond */
  /* Code */
  Global *this = Global_get();
  if(this->storage != NULL) {
    Storage_destroy(this->storage);
  }
  this->storage = storage;
}

  

/**
 * Add the label to the last page
 * @param *str the label of the last page
 */
void  Global_setLabel (const gchar *str)
{
  /* Variables and pre-cond */
  GtkWidget *lbl;
  Global   *this = Global_get();
  g_return_if_fail(GTK_IS_NOTEBOOK(this->parent));
  /* Code */
  lbl = gtk_label_new (str);
  gtk_widget_show (lbl);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (this->parent), gtk_notebook_get_nth_page (GTK_NOTEBOOK (this->parent), -1), lbl);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(this->parent), -1);
}

  

