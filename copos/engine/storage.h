/************************************************************************
* Fichier          : storage.h
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#ifndef STORAGE_H
#define STORAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>

#define TMP_STORAGE "/tmp/coposXXXXXX"

/**
  * Class Storage
  * A class which manages the temporary files of images 
  * 
  */
typedef struct Storage
{
  /**
   * Fields
   */
  /**
   * A pointer on the open file
   */
   FILE *file;
  /**
   * The path of the temporary file
   */
   gchar *filename;
  /**
   * The width of each picture
   */
   guint width;
  /**
   * The height of each picture
   */
   guint height;
  /**
   * The number of bytes per pixel for each image
   */
   guint bytesPerPixel;
  /**
   * The number of recorded images
   */
   guint nbImages;
} Storage;


  /**
   * The constructor
   */
  Storage*  Storage_new ();
    
  
  /**
   * Destroy the object
   * @param *this The object to be destroy
   */
  void  Storage_destroy (Storage *this);
    
  
  /**
   * Open a temporary file to store the images (it closes and destroy the previous one if it exists)
   * @param *this The objet to start the record
   * @param width The width of image (will be used as verificator) 
   * @param height The height of image (will be used as verificator) 
   * @param bytesPerPixel The number of bytes per pixel of image (will be used as verificator) 
   */
  void  Storage_record (Storage *this, guint width, guint height, guint bytesPerPixel);
    
  
  /**
   * Add the image to the file
   * @param *this The object of storage
   * @param width The width of image
   * @param height The height of image
   * @param BytesPerPixel The number of bytes per pixel
   * @param *image The image to be stored
   */
  void  Storage_put (Storage *this, guint width, guint height, guint bytesPerPixel, guchar *image);
    
  
  /**
   * Return all the informations about each image
   * @param *this The object which gives informations
   * @param *width Pointer which will contain the width 
   * @param *height Pointer which will contain the height 
   * @param *bytesPerPixels Pointer which will contain the number of bytes per pixel
   */
  void  Storage_getInfos (Storage *this, guint *width, guint *height, guint *bytesPerPixel);
    
  
  /**
   * Begin the reading of the preceding recorded file (start)
   * @param *this The object which contains images
   */
  void  Storage_readBegin (Storage *this);
    
  
  /**
   * Begin the reading of the preceding recorded file (end)
   * @param *this The object which contains the images
   */
  void  Storage_readEnd (Storage *this);
    
  
  /**
   * Return the current image and select the next
   * @param *this The object which return the current image
   * @param *image The returned image (must be initialized)
   */
  gboolean  Storage_get (Storage *this, guchar *image);
    
  
  /**
   * Return the current image and select the previous
   * @param *this The object
   * @param *image The returned image (must be initialized)
   */
  gboolean  Storage_getReverse (Storage *this, guchar *image);
    
  
  /**
   * Test if the object is on reading or recording
   * @param *this The object to be tested
   */
  gboolean  Storage_isOpen (Storage *this);
    
  
  /**
   * Stop the recording or reading and close the file
   * @param *this The object to be closed
   */
  void  Storage_close (Storage *this);
    
  
#endif //STORAGE_H
