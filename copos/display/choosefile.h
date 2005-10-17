#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H

#include <gtk/gtk.h>
#include "global.h"
#include "engine/storage.h"
#include "display/picklaser.h"

/**
  * Class ChooseFile
  * Class to select the appropriate video file and display information
  * 
  */
typedef struct ChooseFile
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
  GtkWidget *mainWidget;
  /**
   * The label of video codec
   */
  GtkWidget *lbl_video;
  /**
   * The label of the size of video
   */
  GtkWidget *lbl_size;
  /**
   * The label of the number of image
   */
  GtkWidget *lbl_nbImg;
  /**
   * The entry text of the file name
   */
  GtkWidget *ent_browse;
  /**
   * The togglebutton to know how to orient the image
   */
  GtkWidget *chb_orientation;
  /**
   * The button to launch the next page
   */
  GtkWidget *btn_forward;
  /**
   * A pointer to the next page
   */
  PickLaser *pickLaser;
  
} ChooseFile;


  /**
   * Constructor
   */
  ChooseFile*  ChooseFile_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroy
   */
  void  ChooseFile_destroy (ChooseFile *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  ChooseFile_isDisplayed (ChooseFile *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  ChooseFile_presentation (ChooseFile *this);
    
  
#endif //CHOOSEFILE_H
