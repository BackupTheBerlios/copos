/************************************************************************
* Fichier          : confphilips.h
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#ifndef CONFPHILIPS_H
#define CONFPHILIPS_H

#include <gtk/gtk.h>

#include "webcam.h"
#include "philips740k.h"

#include "confsizefrm.h"
#include "confvidctrl.h"
#include "confextensions.h"

/**
  * Class ConfPhilips
  * Configuration générale de la webcam
  * 
  */
typedef struct ConfPhilips
{
  /**
   * Fields
   */
  /**
   * La webcam qui sera modifié
   */
   Webcam *webcam;
  /**
   * Affichage de la configuration de la taille et framerate
   */
   ConfSizeFrm *confSizeFrm;
  /**
   * Affichage de la configuration des extensions philips
   */
   ConfExtensions *confExtensions;
} ConfPhilips;


  /**
   * Construit et affiche la boite de dialogue de configuration
   * @param *webcam La webcam à modifier
   */
  G_MODULE_EXPORT void  getGtkConf (Webcam *webcam);
    
  
  /**
   * Constructeur
   * @param *webcam La webcam concerné par les modifications
   */
  ConfPhilips*  ConfPhilips_new (Webcam *webcam);
    
  
  /**
   * Destructeur
   * @param *this L'objet à detruire
   */
  void  ConfPhilips_delete (ConfPhilips *this);
    
  
  /**
   * Affichage de la boite de dialogue
   * @param *this L'objet à afficher
   */
  void  ConfPhilips_presentation (ConfPhilips *this);
    
  
#endif //CONFPHILIPS_H
