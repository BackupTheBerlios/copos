/************************************************************************
* Fichier          : confvidctrl.h
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#ifndef CONFVIDCTRL_H
#define CONFVIDCTRL_H

#include <sys/ioctl.h>
#include <linux/videodev.h>
#include <gtk/gtk.h>

#include "webcam.h"

  /**
   * Affiche les widgets
   * @param *this L'objet
   * @param *parent Le containeur recevant les widgets
   */
  void  ConfVidCtrl_presentation (Webcam *webcam, GtkContainer *parent);
    
  
#endif //CONFVIDCTRL_H
