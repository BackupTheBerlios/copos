/************************************************************************
* Fichier          : confextensions.c
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#include "confextensions.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ConfExtensions"

void on_chb_agc_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
  int arg;
  ConfExtensions *this;
  Webcam *webcam;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_TOGGLE_BUTTON(togglebutton));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;

/*   g_message("on_chb_agc_toggled\n"); */
  if(ioctl(webcam->device, VIDIOCPWCGAGC, &arg) < 0) {
    g_warning("Impossible de lire la configuration AGC\n");
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_agc), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(togglebutton), FALSE);
  }
    
  /* Si on active auto */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    arg = -arg;
    gtk_range_set_value(GTK_RANGE(this->hsc_agc),arg);
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_agc), FALSE);
    if(ioctl(webcam->device, VIDIOCPWCSAGC, &arg) < 0) {
      g_warning("Impossible d'ecrire la configuration AGC\n");
      gtk_widget_set_sensitive(GTK_WIDGET(togglebutton), FALSE);
    }
  }
  /* Sinon, la valeur est entré manuellement */
  else {
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_agc), TRUE);
    arg = abs(arg);
    gtk_range_set_value(GTK_RANGE(this->hsc_agc),arg);
  }
}



void on_chb_shuttler_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
  ConfExtensions *this;
  int arg;
  Webcam *webcam;
  Philips740K *philips;

  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_TOGGLE_BUTTON(togglebutton));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
  philips = (Philips740K*) webcam->data;

/*   g_message("on_chb_shuttler_toggled\n"); */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    philips->shuttler = -65535;
    arg = -65535;
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_shuttler), FALSE);
    gtk_range_set_value(GTK_RANGE(this->hsc_shuttler),arg);
    if(ioctl(webcam->device, VIDIOCPWCSSHUTTER, &arg) < 0) {
      g_warning("Impossible d'ecrire la configuration SHUTTLER\n");
    }
  }
  else {
    philips->shuttler = 0;
    arg = 0;
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_shuttler), TRUE);
    gtk_range_set_value(GTK_RANGE(this->hsc_shuttler),arg);
  }
}

void on_chb_contour_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
  ConfExtensions *this;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_TOGGLE_BUTTON(togglebutton));
  this = (ConfExtensions*) user_data;

/*   g_message("on_chb_contour_toggled\n"); */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    int arg = -65535;
    Webcam *webcam = this->webcam;
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_contour), FALSE);
    gtk_range_set_value(GTK_RANGE(this->hsc_contour),0);
    if(ioctl(webcam->device, VIDIOCPWCSCONTOUR, &arg) < 0) {
      g_warning("Impossible d'ecrire la configuration CONTOUR\n");
    }
  }
  else {
    gtk_range_set_value(GTK_RANGE(this->hsc_contour),0);
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_contour), TRUE);
  }
}


void on_hsc_agc_value_changed(GtkRange *range, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  int arg;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_RANGE(range));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
  arg = gtk_range_get_value(GTK_RANGE(range));
/*   g_message("on_hsc_agc_value_changed\n"); */
  if(ioctl(webcam->device, VIDIOCPWCSAGC, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration AGC\n");
  }
}

void on_hsc_shuttler_value_changed(GtkRange *range, gpointer user_data)
{
  ConfExtensions *this;
  int arg;
  Webcam *webcam;
  Philips740K *philips;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_RANGE(range));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
  philips = (Philips740K*) webcam->data;

/*   g_message("on_hsc_shuttler_value_changed\n"); */
  philips->shuttler = gtk_range_get_value(GTK_RANGE(range));
  arg = philips->shuttler;
  if(ioctl(webcam->device, VIDIOCPWCSSHUTTER, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration SHUTTLER\n");
  }
}

void on_hsc_contour_value_changed (GtkRange *range, gpointer user_data)
{
  ConfExtensions *this;
  int arg;
  Webcam *webcam;
    
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_RANGE(range));
  this = (ConfExtensions*) user_data;
  arg = gtk_range_get_value(GTK_RANGE(range));
  webcam = this->webcam;

/*   g_message("on_hsc_contour_value_changed %d\n",arg); */
  if(ioctl(webcam->device, VIDIOCPWCSCONTOUR, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration CONTOUR\n");
  }
}

void on_chb_blight_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  ConfExtensions *this;
  int arg;
  Webcam *webcam;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_TOGGLE_BUTTON(togglebutton));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;

/*   g_message("on_chb_blight_toggled\n"); */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    arg = 1;
  }
  else {
    arg = 0;
  }
  if(ioctl(webcam->device, VIDIOCPWCSBACKLIGHT , &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration VIDIOCPWCSBACKLIGHT\n");
  } 
}

void on_chb_fliker_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  ConfExtensions *this;
  int arg;
  Webcam *webcam;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_TOGGLE_BUTTON(togglebutton));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
/*   g_message("on_chb_fliker_toggled\n"); */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    arg = 1;
  }
  else {
    arg = 0;
  }
  if(ioctl(webcam->device, VIDIOCPWCSFLICKER, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration VIDIOCPWCSBACKLIGHT\n");
  }
}

void on_cmb_compression_changed(GtkComboBox *combobox, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  int arg;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_COMBO_BOX(combobox));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
/*   g_message("on_cmb_compression_changed\n"); */

  arg = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
  if(ioctl(webcam->device, VIDIOCPWCSCQUAL, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration compression\n");
  }
}

void on_cmb_noise_changed(GtkComboBox *combobox, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  int arg;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_COMBO_BOX(combobox));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
/*   g_message("on_cmb_noise_changed\n"); */

  arg = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
  if(ioctl(webcam->device, VIDIOCPWCSDYNNOISE, &arg) < 0) {
    g_warning("Impossible d'ecrire la configuration compression\n");
  }
}

void on_cmb_whiteb_changed(GtkComboBox *combobox, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  int arg;
  struct pwc_whitebalance whiteb;

  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_COMBO_BOX(combobox));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;

  arg = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
/*   g_message("on_cmb_whiteb_changed %d\n",arg); */

  if(ioctl(webcam->device, VIDIOCPWCGAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
  if(whiteb.mode == PWC_WB_MANUAL) {
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_red), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_blue), FALSE);
    gtk_range_set_value(GTK_RANGE(this->vsc_red),0);
    gtk_range_set_value(GTK_RANGE(this->vsc_blue),0);
  }

  switch(arg) {
  case 0 :
    whiteb.mode = PWC_WB_AUTO;
    break;
  case 1 :
    whiteb.mode = PWC_WB_INDOOR;
    break;
  case 2 :
    whiteb.mode = PWC_WB_OUTDOOR;
    break;
  case 3 :
    whiteb.mode = PWC_WB_FL;
    break;
  case 4 :
    whiteb.mode = PWC_WB_MANUAL;
    whiteb.manual_red = whiteb.read_red;
    whiteb.manual_blue = whiteb.read_blue;
    gtk_range_set_value(GTK_RANGE(this->vsc_red),arg);
    gtk_range_set_value(GTK_RANGE(this->vsc_blue),arg);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_red), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_blue), TRUE);
    break;
  }
  
  if(ioctl(webcam->device, VIDIOCPWCSAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
}

void on_vsc_red_value_changed (GtkRange *range, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  struct pwc_whitebalance whiteb;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_RANGE(range));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
/*   g_message("on_vsc_red_value_changed\n"); */

  if(ioctl(webcam->device, VIDIOCPWCGAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
  whiteb.manual_red = gtk_range_get_value(GTK_RANGE(range));
  
  if(ioctl(webcam->device, VIDIOCPWCSAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
}

void on_vsc_blue_value_changed (GtkRange *range, gpointer user_data)
{
  ConfExtensions *this;
  Webcam *webcam;
  struct pwc_whitebalance whiteb;
  g_return_if_fail (user_data != NULL);
  g_return_if_fail (GTK_IS_RANGE(range));
  this = (ConfExtensions*) user_data;
  webcam = this->webcam;
/*   g_message("on_vsc_blue_value_changed\n"); */

  if(ioctl(webcam->device, VIDIOCPWCGAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
  whiteb.manual_blue = gtk_range_get_value(GTK_RANGE(range));
  if(ioctl(webcam->device, VIDIOCPWCSAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
  }
}

/**
 * Constructeur
 * @param *webcam La webcam à modifier
 */
ConfExtensions*  ConfExtensions_new (Webcam *webcam)
{
  ConfExtensions * ret = (ConfExtensions*) g_malloc(sizeof(ConfExtensions));
  g_return_val_if_fail((webcam != NULL), NULL);
  ret->webcam = webcam;
  return ret;
}

  

/**
 * Destructeur
 * @param *this L'objet à detruire
 */
void  ConfExtensions_delete (ConfExtensions *this)
{
  g_free(this);
}

  

/**
 * Affiche les widgets
 * @param *this L'objet
 * @param *parent Le parent où seront affiché les widgets
 */
void  ConfExtensions_presentation (ConfExtensions *this, GtkContainer *parent)
{
  GtkWidget *hbx_extPhilips;
  GtkWidget *vbox_exp_algo;
  GtkWidget *frm_exposure;
  GtkWidget *vbx_exposure;
  GtkWidget *tbl_exposure;
  GtkWidget *lbl_agc;
  GtkWidget *chb_agc;
  GtkWidget *lbl_shuttle;
  GtkWidget *lbl_contour;
  GtkWidget *chb_shuttler;
  GtkWidget *chb_contour;
  GtkWidget *hbx_blight_flicer;
  GtkWidget *chb_blight;
  GtkWidget *chb_fliker;
  GtkWidget *lbl_exposure;
  GtkWidget *frm_algorithms;
  GtkWidget *tbl_algorithms;
  GtkWidget *cmb_compression;
  GtkWidget *cmb_noise;
  GtkWidget *lbl_noise;
  GtkWidget *lbl_compression;
  GtkWidget *lbl_algorithms;
  GtkWidget *frm_whitebalance;
  GtkWidget *vbx_whiteb;
  GtkWidget *cmb_whiteb;
  GtkWidget *hbx_red_blue;
  GtkWidget *frm_red;
  GtkWidget *lbl_red;
  GtkWidget *frm_blue;
  GtkWidget *lbl_blue;
  GtkWidget *lbl_whiteb;
  int arg;
  struct pwc_whitebalance whiteb;
  Webcam* webcam = this->webcam;
  Philips740K* philips = (Philips740K*) webcam->data; 
  
  hbx_extPhilips = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_extPhilips);
  gtk_container_add (GTK_CONTAINER (parent), hbx_extPhilips);

  vbox_exp_algo = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox_exp_algo);
  gtk_box_pack_start (GTK_BOX (hbx_extPhilips), vbox_exp_algo, TRUE, TRUE, 0);

  frm_exposure = gtk_frame_new (NULL);
  gtk_widget_show (frm_exposure);
  gtk_box_pack_start (GTK_BOX (vbox_exp_algo), frm_exposure, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_exposure), 10);

  vbx_exposure = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_exposure);
  gtk_container_add (GTK_CONTAINER (frm_exposure), vbx_exposure);

  tbl_exposure = gtk_table_new (3, 3, FALSE);
  gtk_widget_show (tbl_exposure);
  gtk_box_pack_start (GTK_BOX (vbx_exposure), tbl_exposure, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_exposure), 10);

  lbl_agc = gtk_label_new ("AGC");
  gtk_widget_show (lbl_agc);
  gtk_table_attach (GTK_TABLE (tbl_exposure), lbl_agc, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_agc), 0, 0.5);

  chb_agc = gtk_check_button_new_with_mnemonic ("auto");
  gtk_widget_show (chb_agc);
  gtk_table_attach (GTK_TABLE (tbl_exposure), chb_agc, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  lbl_shuttle = gtk_label_new ("Shuttler");
  gtk_widget_show (lbl_shuttle);
  gtk_table_attach (GTK_TABLE (tbl_exposure), lbl_shuttle, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_shuttle), 0, 0.5);

  lbl_contour = gtk_label_new ("Contour");
  gtk_widget_show (lbl_contour);
  gtk_table_attach (GTK_TABLE (tbl_exposure), lbl_contour, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_contour), 0, 0.5);

  chb_shuttler = gtk_check_button_new_with_mnemonic ("auto");
  gtk_widget_show (chb_shuttler);
  gtk_table_attach (GTK_TABLE (tbl_exposure), chb_shuttler, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  chb_contour = gtk_check_button_new_with_mnemonic ("auto");
  gtk_widget_show (chb_contour);
  gtk_table_attach (GTK_TABLE (tbl_exposure), chb_contour, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  this->hsc_agc = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 65535, 1, 0, 0)));
  gtk_widget_show (this->hsc_agc);
  gtk_table_attach (GTK_TABLE (tbl_exposure), this->hsc_agc, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_agc), 0);

  this->hsc_shuttler = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 65535, 1, 0, 0)));
  gtk_widget_show (this->hsc_shuttler);
  gtk_table_attach (GTK_TABLE (tbl_exposure), this->hsc_shuttler, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_shuttler), 0);

  this->hsc_contour = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 65535, 1, 10, 0)));
  gtk_widget_show (this->hsc_contour);
  gtk_table_attach (GTK_TABLE (tbl_exposure), this->hsc_contour, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_contour), 0);

  hbx_blight_flicer = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbx_blight_flicer);
  gtk_box_pack_end (GTK_BOX (vbx_exposure), hbx_blight_flicer, TRUE, FALSE, 0);

  chb_blight = gtk_check_button_new_with_mnemonic ("Backlight comp.");
  gtk_widget_show (chb_blight);
  gtk_box_pack_start (GTK_BOX (hbx_blight_flicer), chb_blight, FALSE, FALSE, 0);

  chb_fliker = gtk_check_button_new_with_mnemonic ("Flickerless mode");
  gtk_widget_show (chb_fliker);
  gtk_box_pack_start (GTK_BOX (hbx_blight_flicer), chb_fliker, FALSE, FALSE, 0);

  lbl_exposure = gtk_label_new ("Exposure");
  gtk_widget_show (lbl_exposure);
  gtk_frame_set_label_widget (GTK_FRAME (frm_exposure), lbl_exposure);

  frm_algorithms = gtk_frame_new (NULL);
  gtk_widget_show (frm_algorithms);
  gtk_box_pack_start (GTK_BOX (vbox_exp_algo), frm_algorithms, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_algorithms), 10);

  tbl_algorithms = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_algorithms);
  gtk_container_add (GTK_CONTAINER (frm_algorithms), tbl_algorithms);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_algorithms), 10);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_algorithms), 10);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_algorithms), 10);
  /* Menu Compression */
  cmb_compression = gtk_combo_box_new_text ();
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_compression),"None");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_compression),"Low");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_compression),"Medium");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_compression),"High");
  gtk_widget_show (cmb_compression);
  gtk_table_attach (GTK_TABLE (tbl_algorithms), cmb_compression, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  /* Menu Noise */
  cmb_noise = gtk_combo_box_new_text ();
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_noise),"None");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_noise),"Low");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_noise),"Medium");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_noise),"High");
  gtk_widget_show (cmb_noise);
  gtk_table_attach (GTK_TABLE (tbl_algorithms), cmb_noise, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  lbl_noise = gtk_label_new ("Noise Reduction");
  gtk_widget_show (lbl_noise);
  gtk_table_attach (GTK_TABLE (tbl_algorithms), lbl_noise, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_noise), 0, 0.5);

  lbl_compression = gtk_label_new ("Preferred Compression");
  gtk_widget_show (lbl_compression);
  gtk_table_attach (GTK_TABLE (tbl_algorithms), lbl_compression, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_compression), 0, 0.5);

  lbl_algorithms = gtk_label_new ("Algorithms");
  gtk_widget_show (lbl_algorithms);
  gtk_frame_set_label_widget (GTK_FRAME (frm_algorithms), lbl_algorithms);

  frm_whitebalance = gtk_frame_new (NULL);
  gtk_widget_show (frm_whitebalance);
  gtk_box_pack_start (GTK_BOX (hbx_extPhilips), frm_whitebalance, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_whitebalance), 10);

  vbx_whiteb = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_whiteb);
  gtk_container_add (GTK_CONTAINER (frm_whitebalance), vbx_whiteb);

  cmb_whiteb = gtk_combo_box_new_text ();
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_whiteb),"Auto");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_whiteb),"Indoor");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_whiteb),"Outdoor");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_whiteb),"Fluorescent Lighting");
  gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_whiteb),"Manual");
  gtk_widget_show (cmb_whiteb);

  gtk_box_pack_start (GTK_BOX (vbx_whiteb), cmb_whiteb, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (cmb_whiteb), 5);

  hbx_red_blue = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_red_blue);
  gtk_box_pack_start (GTK_BOX (vbx_whiteb), hbx_red_blue, TRUE, TRUE, 0);

  frm_red = gtk_frame_new (NULL);
  gtk_widget_show (frm_red);
  gtk_box_pack_start (GTK_BOX (hbx_red_blue), frm_red, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_red), 5);

  this->vsc_red = gtk_vscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 65535, 1, 10, 0)));
  gtk_widget_show (this->vsc_red);
  gtk_container_add (GTK_CONTAINER (frm_red), this->vsc_red);
  gtk_scale_set_digits (GTK_SCALE (this->vsc_red), 0);
  gtk_range_set_inverted (GTK_RANGE (this->vsc_red), TRUE);

  lbl_red = gtk_label_new ("Rouge");
  gtk_widget_show (lbl_red);
  gtk_frame_set_label_widget (GTK_FRAME (frm_red), lbl_red);

  frm_blue = gtk_frame_new (NULL);
  gtk_widget_show (frm_blue);
  gtk_box_pack_start (GTK_BOX (hbx_red_blue), frm_blue, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_blue), 5);

  this->vsc_blue = gtk_vscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 65535, 1, 10, 0)));
  gtk_widget_show (this->vsc_blue);
  gtk_container_add (GTK_CONTAINER (frm_blue), this->vsc_blue);
  gtk_scale_set_digits (GTK_SCALE (this->vsc_blue), 0);
  gtk_range_set_inverted (GTK_RANGE (this->vsc_blue), TRUE);

  lbl_blue = gtk_label_new ("Bleu");
  gtk_widget_show (lbl_blue);
  gtk_frame_set_label_widget (GTK_FRAME (frm_blue), lbl_blue);

  lbl_whiteb = gtk_label_new ("White Balance");
  gtk_widget_show (lbl_whiteb);
  gtk_frame_set_label_widget (GTK_FRAME (frm_whitebalance), lbl_whiteb);


  /* Initialisation */
  if(ioctl(webcam->device, VIDIOCPWCGAGC, &arg) < 0) {
    g_warning("Impossible de lire la configuration AGC\n");
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_agc), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(chb_agc), FALSE);
  }
  else {
    if(arg < 0) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_agc),TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_agc), FALSE);
    }
    else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_agc),FALSE);
      gtk_range_set_value(GTK_RANGE(this->hsc_agc),arg);
    }
  }
  if(philips->shuttler < 0) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_shuttler),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_shuttler), FALSE);
  }
  else {
    gtk_range_set_value(GTK_RANGE(this->hsc_shuttler),philips->shuttler);
  }
  if(ioctl(webcam->device, VIDIOCPWCGCONTOUR, &arg) < 0) {
    g_warning("Impossible de lire la configuration CONTOUR\n");
    gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_contour), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(chb_contour), FALSE);
  }
  else {
    if(arg < 0) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_contour),TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(this->hsc_contour), FALSE);
    }
    else {
      gtk_range_set_value(GTK_RANGE(this->hsc_contour),arg);
    }
  }
  if(ioctl(webcam->device, VIDIOCPWCGBACKLIGHT, &arg) < 0) {
    g_warning("Impossible de lire la configuration BACKLIGHT\n");
    gtk_widget_set_sensitive(GTK_WIDGET(chb_blight), FALSE);
  }
  else {
    if(arg == 0) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_blight),FALSE);
    }
    else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_blight),TRUE);
    }
  }
  if(ioctl(webcam->device, VIDIOCPWCGFLICKER, &arg) < 0) {
    g_warning("Impossible de lire la configuration FLICKER\n");
    gtk_widget_set_sensitive(GTK_WIDGET(chb_fliker), FALSE);
  }
  else {
    if(arg == 0) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_fliker),FALSE);
    }
    else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chb_fliker),TRUE);
    }
  }
  if(ioctl(webcam->device, VIDIOCPWCGDYNNOISE, &arg) < 0) {
    g_warning("Impossible de lire la configuration DYNNOISE\n");
    gtk_widget_set_sensitive(GTK_WIDGET(cmb_noise), FALSE);
  }
  else {
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_noise),arg);
  }
  if(ioctl(webcam->device, VIDIOCPWCGCQUAL, &arg) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
    gtk_widget_set_sensitive(GTK_WIDGET(cmb_compression), FALSE);
  }
  else {
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_compression),arg);
  }
  if(ioctl(webcam->device, VIDIOCPWCGAWB, &whiteb) < 0) {
    g_warning("Impossible de lire la configuration Compression QUAL\n");
    gtk_widget_set_sensitive(GTK_WIDGET(cmb_whiteb), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_red), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_blue), FALSE);
  }
  else {
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_red), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_blue), FALSE);
    switch(whiteb.mode) {
    case PWC_WB_AUTO :
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_whiteb),0);
      break;
    case PWC_WB_INDOOR :
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_whiteb),1);
      break;
    case PWC_WB_OUTDOOR :
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_whiteb),2);
      break;
    case PWC_WB_FL :
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_whiteb),3);
      break;
    case PWC_WB_MANUAL :
      gtk_combo_box_set_active(GTK_COMBO_BOX(cmb_whiteb),4);
      gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_red), TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(this->vsc_blue), TRUE);
      break;
    }
  }
  
  g_signal_connect ((gpointer) chb_agc, "toggled",
                    G_CALLBACK (on_chb_agc_toggled),
                    this);
  g_signal_connect ((gpointer) chb_shuttler, "toggled",
                    G_CALLBACK (on_chb_shuttler_toggled),
                    this);
  g_signal_connect ((gpointer) chb_contour, "toggled",
                    G_CALLBACK (on_chb_contour_toggled),
                    this);
  g_signal_connect ((gpointer) this->hsc_agc, "value_changed",
                    G_CALLBACK (on_hsc_agc_value_changed),
                    this);
  g_signal_connect ((gpointer) this->hsc_shuttler, "value_changed",
                    G_CALLBACK (on_hsc_shuttler_value_changed),
                    this);
  g_signal_connect ((gpointer) this->hsc_contour, "value_changed",
                    G_CALLBACK (on_hsc_contour_value_changed),
                    this);
  g_signal_connect ((gpointer) chb_blight, "toggled",
                    G_CALLBACK (on_chb_blight_toggled),
                    this);
  g_signal_connect ((gpointer) chb_fliker, "toggled",
                    G_CALLBACK (on_chb_fliker_toggled),
                    this);
  g_signal_connect ((gpointer) cmb_compression, "changed",
                    G_CALLBACK (on_cmb_compression_changed),
                    this);
  g_signal_connect ((gpointer) cmb_noise, "changed",
                    G_CALLBACK (on_cmb_noise_changed),
                    this);
  g_signal_connect ((gpointer) cmb_whiteb, "changed",
                    G_CALLBACK (on_cmb_whiteb_changed),
                    this);
  g_signal_connect ((gpointer) this->vsc_red, "value_changed",
                    G_CALLBACK (on_vsc_red_value_changed),
                    this);
  g_signal_connect ((gpointer) this->vsc_blue, "value_changed",
                    G_CALLBACK (on_vsc_blue_value_changed),
                    this);


}
