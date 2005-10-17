/************************************************************************
* Fichier          : global.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "engine/imageanalyse.h"
#include "engine/perspectivecreator.h"
#include "engine/collector.h"
#include "engine/storage.h"



#ifdef HAVE_CONFIG_H
#  include <config.h>
#  define POSITIONS_PIX PACKAGE_DATA_DIR"/positions.png"
#  define ABOUT_PIX PACKAGE_DATA_DIR"/about.png"
#else
#  define POSITIONS_PIX "./positions.png"
#  define ABOUT_PIX "./about.png"
#  define ENABLE_NLS 1
#  define GETTEXT_PACKAGE "copos"
#  define PACKAGE_LOCALE_DIR "/home/bug/projet/copos2/locale"
#endif


#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

/**
  * Class Global
  *  A class containing the data and functions used by the application
  * 
  */
typedef struct Global
{
  /**
   * Fields
   */
  /**
   * The current storage object
   */
   Storage *storage;
  /**
   * The current ImageAnalyse object
   */
   ImageAnalyse *imageAnalyse;
  /**
   * The current PerspectiveCreator object
   */
   PerspectiveCreator *perspectiveCreator;
  /**
   * The current Collector object
   */
   Collector *collector;
  /**
   * The parent where all the pages will be displayed
   */
   GtkContainer *parent;
} Global;


  /**
   * Return the static global object
   */
  Global*  Global_get ();
    
  
  /**
   * Destroy the global object and all of its pointers
   */
  void  Global_destroy ();
    
  
  /**
   * Display an error message to the user
   * @param *message the message to be displayed
   */
  void  Global_errMessage (const gchar *message);
    
  
  /**
   * A function to put a pixel in a pixbuf
   * @param *pixbuf the pixbuf to be modified
   * @param x coordinate x
   * @param y coordinate y
   * @param red the red value of the pixel
   * @param green the green value of the pixel
   * @param blue the blue value of the pixel
   */
  void  Global_putPixel (GdkPixbuf *pixbuf, gint x, gint y, guchar red, guchar green, guchar blue);
    
  
  /**
   * A function to transform an array of guchar to a pixbuf
   * @param *src the source image
   * @param *dst the initialized pixbuf destination
   */
  void  Global_gucharToGdkPixbuf (guchar *src, GdkPixbuf *dst);
    
  /**
   * A function to get the next image of the storage class
   */
  GdkPixbuf*  Global_getPixbufFromStorage ();
  
  /**
   * Store the imageAnalyse
   * @param *imageAnalyse the ImageAnalyse to store
   */
  void  Global_setImageAnalyse (ImageAnalyse *imageAnalyse);
    
  
  /**
   * Store the perspectiveCreator
   * @param *perspectiveCreator the perspectiveCreator to store
   */
  void  Global_setPerspectiveCreator (PerspectiveCreator *perspectiveCreator);
    
  
  /**
   * Store the collector
   * @param *collector the collector to store
   */
  void  Global_setCollector (Collector *collector);
    
  
  /**
   * Store the storage object
   * @param *storage the storage to be stored
   */
  void  Global_setStorage (Storage *storage);
    
  
  /**
   * Add the label to the last page
   * @param *str the label of the last page
   */
  void  Global_setLabel (const gchar *str);
    
  
#endif //GLOBAL_H
