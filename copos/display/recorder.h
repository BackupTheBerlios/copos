/************************************************************************
* Fichier          : recorder.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef RECORDER_H
#define RECORDER_H
#include <gtk/gtk.h>
#include <math.h>
#include "global.h"
#include "engine/storage.h"
#include "picklaser.h"

/**
  * Class Recorder
  * Widgets allowing to record a sequence of image
  * 
  */
typedef struct Recorder
{
  /**
   * Fields
   */
  /**
   * Pointer containing all the widget of the page (for the destruction)
   */
   GtkWidget *mainWidget;
  /**
   * An object to facilitate the storage of the video
   */
   Storage *storage;
  /**
   * The area where the webcam is drawed
   */
   GtkWidget *dra_recorder;
  /**
   * A buffer to store the next image to be displayed
   */
   guchar *tampon;
  /**
   * handler of the function which update the image of the webcam (for the destruction or the step to the following page) 
   */
   guint tag;
  /**
   * Label of the number of stored images
   */
   GtkWidget *lbl_recorded;
  /**
   * the number of stored images
   */
   guint nbRecorded;
  /**
   * Label of the number of frame per second
   */
   GtkWidget *lbl_fps;
  /**
   * The number of frames per second
   */
   guint nbFPS;
  /**
   * The timer allowing to calculate the number of frames per seconds 
   */
   GTimer *fpsTimer;
  /**
   * An entry text to determinate the duration of the recording
   */
   GtkWidget *ent_timeToRecord;
  /**
   * The duration of the recording (if 0, there's no limit)
   */
   gdouble recordTime;
  /**
   * The timer to control the duration of the recording
   */
   GTimer *recordTimer;
  /**
   * The button to stop the recording
   */
   GtkWidget *btn_stop;
  /**
   * The button to the next page
   */
   GtkWidget *btn_next;
  /**
   * The next page
   */
   PickLaser *pickLaser;
} Recorder;


  /**
   * Constructor
   */
  Recorder*  Recorder_new ();
    
  
  /**
   * Destructor
   * @param *this The object to be destroyed
   */
  void  Recorder_destroy (Recorder *this);
    
  
  /**
   * Test if the page is displayed
   * @param *this The object to be tested
   */
  gboolean  Recorder_isDisplayed (Recorder *this);
    
  
  /**
   * It displays all the widgets in the global container (see Global)
   * @param *this The object to be displayed
   */
  void  Recorder_presentation (Recorder *this);
    
  
#endif //RECORDER_H
