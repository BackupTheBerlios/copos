/************************************************************************
* Fichier          : confphilips.c
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#include "confphilips.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ConfPhilips"

void on_dialog_destroy(GtkObject       *object,
                       gpointer         user_data)
{
  ConfPhilips *conf;
  g_assert(user_data != NULL);
  g_assert(GTK_IS_DIALOG(object));

  conf = (ConfPhilips*) user_data;
  ConfPhilips_delete(conf);
}

/**
 * Construit et affiche la boite de dialogue de configuration
 * @param *webcam La webcam à modifier
 */
G_MODULE_EXPORT void  getGtkConf (Webcam *webcam)
{
  ConfPhilips *conf = ConfPhilips_new(webcam);
  ConfPhilips_presentation(conf);
}

  

/**
 * Constructeur
 * @param *webcam La webcam concerné par les modifications
 */
ConfPhilips*  ConfPhilips_new (Webcam *webcam)
{
  ConfPhilips* ret = (ConfPhilips*) g_malloc(sizeof(ConfPhilips));
  ret->webcam = webcam;
  ret->confSizeFrm = ConfSizeFrm_new(webcam);
  ret->confExtensions = ConfExtensions_new(webcam);
  return ret;
}

  

/**
 * Destructeur
 * @param *this L'objet à detruire
 */
void  ConfPhilips_delete (ConfPhilips *this)
{
  g_message("ConfPhilips_delete");
  ConfSizeFrm_delete(this->confSizeFrm);
  ConfExtensions_delete(this->confExtensions);
  g_free(this);
}

  

/**
 * Affichage de la boite de dialogue
 * @param *this L'objet à afficher
 */
void  ConfPhilips_presentation (ConfPhilips *this)
{
/*   g_message("ConfPhilips_presentation"); */
  GtkWidget *dialog;
  GtkWidget *vbx_main;
  GtkWidget *ntb_config;
  GtkWidget *lbl_size_frate;
  GtkWidget *lbl_vidControl;
  GtkWidget *lbl_extPhilips;
  GtkWidget *close_area;
  GtkWidget *btn_close;

  dialog = gtk_dialog_new ();
  gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
  gtk_window_set_title (GTK_WINDOW (dialog), "Configuration");
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
  gtk_window_set_modal(GTK_WINDOW (dialog), TRUE);
  vbx_main = GTK_DIALOG (dialog)->vbox;
  gtk_widget_show (vbx_main);

  ntb_config = gtk_notebook_new ();
  gtk_widget_show (ntb_config);
  gtk_box_pack_start (GTK_BOX (vbx_main), ntb_config, TRUE, TRUE, 0);

  /* Ajout des différentes pages du notebook */
  
  ConfSizeFrm_presentation(this->confSizeFrm, GTK_CONTAINER(ntb_config));
  
  lbl_size_frate = gtk_label_new ("Size & Framerate");
  gtk_widget_show (lbl_size_frate);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntb_config), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntb_config), 0), lbl_size_frate);
  
  ConfVidCtrl_presentation (this->webcam, GTK_CONTAINER(ntb_config));
  lbl_vidControl = gtk_label_new ("Video Controls");
  gtk_widget_show (lbl_vidControl);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntb_config), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntb_config), 1), lbl_vidControl);

  ConfExtensions_presentation(this->confExtensions, GTK_CONTAINER(ntb_config));
  lbl_extPhilips = gtk_label_new ("Philips Extensions");
  gtk_widget_show (lbl_extPhilips);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (ntb_config), gtk_notebook_get_nth_page (GTK_NOTEBOOK (ntb_config), 2), lbl_extPhilips);

  /* Zone d'action pour fermer la dialog */
  close_area = GTK_DIALOG (dialog)->action_area;
  gtk_widget_show (close_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (close_area), GTK_BUTTONBOX_END);

  btn_close = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (btn_close);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog), btn_close, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (btn_close, GTK_CAN_DEFAULT);
  gtk_widget_show (dialog);

  g_signal_connect ((gpointer) dialog, "destroy",
                    G_CALLBACK (on_dialog_destroy),
                    (gpointer) this);
  g_signal_connect_swapped(GTK_OBJECT (btn_close), 
			   "clicked", 
			   G_CALLBACK (gtk_widget_destroy),
			   GTK_OBJECT (dialog));
}
