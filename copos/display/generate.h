/************************************************************************
* Fichier          : generate.h
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#ifndef GENERATE_H
#define GENERATE_H
#include "gtk/gtk.h"
#include "global.h"
#include "engine/collector.h"
#include "visualization.h"

/**
  * Class Generate
  * Class which makes it possible to check the centre of rotation then to generate all the profiles 
  * 
  */
typedef struct Generate
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * The position of the axis of rotation on the picture (in pixel)
   */
   guint rotationCenterPixel;
  /**
   * The position of the axis of rotation in the world (z coordinate)
   */
   gdouble rotationCenterReal;
  /**
   * The widget which displays the current stored image and the axis of rotation
   */
   GtkWidget *img_generate;
  /**
   * The widget to enter the value of the total rotation of the scanned object
   */
   GtkWidget *ent_rotation;
  /**
   * The widget to enter the value of the total translation of the scanned object
   */
   GtkWidget *ent_translation;
  /**
   * The field which throw the signal interruption of computation
   */
   gboolean interrupt;
  /**
   * The next page
   */
   Visualization *visualization;
} Generate;


  /**
   * Constructor
   */
  Generate*  Generate_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  Generate_destroy (Generate *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  Generate_isDisplayed (Generate *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  Generate_presentation (Generate *this);
    
  
#endif //GENERATE_H
