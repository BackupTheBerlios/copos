/************************************************************************
* Fichier          : global.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 11:05:55 with umbrello
**************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string.h>
#include <glib.h>
#include <gmodule.h>
#include <gtk/gtk.h>
#include "plugins/webcam.h"
#include "engine/storage.h"
#include "engine/imageanalyse.h"
#include "engine/perspectivecreator.h"
#include "engine/collector.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#  define POSITIONS_PIX PACKAGE_DATA_DIR"/copos.png"
#else
#  define PACKAGE_DATA_DIR "plugins"
#  define POSITIONS_PIX "./copos.png"
#  define ENABLE_NLS 1
#  define GETTEXT_PACKAGE "copos"
#  define PACKAGE_LOCALE_DIR "/home/bug/projet/Copos/locale"
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
  * A class containing the data and functions used by the application
  * 
  */
typedef struct Global
{
  /**
   * Fields
   */
  /**
   * The list of the open modules 
   */
   GSList *modules;
  /**
   * The list of the names of open modules 
   */
   GSList *modulesNames;
  /**
   * The path to search the modules
   */
   gchar *modulesPath;
  /**
   * The current webcam of the software
   */
   Webcam *webcam;
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
   * @param *message The message to be displayed
   */
  void  Global_errMessage (const gchar *message);
    
  
  /**
   * A function to put a pixel in a pixbuf
   * @param *pixbuf The pixbuf to be modified
   * @param x Coordinate x
   * @param y Coordinate y
   * @param red The red of the pixel
   * @param green The green of the pixel
   * @param blue The blue of the pixel
   */
  void  Global_putPixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue);
    
  
  /**
   * A function to transform an array of guchar to a pixbuf
   * @param *src The image source
   * @param *dst The initialized pixbuf destination
   */
  void  Global_gucharToGdkPixbuf (guchar *src, GdkPixbuf *dst);
    
  
  /**
   * The function to search and open the modules
   */
  void  Global_loadModulesDir ();
    
  
  /**
   * Store the webcam
   * @param *webcam The webcam to store
   */
  void  Global_setWebcam (Webcam *webcam);
    
  
  /**
   * Store the imageAnalyse
   * @param imageAnalyse The ImageAnalyse to store
   */
  void  Global_setImageAnalyse (ImageAnalyse *imageAnalyse);
    
  
  /**
   * Store the perspectiveCreator
   * @param *perspectiveCreator The perspectiveCreator to store
   */
  void  Global_setPerspectiveCreator (PerspectiveCreator *perspectiveCreator);
    
  
  /**
   * Store the collector
   * @param *collector The collector to store
   */
  void  Global_setCollector (Collector *collector);
    
  /**
   * Store the storage object
   * @param *storage The storage to be stored
   */
  void  Global_setStorage (Storage *storage);

  /**
   * Add the label to the last page
   * @param *str The label of the last page
   */
  void  Global_setLabel (const gchar *str);
    
  
#endif //GLOBAL_H
