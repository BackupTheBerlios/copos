/************************************************************************
* Fichier          : genericv4l.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#include "genericv4l.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "GenericV4L"

static void YUV420pToRGB(Webcam *this, guchar *src, guchar *dst);

/**
 * Exported function of the module. it returns the webcam structure
 * @param *device The file/device of the webcam
 */
G_MODULE_EXPORT Webcam*  getWebcam (const gchar *device)
{
  return GenericV4L_new(device);
}

/**
 * Constructor of a generic webcam
 * @param *device The name of the file/device
 */
Webcam*  GenericV4L_new (const gchar *device)
{
  /* Variables and pre-cond */
  gint video_fd;
  Webcam* ret;
  GenericV4L* genericv4l;

  g_assert(device != NULL);
  video_fd = open(device, O_RDWR);
  g_assert(video_fd > 0);
  /* Code */
  /* Generic part of the webcam */
  ret = (Webcam*) g_malloc(sizeof(Webcam));
  ret->device = video_fd;
  ret->name = NULL;
  ret->width  = 0;
  ret->height = 0;
  ret->framerate = 0;
  ret->data = (GenericV4L*) g_malloc(sizeof(GenericV4L));
  ret->init = GenericV4L_init;
  ret->update = GenericV4L_update;
  ret->getRGB = GenericV4L_getRGB;
  ret->isLandscape = GenericV4L_isLandscape;
  ret->close  = GenericV4L_close;
  ret->maximumSize = GenericV4L_maximumSize;
  /* Specifique part of the webcam */
  genericv4l = (GenericV4L*) ret->data;
  genericv4l->image = NULL;

  return ret;
}

  

/**
 * It tries to initialize the webcam in an optimal way
 * @param *this The webcam to be initialized 
 */
gboolean  GenericV4L_init (Webcam *this)
{
  /* Variables and pre-cond */
  GenericV4L *genericv4l;
  struct video_capability vcap;
  struct video_picture    vpicture;

  if(this == NULL) {
    g_warning("Webcam == NULL !");
    close(this->device);
    return FALSE;
  }
  g_assert(this->device != 0);
  genericv4l = (GenericV4L*) this->data;
  /* Code */
  if(ioctl(this->device, VIDIOCGCAP, &vcap) < 0) {
    g_warning("Couldn't get capabilities from Video4Linux");
    close(this->device);
    return FALSE;
  }
  this->name = strdup(vcap.name);
  this->width = vcap.maxwidth;
  this->height = vcap.maxheight;

  if(ioctl(this->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Couldn't get picture informations from Video4Linux \n");
    close(this->device);
    return FALSE;
  }
  if(vpicture.palette != VIDEO_PALETTE_YUV420P) {
    vpicture.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(this->device, VIDIOCSPICT, &vpicture) < 0) {
      g_warning("Couldn't write the picture configuration\n");
      close(this->device);
      return FALSE;
    }
  }
  if(!GenericV4L_update(this)) {
    close(this->device);
    return FALSE;
  }
  return TRUE;
}

  

/**
 * It tries to update the webcam with the parameters of the structure
 * @param *this The webcam to be updated
 */
gboolean  GenericV4L_update (Webcam *this)
{
  /* Variables and pre-cond */
  GenericV4L *genericv4l;
  struct video_window vwindow;
  if(this == NULL) {
    g_warning("Webcam == NULL !");
    close(this->device);
    return FALSE;
  }
  g_assert(this->device != 0);
  genericv4l = (GenericV4L*) this->data;
  /* Code */

  if(genericv4l->image != NULL) {
    munmap(genericv4l->image, genericv4l->vmbuf.size);
    genericv4l->image = NULL;
  }
  if(ioctl(this->device, VIDIOCGWIN, &vwindow) < 0) {
    g_warning("Couldn't read the configuration of the video window\n");
    return FALSE;
  }
  if(this->isLandscape(this)) {
    vwindow.width  = this->width;
    vwindow.height = this->height;
  }
  else {
    vwindow.width  = this->height;
    vwindow.height = this->width;
  }
  vwindow.flags = 589824; /* A specific value for the philips webcam */
  if((ioctl(this->device, VIDIOCSWIN, &vwindow)) < 0) {
    g_warning("Couldn't write the configuration of the video window\n");
    return FALSE;
  }
  if(ioctl(this->device, VIDIOCGMBUF, &genericv4l->vmbuf) < 0) {
    g_warning("Couldn't read the configuration of the video buffer\n");
    return FALSE;
  }
  genericv4l->image = mmap(0, genericv4l->vmbuf.size, PROT_READ, MAP_SHARED, 
			   this->device, 0);
  genericv4l->vmap.frame = 0;
  genericv4l->vmap.width  = vwindow.width;
  genericv4l->vmap.height = vwindow.height;
  genericv4l->vmap.format = VIDEO_PALETTE_YUV420P;

  return TRUE;

}

  

/**
 * It fills out the array with a RGB24 image
 * @param *this The webcam which return the image 
 * @param *image The initialized array which receive the image 
 */
gboolean  GenericV4L_getRGB (Webcam *this, guchar *image)
{
  /* Variables and pre-cond */
  GenericV4L *genericv4l;
  g_return_val_if_fail(this != NULL, FALSE);
  g_assert(this->device != 0);
  genericv4l = (GenericV4L*) this->data;
  /* Code */
  if(ioctl(this->device, VIDIOCMCAPTURE, &genericv4l->vmap) < 0) {
    g_warning("Couldn't grab the current frame\n");
    return FALSE;
  }
  else {
    if(ioctl(this->device, VIDIOCSYNC, &genericv4l->vmap) < 0) {
      g_warning("Couldn't find the end of the current frame\n");
      return FALSE;
    }
  }
  if(genericv4l->vmap.format == VIDEO_PALETTE_YUV420P) {
    YUV420pToRGB(this,genericv4l->image, image);
  }
  else {
    guint i;
    guint size = genericv4l->vmap.width * genericv4l->vmap.height * 3;
    for(i=0; i<size; ++i) {
      image[i] = genericv4l->image[i];
    }
  }

  return TRUE;
}

  

/**
 * Test if the orientation of the webcam is in landscape 
 * @param *this The object to test
 */
gboolean  GenericV4L_isLandscape (Webcam *this)
{
  /* Variables and pre-cond */
  /* Code */
  return (this->width > this->height);
}

  

/**
 * Return the maximum size of the buffer (guchar *) for the highest resolution
 * @param *this The initialized webcam
 */
guint  GenericV4L_maximumSize (Webcam *this)
{
  /* Variables and pre-cond */
  GenericV4L *genericv4l;
  g_return_val_if_fail(this != NULL, FALSE);
  g_assert(this->device != 0);
  genericv4l = (GenericV4L*) this->data;
  guint size;
  /* Code */
  size = sizeof(guchar)*this->width*this->height*3;
  return size;
}

  

/**
 * Close the webcam and free it
 * @param *this The webcam to be closed
 */
gboolean  GenericV4L_close (Webcam *this)
{
  /* Variables and pre-cond */
  GenericV4L *genericv4l;
  g_return_val_if_fail(this != NULL, FALSE);
  g_assert(this->device != 0);
  genericv4l = (GenericV4L*) this->data;
  /* Code */
  if(genericv4l->image != NULL) {
    munmap(genericv4l->image, genericv4l->vmbuf.size);
    g_free(this->name);
  }
  close(this->device);
  g_free(this);
  return TRUE;
}

  

static void YUV420pToRGB(Webcam *this, guchar *src, guchar *dst) {
  gint line, col, linewidth;
  gint y, u, v, yy, vr, ug, vg, ub;
  gint r, g, b;
  guchar *py, *pu, *pv;
  guchar *pixel;
  GenericV4L* genericv4l = (GenericV4L*) this->data;
  gint width  = genericv4l->vmap.width;
  gint height = genericv4l->vmap.height;

  linewidth = width >> 1;

  py = src;
  pu = py + (width * height);
  pv = pu + (width * height) / 4;

  y = *py++;
  yy = y << 8;
  u = *pu - 128;
  ug =   88 * u;
  ub =  454 * u;
  v = *pv - 128;
  vg =  183 * v;
  vr =  359 * v;

  if(this->isLandscape(this)) {
    pixel = dst;
  } 
  else {
    pixel = dst+((this->width)*3)*(this->height-1);
  }

  for (line = 0; line < height; line++) {
    for (col = 0; col < width; col++) {
      r = (yy +      vr) >> 8;
      g = (yy - ug - vg) >> 8;
      b = (yy + ub     ) >> 8;
      
      if (r < 0)   r = 0;
      if (r > 255) r = 255;
      if (g < 0)   g = 0;
      if (g > 255) g = 255;
      if (b < 0)   b = 0;
      if (b > 255) b = 255;

      pixel[0] = r;
      pixel[1] = g;
      pixel[2] = b;
      
      if(this->isLandscape(this)) {
	pixel += 3;
      }
      else {
	/* on remonte d'une ligne */
	if(pixel != (dst+line*3)) {
	  pixel -= (this->width*3);
	}
      }
	y = *py++;
	yy = y << 8;
	if (col & 1) {
	  pu++;
	  pv++;
	  
	  u = *pu - 128;
	  ug =   88 * u;
	  ub =  454 * u;
	  v = *pv - 128;
	  vg =  183 * v;
	  vr =  359 * v;
	}
    } /* ..for col */
    if ((line & 1) == 0) { // even line: rewind
      pu -= linewidth;
      pv -= linewidth;
    }
    if(! this->isLandscape(this)) {
      /* On redescend à la dernière ligne + decalage d'un pixel */
      pixel = pixel+((this->width)*3)*(this->height-1)+3;
    }
  } /* ..for line */
}
