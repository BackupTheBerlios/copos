/************************************************************************
* Fichier          : storage.h
* Date de Creation : Thu Sep 29 2005
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated with umbrello
**************************************************************************/

#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>

/**
  * Class Storage
  * A class which manages the  video files (with ffmpeg)
  * 
  */
typedef struct Storage
{
  /**
   * Fields
   */
  /**
   * The path of the video file
   */
   gchar *filename;
  /**
   * number of pictures of the video (approximately)
   */
   guint nbImages;
  /**
   * pointer to the format context of the video
   */
   AVFormatContext *formatCtx;
  /**
   * pointer to the codec context of the video
   */
   AVCodecContext *codecCtx;
  /**
   * the index of the first video stream
   */
   gint streamIndex;
  /**
   * the name of the codec to read this video
   */
   gchar *codecName;
  /**
   * A field to change the orientation of the video
   */
  gboolean hasChangedOrientation;

} Storage;


  /**
   * Constructor
   */
  Storage*  Storage_new ();
    
  
  /**
   * Destructor
   */
  void  Storage_destroy (Storage *this);
    
  
  /**
   * Set the path to the video file
   * @param *filename the path to the video file
   */
  void  Storage_setFileName (Storage *this, const gchar *filename);
    
  
  /**
   * Return all the informations about each image
   * @param *width pointer which receive the width 
   * @param *height pointer which receive the height
   * @param *bytesPerPixel pointer which receive the number of bytes per pixel
   */
  void  Storage_getInfos (Storage *this, guint *width, guint *height, guint *bytesPerPixel, gchar *codecName);
    
  
  /**
   * Begin the loading of the video file
   */
  gboolean  Storage_load (Storage *this);
    
  
  /**
   * Return the current image and select the next
   * @param *image the returned Storage_image (Storage *this, must be initialized)
   */
  gboolean  Storage_get (Storage *this, guchar *image);
    
  /**
   * Seek to a frame
   * @param pos pourcentage position of frame in total timeline
   */
  gboolean  Storage_seek (Storage *this, guint pos);
  /**
   * Test if the object is loaded
   */
  gboolean  Storage_isLoaded (Storage *this);
    
  
  /**
   * Stop the reading and close the video file
   */
  void  Storage_close (Storage *this);
    
  
#endif //STORAGE_H
