#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>
#include "global.h"
#include "display/introduction.h"
/**
  * Class MainWindow
  * The main window of the software (gathers all the stages of the capture)
  * 
  */
typedef struct MainWindow
{
  /**
   * Fields
   */
  /**
   * The next page (introduction)
   */
   Introduction *introduction;
} MainWindow;


  /**
   * Constructor
   */
  MainWindow*  MainWindow_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  MainWindow_destroy (MainWindow *this);
    
  
  /**
   * Displays the main window
   * @param *this The object to be displayed
   */
  void  MainWindow_presentation (MainWindow *this);
    
  
#endif //MAINWINDOW_H
