/************************************************************************
* Fichier          : philips740k.h
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:09:09 with umbrello
**************************************************************************/

#ifndef PHILIPS740K_H
#define PHILIPS740K_H

/*test*/
#include <string.h>
#include <errno.h>
/**/
#include <pwc-ioctl.h>

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
  * Class Philips740K
  * Structure spécifique à la philips 740K
  * 
  */
typedef struct Philips740K
{
  /**
   * Fields
   */
  /**
   * Le nombre de taille spécifique à la webcam
   */
   guint nbSize;
  /**
   * Le tableau contenant toutes les largeurs possibles
   */
   guint *tabSizeX;
  /**
   * Le tableau contenant toutes les longueur possibles
   */
   guint *tabSizeY;
  /**
   * La vitesse maximale (fonction de la taille)
   */
   guint *tabMaxFrame;
  /**
   * Le buffer contenant l'image
   */
   guchar *image;
  /**
   * Contient les caractéristique du buffer video
   */
   struct video_mbuf vmbuf;
  /**
   * Contient les caractéristiques de l'image capturée
   */
   struct video_mmap vmap;
  /**
   * Détermine si la webcam à été correctement initialisé
   */
   gboolean init;
  /**
   * La vitesse d'obturation
   */
   gint shuttler;
} Philips740K;

  
  /**
   * Constructeur d'une camera Philips
   * @param device Le device de la webcam
   */

  Webcam*  Philips740K_new (const gchar *device);
  
  /**
   * Cherche à initialiser la caméra de façon optimal
   * @param *this La webcam à initialiser
   */
  gint  Philips740K_init (Webcam *this);
    
  
  /**
   * Cherche à mettre à jours la caméra avec les paramêtres de la structure
   * @param this La webcam à mettre à jour
   */
  gboolean  Philips740K_update (Webcam* this);
    
  
  /**
   * Rempli le tableau avec l'image RGB24
   * @param this La webcam qui renvoit l'image
   * @param *image Le tableau initialiser recevant l'image
   */
  gboolean  Philips740K_getRGB (Webcam* this, guchar *image);

    
  /**
   * Test si l'orientation de la camera est en paysage
   * @param *this L'objet à tester
   */
  gboolean  Philips740K_isLandscape (Webcam *this);


  /**
   * Détruit la caméra Philips
   * @param *this La webcam à fermer
   */
  gboolean  Philips740K_close (Webcam *this);
    
  guint Philips740K_maximumSize(struct Webcam*);
#endif //PHILIPS740K_H
