/************************************************************************
* Fichier          : global.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 11:05:55 with umbrello
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
  if(global == NULL) {
    global = (Global*) g_malloc(sizeof(Global));
    global->modules = NULL;
    global->modulesNames = NULL;
    global->modulesPath = strdup(PACKAGE_DATA_DIR);
    global->webcam = NULL;
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
  Global *this = Global_get();
  if(this->webcam != NULL) {
    (this->webcam)->close(this->webcam);
  }
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
  g_slist_foreach(this->modules,(GFunc) g_module_close,NULL);
  g_slist_free(this->modules);
  g_slist_foreach(this->modulesNames,(GFunc) g_free,NULL);
  g_slist_free(this->modulesNames);
  g_free(this->modulesPath);
  g_object_unref(this->parent);
  g_free(this);
}

  

/**
 * Display an error message to the user
 * @param *message The message to be displayed
 */
void  Global_errMessage (const gchar *message)
{
  GtkWidget *dialog;
  gchar* sUtf8;
  g_return_if_fail(message != NULL);
  g_warning(message);
  sUtf8 = g_locale_to_utf8(message,-1, NULL, NULL, NULL);
  dialog = gtk_message_dialog_new(NULL,
				  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  sUtf8);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  g_free(sUtf8);
}

  

/**
 * A function to put a pixel in a pixbuf
 * @param *pixbuf The pixbuf to be modified
 * @param x Coordinate x
 * @param y Coordinate y
 * @param red The red of the pixel
 * @param green The green of the pixel
 * @param blue The blue of the pixel
 */
void  Global_putPixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue)
{
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

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
}

  

/**
 * A function to transform an array of guchar to a pixbuf
 * @param *src The image source
 * @param *dst The initialized pixbuf destination
 */
void  Global_gucharToGdkPixbuf (guchar *src, GdkPixbuf *dst)
{
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
 * The function to search and open the modules
 */
void  Global_loadModulesDir ()
{
  Global   *this = Global_get();
  GDir    *dir=NULL;
  GModule *module;
  G_CONST_RETURN gchar* file = NULL;

  g_return_if_fail(g_module_supported());
  g_return_if_fail(this != NULL);
  g_return_if_fail(g_file_test(this->modulesPath,G_FILE_TEST_IS_DIR));

  dir = g_dir_open(this->modulesPath, 0, NULL);
  g_return_if_fail(dir != NULL);
  file = g_dir_read_name(dir);
  while(file != NULL)  {
    gchar *path = g_module_build_path(this->modulesPath,file);
    if(! g_file_test(path, G_FILE_TEST_IS_SYMLINK | G_FILE_TEST_IS_DIR)) {
      module = g_module_open(path, G_MODULE_BIND_MASK);
      if(module != NULL) {
	gchar* name = strdup(file);
	g_message("name:%s",name);
	this->modulesNames = g_slist_append(this->modulesNames,name);
	this->modules = g_slist_append(this->modules,module);
      }
      g_free(path);
    }
    file = g_dir_read_name(dir);
  }
  g_dir_close(dir);
}

  

/**
 * Store the webcam
 * @param *webcam The webcam to store
 */
void  Global_setWebcam (Webcam *webcam)
{
  Global *this = Global_get();
  if(this->webcam != NULL) {
    (this->webcam)->close(this->webcam);
  }
  this->webcam = webcam;
}

  

/**
 * Store the imageAnalyse
 * @param imageAnalyse The ImageAnalyse to store
 */
void  Global_setImageAnalyse (ImageAnalyse *imageAnalyse)
{
  Global *this = Global_get();
  if(this->imageAnalyse != NULL) {
    ImageAnalyse_destroy(this->imageAnalyse);
  }
  this->imageAnalyse = imageAnalyse;
}

  

/**
 * Store the perspectiveCreator
 * @param *perspectiveCreator The perspectiveCreator to store
 */
void  Global_setPerspectiveCreator (PerspectiveCreator *perspectiveCreator)
{
  Global *this = Global_get();
  if(this->perspectiveCreator != NULL) {
    PerspectiveCreator_destroy(this->perspectiveCreator);
  }
  this->perspectiveCreator = perspectiveCreator;
}

  

/**
 * Store the collector
 * @param *collector The collector to store
 */
void  Global_setCollector (Collector *collector)
{
  Global *this = Global_get();
  if(this->collector != NULL) {
    Collector_destroy(this->collector);
  }
  this->collector = collector;
}

/**
 * Store the storage object
 * @param *storage The storage to be stored
 */
void  Global_setStorage (Storage *storage)
{
  Global *this = Global_get();
  if(this->storage != NULL) {
    Storage_destroy(this->storage);
  }
  this->storage = storage;
}

/**
 * Add the label to the last page
 * @param *str The label of the last page
 */
void  Global_setLabel (const gchar *str)
{
  GtkWidget *lbl;
  Global   *this = Global_get();
  g_return_if_fail(GTK_IS_NOTEBOOK(this->parent));
  
  lbl = gtk_label_new (str);
  gtk_widget_show (lbl);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (this->parent), gtk_notebook_get_nth_page (GTK_NOTEBOOK (this->parent), -1), lbl);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(this->parent), -1);
}

  

