/************************************************************************
* Fichier          : genericv4l.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef GENERICV4L_H
#define GENERICV4L_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev.h>
#include <string.h>
#include "webcam.h"

/**
  * Class GenericV4L
  * A generic class to manage the webcam via video 4 linux
  * 
  */
typedef struct GenericV4L
{
  /**
   * Fields
   */
  /**
   * The buffer of the picture
   */
   guchar *image;
  /**
   * The characteristic of the video buffer
   */
   struct video_mbuf vmbuf;
  /**
   * The characteristic of the captured picture
   */
   struct video_mmap vmap;

} GenericV4L;


  /**
   * Constructor of a generic webcam
   * @param *device The name of the file/device
   */
  Webcam*  GenericV4L_new (const gchar *device);
    
  
  /**
   * It tries to initialize the webcam in an optimal way
   * @param *this The webcam to be initialized 
   */
  gboolean  GenericV4L_init (Webcam *this);
    
  
  /**
   * It tries to update the webcam with the parameters of the structure
   * @param *this The webcam to be updated
   */
  gboolean  GenericV4L_update (Webcam *this);
    
  
  /**
   * It fills out the array with a RGB24 image
   * @param *this The webcam which return the image 
   * @param *image The initialized array which receive the image 
   */
  gboolean  GenericV4L_getRGB (Webcam *this, guchar *image);
    
  
  /**
   * Test if the orientation of the webcam is in landscape 
   * @param *this The object to test
   */
  gboolean  GenericV4L_isLandscape (Webcam *this);
    
  
  /**
   * Return the maximum size of the buffer (guchar *) for the highest resolution
   * @param *this The initialized webcam
   */
  guint  GenericV4L_maximumSize (Webcam *this);
    
  
  /**
   * Close the webcam and free it
   * @param *this The webcam to be closed
   */
  gboolean  GenericV4L_close (Webcam *this);
    
  
#endif //GENERICV4L_H
