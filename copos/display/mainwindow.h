/************************************************************************
* Fichier          : mainwindow.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 11:05:55 with umbrello
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>
#include "global.h"
#include "introduction.h"

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
