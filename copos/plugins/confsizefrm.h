/************************************************************************
* Fichier          : confsizefrm.h
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#ifndef CONFSIZEFRM_H
#define CONFSIZEFRM_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "webcam.h"
#include "philips740k.h"

/**
  * Class ConfSizeFrm
  * La presentation de la configuration de la taille et du framerate
  * 
  */
typedef struct ConfSizeFrm
{
  /**
   * Fields
   */
  /**
   * Les boutons représentant les tailles de capture
   */
   GSList *size;
  /**
   * Les boutons représentant l'orientation de la capture
   */
   GSList *orientation;
  /**
   * La vitesse de capture
   */
   GtkWidget *framerate;
  /**
   * La webcam qui sera modifié
   */
   Webcam *webcam;
} ConfSizeFrm;


  /**
   * Constructeur
   * @param *webcam La webcam qui sera modifié
   */
  ConfSizeFrm*  ConfSizeFrm_new (Webcam *webcam);
    
  
  /**
   * Destructeur
   * @param *this L'objet à détruire
   */
  void  ConfSizeFrm_delete (ConfSizeFrm *this);
    
  
  /**
   * Fonction affichant les widgets dans le parent
   * @param *this L'objet à modifier
   * @param *parent Le parent dans lequel on ajoute la presentation
   */
  void  ConfSizeFrm_presentation (ConfSizeFrm *this, GtkContainer *parent);
    
  
#endif //CONFSIZEFRM_H
