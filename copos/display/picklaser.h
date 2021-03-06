#ifndef PICKLASER_H
#define PICKLASER_H

#include <gtk/gtk.h>
#include "global.h"
#include "engine/storage.h"
#include "engine/imageanalyse.h"
#include "display/positions.h"
/**
  * Class PickLaser
  * Class to select the appropriate video file and display information
  * 
  */
typedef struct PickLaser
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * The object which analyze the image
   */
   ImageAnalyse *imageAnalyse;
  /**
   * Widget to display the image
   */
   GtkWidget *image;
  /**
   * Store the original image
   */
   GdkPixbuf *pixbuf;  
  /**
   * the scale of the image in percent
   */
   gdouble scale;
  /**
   * Variable to test if the user have clicked on the horizontal scroller
   */
   gboolean hsc_clicked;
  /**
   * Horizontal scroller to select the amount of red
   */
   GtkWidget *hsc_red;
  /**
   * Horizontal scroller to select the amount of green
   */
   GtkWidget *hsc_green;
  /**
   * Horizontal scroller to select the amount of blue
   */
   GtkWidget *hsc_blue;
  /**
   * Label to display the nomber of selected points
   */
   GtkWidget *lbl_nbPoints;
  /**
   * An handler of the cursor (change over the image)
   */
   GdkCursor *cursor;
  /**
   * A pointer to the next page
   */
  Positions *positions;
} PickLaser;


  /**
   * Constructor
   */
  PickLaser*  PickLaser_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroy
   */
  void  PickLaser_destroy (PickLaser *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  PickLaser_isDisplayed (PickLaser *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  PickLaser_presentation (PickLaser *this);
    
  
#endif //PICKLASER_H
