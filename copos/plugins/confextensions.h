/************************************************************************
* Fichier          : confextensions.h
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#ifndef CONFEXTENSIONS_H
#define CONFEXTENSIONS_H

#include <stdlib.h>
#include <pwc-ioctl.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>
#include <gtk/gtk.h>

#include "webcam.h"
#include "philips740k.h"

/**
  * Class ConfExtensions
  * Configuration des extensions Philips
  * 
  */
typedef struct ConfExtensions
{
  /**
   * Fields
   */
  /**
   * La webcam à modifier
   */
   Webcam *webcam;
  /**
   * Widget selecteur gain automatique
   */
   GtkWidget *hsc_agc;
  /**
   * Widget selecteur vitesse d'obturation
   */
   GtkWidget *hsc_shuttler;
  /**
   * Widget selecteur contour plus net
   */
   GtkWidget *hsc_contour;
  /**
   * Widget selecteur du niveau de rouge
   */
   GtkWidget *vsc_red;
  /**
   * Widget selecteur du niveau de bleu
   */
   GtkWidget *vsc_blue;
} ConfExtensions;


  /**
   * Constructeur
   * @param *webcam La webcam à modifier
   */
  ConfExtensions*  ConfExtensions_new (Webcam *webcam);
    
  
  /**
   * Destructeur
   * @param *this L'objet à detruire
   */
  void  ConfExtensions_delete (ConfExtensions *this);
    
  
  /**
   * Affiche les widgets
   * @param *this L'objet
   * @param *parent Le parent où seront affiché les widgets
   */
  void  ConfExtensions_presentation (ConfExtensions *this, GtkContainer *parent);
    
  
#endif //CONFEXTENSIONS_H
