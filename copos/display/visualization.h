/************************************************************************
* Fichier          : visualization.h
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "trackball.h"
#include "engine/collector.h"
#include "global.h"

/**
  * Class Visualization
  * Class allowing the visulisation of the result 
  * 
  */
typedef struct Visualization
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
} Visualization;


  /**
   * Constructor
   */
  Visualization*  Visualization_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  Visualization_destroy (Visualization *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this the object to be tested
   */
  gboolean  Visualization_isDisplayed (Visualization *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  Visualization_presentation (Visualization *this);
    
  
#endif //VISUALIZATION_H
