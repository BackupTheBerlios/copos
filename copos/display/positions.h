/************************************************************************
* Fichier          : positions.h
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#ifndef POSITIONS_H
#define POSITIONS_H
#include <gtk/gtk.h>
#include "engine/perspectivecreator.h"
#include "global.h"
#include "generate.h"

/**
  * Class Positions
  * Class allowing to enter the values of positions and the parameters of the webcam
  * 
  */
typedef struct Positions
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * The background picture
   */
   GdkPixbuf *pixbuf;
  /**
   * Widget containing the distance with the webcam 
   */
   GtkWidget *ent_distCam;
  /**
   * Widget containing the distance with the center of rotation
   */
   GtkWidget *ent_distCenter;
  /**
   * Widget containing the focal distance of the webcam 
   */
   GtkWidget *ent_focal;
  /**
   * Widget containing the angle with the webcam 
   */
   GtkWidget *ent_angle;
  /**
   * A pointer to the next page
   */
   Generate *generate;
} Positions;


  /**
   * Constructor
   */
  Positions*  Positions_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  Positions_destroy (Positions *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  Positions_isDisplayed (Positions *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  Positions_presentation (Positions *this);
    
  
#endif //POSITIONS_H
