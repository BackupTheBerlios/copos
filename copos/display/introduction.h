/************************************************************************
* Fichier          : introduction.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#include <gtk/gtk.h>
#include "global.h"
#include "choosedevice.h"

/**
  * Class Introduction
  * Class presenting the various stages of the scannerisation 
  * 
  */
typedef struct Introduction
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * A pointer to the next page
   */
  ChooseDevice *chooseDevice;
} Introduction;


  /**
   * Constructor
   */
  Introduction*  Introduction_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroy
   */
  void  Introduction_destroy (Introduction *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  Introduction_isDisplayed (Introduction *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  Introduction_presentation (Introduction *this);
    
  
#endif //INTRODUCTION_H
