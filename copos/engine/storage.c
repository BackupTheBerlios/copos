/************************************************************************
* Fichier          : storage.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#include "engine/storage.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Storage"

/**
 * The constructor
 */
Storage*  Storage_new ()
{
  /* Variables and pre-cond */
  Storage* ret = (Storage*) g_malloc(sizeof(Storage));
  /* Code */
  ret->file = NULL;
  ret->filename = NULL;
  ret->width = 0;
  ret->height = 0;
  ret->bytesPerPixel = 0;
  ret->nbImages = 0;
  return ret;
}

  

/**
 * Destroy the object
 * @param *this The object to be destroy
 */
void  Storage_destroy (Storage *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);

  /* Code */
  if(this->file != NULL) {
    Storage_close(this);
  }
  if(this->filename != NULL) {
    /* remove the temporary file */
    g_return_if_fail(remove(this->filename) == 0);
    g_free(this->filename);
  }
  g_free(this);
}

  

/**
 * Open a temporary file to store the images (it closes and destroy the previous one if it exists)
 * @param *this The objet to start the record
 * @param width The width of image (will be used as verificator) 
 * @param height The height of image (will be used as verificator) 
 * @param bytesPerPixel The number of bytes per pixel of image (will be used as verificator) 
 */
void  Storage_record (Storage *this, guint width, guint height, guint bytesPerPixel)
{
  /* Variables and pre-cond */
  char filename[] = TMP_STORAGE;  /* Temporary file in /tmp */
  int  fd;

  g_return_if_fail(this != NULL);
  g_return_if_fail(width > 0);
  g_return_if_fail(height > 0);
  g_return_if_fail(bytesPerPixel > 0);
  /* Code */
  if(this->file != NULL) {
    Storage_close(this);
  }
  if(this->filename != NULL) {
    g_return_if_fail(remove(this->filename) == 0);
    g_free(this->filename);
    this->filename = NULL;
  }
  if((fd = g_mkstemp(filename)) < 0){
    g_warning("mkstemp failed");
    return;
  }
  this->file = fdopen(fd, "wb");
  this->filename = strdup(filename);
  this->width = width;
  this->height = height;
  this->bytesPerPixel = bytesPerPixel;
}

  

/**
 * Add the image to the file
 * @param *this The object of storage
 * @param width The width of image
 * @param height The height of image
 * @param BytesPerPixel The number of bytes per pixel
 * @param *image The image to be stored
 */
void  Storage_put (Storage *this, guint width, guint height, guint bytesPerPixel, guchar *image)
{
  /* Variables and pre-cond */
  guint nb = 0;
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->file != NULL);
  g_return_if_fail(width == this->width);
  g_return_if_fail(height == this->height);
  g_return_if_fail(bytesPerPixel == this->bytesPerPixel);
  /* Code */
  nb = fwrite( image, sizeof(guchar), width*height*bytesPerPixel, this->file);
  if(nb != (width*height*bytesPerPixel)) {
    g_warning("Impossible to write in the temporary file");
  }
  this->nbImages += 1;
}

  

/**
 * Return all the informations about each image
 * @param *this The object which gives informations
 * @param *width Pointer which will contain the width 
 * @param *height Pointer which will contain the height 
 * @param *bytesPerPixels Pointer which will contain the number of bytes per pixel
 */
void  Storage_getInfos (Storage *this, guint *width, guint *height, guint *bytesPerPixel)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->file != NULL);
  /* Code */
  if(width != NULL) { *width  = this->width;}
  if(height != NULL) {*height = this->height;}
  if(bytesPerPixel != NULL) {*bytesPerPixel = this->bytesPerPixel;}
}

  

/**
 * Begin the reading of the preceding recorded file (start)
 * @param *this The object which contains images
 */
void  Storage_readBegin (Storage *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->filename != NULL);
  /* Code */
  if(this->file != NULL) {
    Storage_close(this);
  }
  this->file = fopen(this->filename,"rb");
}

  

/**
 * Begin the reading of the preceding recorded file (end)
 * @param *this The object which contains the images
 */
void  Storage_readEnd (Storage *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->filename != NULL);
  /* Code */
  if(this->file != NULL) {
    Storage_close(this);
  }
  this->file = fopen(this->filename,"rb");
  fseek(this->file, 0L, SEEK_END); /* go to the end */
}

  

/**
 * Return the current image and select the next
 * @param *this The object which return the current image
 * @param *image The returned image (must be initialized)
 */
gboolean  Storage_get (Storage *this, guchar *image)
{
  /* Variables and pre-cond */
  guint nb = 0;
  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(this->file != NULL, FALSE);
  /* Code */
  nb = fread(image, sizeof(guchar), 
	     (this->width*this->height*this->bytesPerPixel), 	     
	     this->file);
  if(nb == 0) {
    g_message("End of the file");
    return FALSE;
  }
  if(nb != (this->width*this->height*this->bytesPerPixel)) {
    g_warning("Impossible to read the temporary file");
    return FALSE;
  }
  return TRUE;
}

  

/**
 * Return the current image and select the previous
 * @param *this The object
 * @param *image The returned image (must be initialized)
 */
gboolean  Storage_getReverse (Storage *this, guchar *image)
{
  /* Variables and pre-cond */
  guint nb = 0;
  gint err = 0;
  g_return_val_if_fail(this != NULL, FALSE);
  g_return_val_if_fail(this->file != NULL, FALSE);
  nb = ftell(this->file);
  if(nb == 0) {
    g_message("Start of the file");
    return FALSE;
  }
  err = fseek(this->file, 
	      -(this->width*this->height*this->bytesPerPixel), 
	      SEEK_CUR);
  if(err != 0) {
    g_warning("Impossible to seek in the stream");
    return FALSE;
  }
  /* Code */
  nb = fread(image, 
	     sizeof(guchar), 
	     (this->width*this->height*this->bytesPerPixel), 	     
	     this->file);
  if(nb != (this->width*this->height*this->bytesPerPixel)) {
    g_warning("Impossible to read the temporary file");
    return FALSE;
  }
  err = fseek(this->file, 
	      -(this->width*this->height*this->bytesPerPixel), 
	      SEEK_CUR);
  if(err != 0) {
    g_warning("Impossible to seek in the stream");
    return FALSE;
  }
  return TRUE;
  
}

  

/**
 * Test if the object is on reading or recording
 * @param *this The object to be tested
 */
gboolean  Storage_isOpen (Storage *this)
{
  /* Variables and pre-cond */
  g_return_val_if_fail(this != NULL, FALSE);
  /* Code */
  if(this->file != NULL) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

  

/**
 * Stop the recording or reading and close the file
 * @param *this The object to be closed
 */
void  Storage_close (Storage *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->file != NULL);
  /* Code */
  fclose(this->file);
  this->file = NULL;
}

  

