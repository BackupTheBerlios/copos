/************************************************************************
* Fichier          : choosedevice.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef CHOOSEDEVICE_H
#define CHOOSEDEVICE_H

/* #include "recorder.h" */
#include <gtk/gtk.h>
#include "global.h"
#include "plugins/webcam.h"
#include "choosemodule.h"
#include "recorder.h"
/**
  * Class ChooseDevice
  * Widgets allowing to choose and configure the webcam
  * 
  */
typedef struct ChooseDevice
{
  /**
   * Fields
   */
  /**
   * The object to choose and open a module
   */
   ChooseModule *chooseModule;
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * The button to configure the webcam
   */
   GtkWidget *btn_configuration;
  /**
   * The button to display the next page
   */
   GtkWidget *btn_next;
  /**
   * The area for the display of the webcam
   */
   GtkWidget *dra_chooseDevice;
  /**
   * A buffer to store the next image to be displayed
   */
   guchar *tampon;
  /**
   * handler of the function which update the image of the webcam (for the destruction or the step to the following page) 
   */
   guint tag;
  /**
   * A pointer to the next page
   */
   Recorder *recorder;
} ChooseDevice;


  /**
   * Constructor
   */
  ChooseDevice*  ChooseDevice_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  ChooseDevice_destroy (ChooseDevice *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   * 
   */
  gboolean  ChooseDevice_isDisplayed (ChooseDevice *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  ChooseDevice_presentation (ChooseDevice *this);
    
  
#endif //CHOOSEDEVICE_H
