#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#include <gtk/gtk.h>
#include "global.h"
#include "display/choosefile.h"
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
  ChooseFile *chooseFile;
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
