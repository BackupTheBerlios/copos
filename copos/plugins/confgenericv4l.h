/************************************************************************
* Fichier          : confgenericv4l.h
* Date de Creation : sam aoû 21 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam aoû 21 2004 at 15:13:05 with umbrello
**************************************************************************/

#ifndef CONFGENERICV4L_H
#define CONFGENERICV4L_H

#include <gtk/gtk.h>

#include "webcam.h"
#include "genericv4l.h"

/**
  * Class ConfGenericV4L
  * A generic configuration of webcam via Video 4 Linux
  * 
  */
typedef struct ConfGenericV4L
{
  /**
   * Fields
   */
  /**
   * The entry for the width
   */
   GtkWidget *ent_width;
  /**
   * The entry for the height
   */
   GtkWidget *ent_height;
  /**
   * The selection of the type of palette
   */
   GtkWidget *cmb_palette;
  /**
   * The webcam to be modified
   */
   Webcam *webcam;
} ConfGenericV4L;


  /**
   * Constructor
   * @param *webcam The webcam to be modified
   */
  ConfGenericV4L*  ConfGenericV4L_new (Webcam *webcam);
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  ConfGenericV4L_destroy (ConfGenericV4L *this);
    
  
  /**
   * Display a dialog box for configuration
   * @param *this The object to be displayed
   */
  void  ConfGenericV4L_presentation (ConfGenericV4L *this);
    
  
  /**
   * Construct and display the dialog box of configuration
   * @param *webcam The webcam to be modified
   */
  G_MODULE_EXPORT void  getGtkConf (Webcam *webcam);
    
  
#endif //CONFGENERICV4L_H
