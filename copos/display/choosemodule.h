/************************************************************************
* Fichier          : choosemodule.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef CHOOSEMODULE_H
#define CHOOSEMODULE_H

#include <gmodule.h>
#include <gtk/gtk.h>
#include "plugins/webcam.h"
#include "global.h"

/**
  * Class ChooseModule
  * A class to choose and open a module
  * 
  */
typedef struct ChooseModule
{
  /**
   * Fields
   */
  /**
   * Fields containing the file/device
   */
   GtkWidget *ent_device;
  /**
   * The number of the selected module (in the list of Global)
   */
   gint activeModule;
} ChooseModule;


  /**
   * Constructor
   */
  ChooseModule*  ChooseModule_new ();
    
  
  /**
   * Destructor
   * @param *this the object to be destroy
   */
  void  ChooseModule_destroy (ChooseModule *this);
    
  
  /**
   * Open a dialog box then return the selected webcam or NULL
   * @param *this The object which return the webcam
   */
  gboolean  ChooseModule_getWebcam (ChooseModule *this);
    
  
  /**
   * Open a dialog box to configure the current webcam (Global)
   * @param *this The object which displays the configuration box
   */
  void  ChooseModule_configureWebcam (ChooseModule *this);
    
  
#endif //CHOOSEMODULE_H
