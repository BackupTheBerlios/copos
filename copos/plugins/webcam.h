/************************************************************************
* Fichier          : webcam.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef WEBCAM_H
#define WEBCAM_H
#include <gmodule.h>
/**
  * Class Webcam
  * Abstract class to manage a webcam
  * 
  */
typedef struct Webcam
{
  /**
   * Fields
   */
  /**
   * The device open for the webcam
   */
   int device;
  /**
   * Name of the webcam
   */
   char *name;
  /**
   * The width of the returned picture
   */
   guint width;
  /**
   * The height of the returned picture
   */
   guint height;
  /**
   * The speed of acquisition of the webcam 
   */
   guint framerate;
  /**
   * Specific data to the webcam 
   */
   gpointer data;
  /**
   * Initialization function of the parameters of the webcam, return true or false
   */
   gboolean (*init)(struct Webcam*);
  /**
   * This function tries to update the parameters of the webcam with the values of the structure. It returns false if failure and gives back the initial values in the structure.
   */
   gboolean (*update)(struct Webcam*);
  /**
   * Fill the array in argument with the captured picture (RGB24) 
   */
   gboolean (*getRGB)(struct Webcam*,guchar*);
  /**
   * Test if the orientation of the camera is of the landscape type
   */
   gboolean (*isLandscape)(struct Webcam*);
  /**
   * Return the maximum size of the buffer (guchar *) for the highest resolution
   */
   guint (*maximumSize)(struct Webcam*);
  /**
   * Close the webcam and free it
   */
   gboolean (*close)(struct Webcam*);
} Webcam;


  /**
   * Exported function of the module. it returns the webcam structure
   * @param *device The file/device of the webcam
   */
  G_MODULE_EXPORT Webcam*  getWebcam (const gchar *device);
    
  
#endif //WEBCAM_H
