/************************************************************************
* Fichier          : confsizefrm.c
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#include "confsizefrm.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ConfSizeFrm"

void on_size_clicked (GtkButton       *button,
		      gpointer         user_data)
{ 
  const gchar* label = NULL;
  ConfSizeFrm *this    = (ConfSizeFrm*) user_data;
  Webcam      *webcam  = (Webcam*) this->webcam;
  Philips740K *philips = (Philips740K*) webcam->data;

  g_return_if_fail (button != NULL);
  g_return_if_fail (GTK_IS_RADIO_BUTTON(button));
/*   g_message("on_size_clicked"); */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
    label = gtk_button_get_label(GTK_BUTTON(button));
    if(label) {
      gdouble width,height;
      gdouble maxframerate;
      gdouble min, max;
      gdouble value;
      gchar **array;
      gint i;
      array = g_strsplit(label,"x",2);
      width = strtod(array[0],NULL);
      height = strtod(array[1],NULL);
      i=0;
      while((width != philips->tabSizeX[i]) &&
	    (height != philips->tabSizeX[i])) {
	++i;
      }
      maxframerate = philips->tabMaxFrame[i];
      value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(this->framerate));
      gtk_spin_button_get_range(GTK_SPIN_BUTTON(this->framerate),&min, &max);
      gtk_spin_button_set_range(GTK_SPIN_BUTTON(this->framerate),min, maxframerate);
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->framerate),value);
      if(value > max) {
	webcam->framerate = maxframerate;
      }
      webcam->width = width;
      webcam->height = height;
      webcam->update(webcam);
      g_strfreev(array);
    }
  }
}

void on_orientation_clicked (GtkButton       *button,
			     gpointer         user_data)
{
  GtkWidget *rdb_size = NULL;
  ConfSizeFrm *this = (ConfSizeFrm*) user_data;

  g_return_if_fail (button != NULL);
  g_return_if_fail (GTK_IS_RADIO_BUTTON(button));
/*   g_message("on_orientation_clicked"); */
  /* Normalement, à chaque fois qu'un bouton orientation est activé 
     on switch width<->height */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
    GSList *list;
    list = this->size;
    while(list) {
      const gchar* label = gtk_button_get_label(GTK_BUTTON(list->data));
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(list->data))) {
	rdb_size = list->data;
      }
      if(label) {
	gchar **array;
	gchar str[256];
	array = g_strsplit(label,"x",2);
	sprintf(str,"%sx%s",array[1],array[0]);
	gtk_button_set_label(GTK_BUTTON(list->data),str);
	g_strfreev(array);
      }
      list = g_slist_next(list);
    }
    g_signal_emit_by_name(rdb_size,"clicked");
  }
}

void on_spb_frate_value_changed (GtkSpinButton   *spinbutton,
				 gpointer         user_data)
{
  ConfSizeFrm *this    = (ConfSizeFrm*) user_data;
  Webcam      *webcam  = (Webcam*) this->webcam;
  g_return_if_fail (spinbutton != NULL);
  g_return_if_fail (GTK_IS_SPIN_BUTTON(spinbutton));
/*   g_message("on_spb_frate_value_changed"); */
  
  webcam->framerate = (guint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(this->framerate));
  webcam->update(webcam);
}

/**
 * Constructeur
 * @param *webcam La webcam qui sera modifié
 */
ConfSizeFrm*  ConfSizeFrm_new (Webcam *webcam)
{
  ConfSizeFrm* ret = (ConfSizeFrm*) g_malloc(sizeof(ConfSizeFrm));
  g_return_val_if_fail((webcam != NULL), NULL);
  ret->webcam = webcam;
  ret->size = NULL;
  ret->orientation = NULL;
  return ret;
}

  

/**
 * Destructeur
 * @param *this L'objet à détruire
 */
void  ConfSizeFrm_delete (ConfSizeFrm *this)
{
  g_free(this);
}

  

/**
 * Fonction affichant les widgets dans le parent
 * @param *this L'objet à modifier
 * @param *parent Le parent dans lequel on ajoute la presentation
 */
void  ConfSizeFrm_presentation (ConfSizeFrm *this, GtkContainer *parent)
{
  GtkWidget *vbx_size_frate;
  GtkWidget *hbx_size_frate;
  GtkWidget *frm_size;
  GtkWidget *vbx_size;
  GtkWidget *rdb_size;
  GtkWidget *lbl_size;
  GtkWidget *frm_frate;
  GtkWidget *vbx_frate;
  GtkObject *spb_frate_adj;
  GtkWidget *lbl_frate;
  GtkWidget *frm_orientation;
  GtkWidget *vbx_orientation;
  GtkWidget *rdb_landscape;
  GtkWidget *rdb_portrait;
  GtkWidget *lbl_orientation;
  guint i;
  gdouble maxframerate;
  Philips740K* philips = (Philips740K*) this->webcam->data;

/*   g_message("ConfSizeFrm_presentation"); */

  vbx_size_frate = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_size_frate);
  gtk_container_add (GTK_CONTAINER (parent), vbx_size_frate);

  hbx_size_frate = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_size_frate);
  gtk_box_pack_start (GTK_BOX (vbx_size_frate), hbx_size_frate, TRUE, TRUE, 0);

  frm_size = gtk_frame_new (NULL);
  gtk_widget_show (frm_size);
  gtk_box_pack_start (GTK_BOX (hbx_size_frate), frm_size, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_size), 10);

  vbx_size = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_size);
  gtk_container_add (GTK_CONTAINER (frm_size), vbx_size);
  gtk_container_set_border_width (GTK_CONTAINER (vbx_size), 5);

  /* Bouton radio pour la taille */
  rdb_size = NULL;
  i = 0;
  while(i < philips->nbSize) {
    /* TODO trouver autre chose c'est pas bo */
    gchar str[256];
    GtkWidget *rdb;
    sprintf(str,"%dx%d",philips->tabSizeX[i],philips->tabSizeY[i]);
    rdb = gtk_radio_button_new_with_mnemonic (NULL, str);
    /* Hack, on test seulement si la largeur ou la longueur de la webcam correspont à 
       une taille prédéfinie */
    if((this->webcam->width == philips->tabSizeX[i]) || 
       (this->webcam->height == philips->tabSizeX[i])) {
      rdb_size = rdb;
      maxframerate = philips->tabMaxFrame[i];
    }
    gtk_widget_show (rdb);
    gtk_box_pack_start (GTK_BOX (vbx_size), rdb, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb), this->size);
    this->size = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb));
    g_signal_connect ((gpointer) rdb, "clicked",
		      G_CALLBACK (on_size_clicked),
		      this);
    ++i;
  }
  if(rdb_size == NULL) {
    rdb_size = this->size->data;
  }
  lbl_size = gtk_label_new ("Size");
  gtk_widget_show (lbl_size);
  gtk_frame_set_label_widget (GTK_FRAME (frm_size), lbl_size);
  /* Framerate */
  frm_frate = gtk_frame_new (NULL);
  gtk_widget_show (frm_frate);
  gtk_box_pack_start (GTK_BOX (hbx_size_frate), frm_frate, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_frate), 10);

  vbx_frate = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_frate);
  gtk_container_add (GTK_CONTAINER (frm_frate), vbx_frate);
  gtk_container_set_border_width (GTK_CONTAINER (vbx_frate), 10);

  spb_frate_adj = gtk_adjustment_new (this->webcam->framerate, 5, maxframerate, 1, 10, 10);
  this->framerate = gtk_spin_button_new (GTK_ADJUSTMENT (spb_frate_adj), 5, 0);
  gtk_widget_show (this->framerate);
  gtk_box_pack_start (GTK_BOX (vbx_frate), this->framerate, FALSE, FALSE, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (this->framerate), TRUE);

  lbl_frate = gtk_label_new ("Framerate");
  gtk_widget_show (lbl_frate);
  gtk_frame_set_label_widget (GTK_FRAME (frm_frate), lbl_frate);
  /* Debut des boutons orientation */
  frm_orientation = gtk_frame_new (NULL);
  gtk_widget_show (frm_orientation);
  gtk_box_pack_start (GTK_BOX (vbx_size_frate), frm_orientation, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_orientation), 10);

  vbx_orientation = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_orientation);
  gtk_container_add (GTK_CONTAINER (frm_orientation), vbx_orientation);

  rdb_landscape = gtk_radio_button_new_with_mnemonic (NULL, "Landscape");
  gtk_widget_show (rdb_landscape);
  gtk_box_pack_start (GTK_BOX (vbx_orientation), rdb_landscape, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_landscape), this->orientation);
  this->orientation = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_landscape));

  rdb_portrait = gtk_radio_button_new_with_mnemonic (NULL, "Portrait");
  gtk_widget_show (rdb_portrait);
  gtk_box_pack_start (GTK_BOX (vbx_orientation), rdb_portrait, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_portrait), this->orientation);
  this->orientation = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_portrait));

  lbl_orientation = gtk_label_new ("Orientation");
  gtk_widget_show (lbl_orientation);
  gtk_frame_set_label_widget (GTK_FRAME (frm_orientation), lbl_orientation);

  g_signal_connect ((gpointer) rdb_landscape, "clicked",
                    G_CALLBACK (on_orientation_clicked),
                    this);
  g_signal_connect ((gpointer) rdb_portrait, "clicked",
                    G_CALLBACK (on_orientation_clicked),
                    this);
  g_signal_connect ((gpointer) this->framerate, "value_changed",
                    G_CALLBACK (on_spb_frate_value_changed),
                    this);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rdb_size),TRUE);
  if(this->webcam->isLandscape(this->webcam)) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rdb_landscape),TRUE);
  }
  else {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rdb_portrait),TRUE);
  }
}

  

