/************************************************************************
* Fichier          : philips740k.c
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:09:09 with umbrello
**************************************************************************/

#include "philips740k.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "Philips740K"

void YUV420pToRGB(Webcam *this, guchar *src, guchar *dst);

/**
 * Fonction du module retournant la webcam Philips 
 * @param device Le device de la webcam
 */
G_MODULE_EXPORT Webcam*  getWebcam (const gchar *device)
{
  return Philips740K_new(device);
}

/**
 * Constructeur d'une camera Philips
 * @param device Le device de la webcam
 */
Webcam*  Philips740K_new (const gchar *device)
{
  gint video_fd;
  Webcam* ret;
  Philips740K* philips;

  g_assert(device != NULL);
  video_fd = open(device, O_RDWR);
  g_assert(video_fd > 0);

  ret = (Webcam*) g_malloc(sizeof(Webcam));
  ret->device = video_fd;
  ret->name = NULL;
  ret->width  = 0;
  ret->height = 0;
  ret->framerate = 0;
  ret->data = (Philips740K*) g_malloc(sizeof(Philips740K));
  ret->init = Philips740K_init;
  ret->update = Philips740K_update;
  ret->getRGB = Philips740K_getRGB;
  ret->isLandscape = Philips740K_isLandscape;
  ret->close  = Philips740K_close;
  ret->maximumSize = Philips740K_maximumSize;
  /* Initialisation spécifique à la structure philips */
  philips = (Philips740K*) ret->data;
  philips->init = FALSE;
  philips->image = NULL;
  philips->shuttler = -1;
  return ret;
}

  

/**
 * Cherche à initialiser la caméra de façon optimal
 * @param *this La webcam à initialiser
 */
gboolean  Philips740K_init (Webcam *this)
{
  struct video_capability vcap;
  struct video_picture    vpicture;
  gint type;
  Philips740K* philips = (Philips740K*) this->data;

  if(this == NULL) {
    g_warning("Webcam == NULL !");
    close(this->device);
    return FALSE;
  }
  g_assert(this->device != 0);
  g_assert(! philips->init);

  g_message("Philips740K init");

  /* Test si une webcam Philips est présente */
  if(ioctl(this->device, VIDIOCGCAP, &vcap) < 0) {
    g_warning("Impossible d'obtenir les information de video4linux \n");
    close(this->device);
    return FALSE;
  }

  if((sscanf(vcap.name, "Philips %d webcam", &type) == 1) && (type == 740)) {
    g_message("C'est une camera Philips %d",type);
  }
  else {
    struct pwc_probe probe;
    /* No match yet; try the PROBE */
    if(ioctl(this->device, VIDIOCPWCPROBE, &probe) == 0) {
      if((!strcmp(vcap.name, probe.name)) && (probe.type == 740)) {
	g_message("C'est une camera compatible Philips %d",probe.type);
      }
      else {
	g_warning("Aucune camera compatible n'a été trouvé\n");
	close(this->device);
	return FALSE;
      }
    }
  }
  this->name = strdup(vcap.name);
  /* fin - test si une webcam philips */
  /* Initialisation du tableau des valeurs possible pour ce device */
  if((vcap.maxwidth > 352) && (vcap.maxheight > 288)) {
    philips->nbSize = 3;
    philips->tabSizeX = (guint*) g_malloc(sizeof(guint) * 3);
    philips->tabSizeY = (guint*) g_malloc(sizeof(guint) * 3);
    philips->tabMaxFrame = (guint*) g_malloc(sizeof(guint) * 3);
    philips->tabSizeX[0] = 640;
    philips->tabSizeY[0] = 480;
    philips->tabMaxFrame[0] = 15;
    philips->tabSizeX[1] = 320;
    philips->tabSizeY[1] = 240;
    philips->tabMaxFrame[1] = 30;
    philips->tabSizeX[2] = 160;
    philips->tabSizeY[2] = 120;
    philips->tabMaxFrame[2] = 30;
  }
  else {
    philips->nbSize = 2;
    philips->tabSizeX = (guint*) g_malloc(sizeof(guint) * 2);
    philips->tabSizeY = (guint*) g_malloc(sizeof(guint) * 2);
    philips->tabMaxFrame = (guint*) g_malloc(sizeof(guint) * 2);
    philips->tabSizeX[0] = 320;
    philips->tabSizeY[0] = 240;
    philips->tabMaxFrame[0] = 9;
    philips->tabSizeX[1] = 160;
    philips->tabSizeY[1] = 120;
    philips->tabMaxFrame[1] = 30;
  }

  /* On essai de mettre les paramêtre au maximum */
/*   this->width  = philips->tabSizeX[0]; */
/*   this->height = philips->tabSizeY[0]; */
  /* Et en landscape */
  this->width  = philips->tabSizeY[0];
  this->height = philips->tabSizeX[0];
  this->framerate = philips->tabMaxFrame[0];
  /* On verifie que c'est la bonne palette */
  if(ioctl(this->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    close(this->device);
    return FALSE;
  }
  if(vpicture.palette != VIDEO_PALETTE_YUV420P) {
    vpicture.palette = VIDEO_PALETTE_YUV420P;
    if(ioctl(this->device, VIDIOCSPICT, &vpicture) < 0) {
      g_warning("Impossible d'ecrire la configuration picture\n");
      close(this->device);
      return FALSE;
    }
  }

  if(ioctl(this->device, VIDIOCPWCSSHUTTER, &philips->shuttler) < 0) {
    g_warning("Impossible d'ecrire la configuration shuttler\n");
    close(this->device);
    return FALSE;
  }

  if(! Philips740K_update(this)) {
    close(this->device);
    return FALSE;
  }
  philips->init = TRUE;
  return TRUE;
}

  

/**
 * Cherche à mettre à jours la caméra avec les paramêtres de la structure
 * @param this La webcam à mettre à jour
 */
gboolean  Philips740K_update (Webcam* this)
{
  struct video_window vwindow;
  Philips740K* philips = (Philips740K*) this->data;

  g_assert(this->device != 0);

  g_message("Philips740K update");

  if(philips->init) {
    munmap(philips->image, philips->vmbuf.size);
  }
  if(ioctl(this->device, VIDIOCGWIN, &vwindow) < 0) {
    g_warning("Impossible de lire la configuration GWIN\n");
    return FALSE;
  }

  /* On essai de mettre les paramêtre */
  g_message("%d %d %d\n", this->width, this->height,this->framerate);
  if(this->isLandscape(this)) {
    vwindow.width  = this->width;
    vwindow.height = this->height;
  }
  else {
    vwindow.width  = this->height;
    vwindow.height = this->width;
  }
  vwindow.flags &= ~PWC_FPS_FRMASK;
  vwindow.flags |= (this->framerate << PWC_FPS_SHIFT);

  if((ioctl(this->device, VIDIOCSWIN, &vwindow)) < 0) {
    g_warning("Impossible d'ecrire la configuration SWIN\n");
    return FALSE;
  }

  /* on recréer le buffer */
  if(ioctl(this->device, VIDIOCGMBUF, &philips->vmbuf) < 0) {
    g_warning("Impossible de lire la configuration mbuf\n");
    return FALSE;
  }
  philips->image = mmap(0, philips->vmbuf.size, PROT_READ, MAP_SHARED, 
			this->device, 0);
  philips->vmap.frame = 0;
  philips->vmap.width  = vwindow.width;
  philips->vmap.height = vwindow.height;
  philips->vmap.format = VIDEO_PALETTE_YUV420P;

  return TRUE;
}

  

/**
 * Rempli le tableau avec l'image RGB24
 * @param this La webcam qui renvoit l'image
 * @param *image Le tableau initialiser recevant l'image
 */
gboolean  Philips740K_getRGB (Webcam* this, guchar *image)
{
  gint err;
  Philips740K* philips = (Philips740K*) this->data;
  g_assert(philips->init);

  err = ioctl(this->device, VIDIOCMCAPTURE, &philips->vmap);
  if( err < 0) {
    g_warning("problème pour la capture d'une frame %d %s\n"
	      ,errno,strerror(errno));
    return FALSE;
  }
  else {
    if(ioctl(this->device, VIDIOCSYNC, &philips->vmap) < 0) {
      g_warning("probleme pour determiner la fin de la frame\n");
      return FALSE;
    }
  }

  YUV420pToRGB(this,philips->image, image);

  return TRUE;
}

/**
 * Test si l'orientation de la camera est en paysage
 * @param *this L'objet à tester
 */
gboolean  Philips740K_isLandscape (Webcam *this)
{
  return (this->width > this->height);
}


/**
 * Détruit la caméra Philips
 * @param *this La webcam à fermer
 */
gboolean  Philips740K_close (Webcam *this)
{
  Philips740K* philips = (Philips740K*) this->data;
  g_assert(this->device != 0);
  g_message("Philips740K close");
  if(philips->init) {
    munmap(philips->image, philips->vmbuf.size);
    g_free(philips->tabSizeX);
    g_free(philips->tabSizeY);
    g_free(philips->tabMaxFrame);
    g_free(philips);
    g_free(this->name);
  }
  close(this->device);
  g_free(this); /* TODO Danger if the function is freed ??? */
  return TRUE;
}

guint Philips740K_maximumSize(struct Webcam* this)
{
  guint size;
  Philips740K* philips = (Philips740K*) this->data;
  g_message("maximumSize");
  size = sizeof(guchar)*philips->tabSizeX[0]*philips->tabSizeY[0]*3;
  return size;
}

void YUV420pToRGB(Webcam *this, guchar *src, guchar *dst) {
  gint line, col, linewidth;
  gint y, u, v, yy, vr, ug, vg, ub;
  gint r, g, b;
  guchar *py, *pu, *pv;
  guchar *pixel;
  Philips740K* philips = (Philips740K*) this->data;
  gint width  = philips->vmap.width;
  gint height = philips->vmap.height;

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
