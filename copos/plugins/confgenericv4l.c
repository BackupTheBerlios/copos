/************************************************************************
* Fichier          : confgenericv4l.c
* Date de Creation : sam aoû 21 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam aoû 21 2004 at 15:13:05 with umbrello
**************************************************************************/

#include "confgenericv4l.h"
static void
on_dlg_confgeneric_destroy             (GtkObject       *object,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  ConfGenericV4L *this;
  g_return_if_fail(GTK_IS_DIALOG(object));
  g_return_if_fail(user_data != NULL);
  this = (ConfGenericV4L*) user_data;
  /* Code */
  ConfGenericV4L_destroy(this);
}


static void
on_btn_apply_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  guint   width, height;
  gint palette = 0;
  ConfGenericV4L *this;
  GenericV4L *genericv4l;
  struct video_picture    vpicture;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ConfGenericV4L*) user_data;
  genericv4l = (GenericV4L*) this->webcam->data;
  /* Code */
  width = (guint) g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_width)),NULL);
  height = (guint) g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_height)),NULL);
  if((width != 0) && (height != 0)) {
    this->webcam->width = width;
    this->webcam->height = height;
    GenericV4L_update(this->webcam);
  }
  else {
    g_warning("Enter correct values");
    return;
  }
  palette = gtk_combo_box_get_active(GTK_COMBO_BOX(this->cmb_palette));
  if(ioctl(this->webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Couldn't get picture informations from Video4Linux");
    return;
  }
  switch(palette) {
  case 0 :
    vpicture.palette = VIDEO_PALETTE_YUV420;
    genericv4l->vmap.format = VIDEO_PALETTE_YUV420P;
    break;
  case 1 :
    vpicture.palette = VIDEO_PALETTE_RGB24;
    genericv4l->vmap.format = VIDEO_PALETTE_RGB24;
    break;
  }
  if(ioctl(this->webcam->device, VIDIOCSPICT, &vpicture) < 0) {
    g_warning("Couldn't write the picture configuration\n");
    return;
  }
}


/**
 * Construct and display the dialog box of configuration
 * @param *webcam The webcam to be modified
 */
G_MODULE_EXPORT void  getGtkConf (Webcam *webcam)
{
  /* Variables and pre-cond */
  g_return_if_fail(webcam != NULL);
  /* Code */
  ConfGenericV4L *conf = ConfGenericV4L_new(webcam);
  ConfGenericV4L_presentation(conf);
}

/**
 * Constructor
 * @param *webcam The webcam to be modified
 */
ConfGenericV4L*  ConfGenericV4L_new (Webcam *webcam)
{
  /* Variables and pre-cond */
  ConfGenericV4L* ret = (ConfGenericV4L*) g_malloc(sizeof(ConfGenericV4L));
  g_return_val_if_fail(webcam != NULL, NULL);
  /* Code */
  ret->webcam = webcam;
  ret->ent_width = NULL;
  ret->ent_height = NULL;
  ret->cmb_palette = NULL;
 
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  ConfGenericV4L_destroy (ConfGenericV4L *this)
{
  /* Variables and pre-cond */
  /* Code */
  g_free(this);
}

  

/**
 * Display a dialog box for configuration
 * @param *this The object to be displayed
 */
void  ConfGenericV4L_presentation (ConfGenericV4L *this)
{
  /* Variables and pre-cond */
  GtkWidget *dlg_confgeneric;
  GtkWidget *vbx_main;
  GtkWidget *vbx_size_palette;
  GtkWidget *frm_size;
  GtkWidget *tbl_size;
/*   GtkWidget *ent_width; */
/*   GtkWidget *ent_height; */
  GtkWidget *lbl_height;
  GtkWidget *lbl_width;
  GtkWidget *lbl_size;
  GtkWidget *frm_palette;
/*   GtkWidget *cmb_palette; */
  GtkWidget *lbl_palette;
  GtkWidget *hbb_action_area;
  GtkWidget *btn_apply;
  GtkWidget *btn_close;
  char str[256];
  /* Code */

  dlg_confgeneric = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dlg_confgeneric), "Configuration");
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dlg_confgeneric), TRUE);
  gtk_window_set_modal(GTK_WINDOW (dlg_confgeneric), TRUE);
  
  vbx_main = GTK_DIALOG (dlg_confgeneric)->vbox;
  gtk_widget_show (vbx_main);

  vbx_size_palette = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_size_palette);
  gtk_box_pack_start (GTK_BOX (vbx_main), vbx_size_palette, TRUE, TRUE, 0);

  frm_size = gtk_frame_new (NULL);
  gtk_widget_show (frm_size);
  gtk_box_pack_start (GTK_BOX (vbx_size_palette), frm_size, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_size), 10);

  tbl_size = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_size);
  gtk_container_add (GTK_CONTAINER (frm_size), tbl_size);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_size), 10);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_size), 10);

  this->ent_width = gtk_entry_new ();
  sprintf(str,"%d",this->webcam->width);
  gtk_entry_set_text(GTK_ENTRY(this->ent_width), str);
  gtk_widget_show (this->ent_width);
  gtk_table_attach (GTK_TABLE (tbl_size), this->ent_width, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  this->ent_height = gtk_entry_new ();
  sprintf(str,"%d",this->webcam->height);
  gtk_entry_set_text(GTK_ENTRY(this->ent_height), str);
  gtk_widget_show (this->ent_height);
  gtk_table_attach (GTK_TABLE (tbl_size), this->ent_height, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  lbl_height = gtk_label_new ("Height : ");
  gtk_widget_show (lbl_height);
  gtk_table_attach (GTK_TABLE (tbl_size), lbl_height, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_height), 0, 0.5);

  lbl_width = gtk_label_new ("Width : ");
  gtk_widget_show (lbl_width);
  gtk_table_attach (GTK_TABLE (tbl_size), lbl_width, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_width), 0, 0.5);

  lbl_size = gtk_label_new ("Size");
  gtk_widget_show (lbl_size);
  gtk_frame_set_label_widget (GTK_FRAME (frm_size), lbl_size);

  frm_palette = gtk_frame_new (NULL);
  gtk_widget_show (frm_palette);
  gtk_box_pack_start (GTK_BOX (vbx_size_palette), frm_palette, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_palette), 10);

  this->cmb_palette = gtk_combo_box_new_text ();
  gtk_combo_box_append_text(GTK_COMBO_BOX(this->cmb_palette),"YUV420");
  gtk_combo_box_append_text(GTK_COMBO_BOX(this->cmb_palette),"RGB24");
  gtk_combo_box_set_active(GTK_COMBO_BOX(this->cmb_palette),0);
  gtk_container_add (GTK_CONTAINER (frm_palette), this->cmb_palette);
  gtk_widget_show (this->cmb_palette);

  lbl_palette = gtk_label_new ("Palette");
  gtk_widget_show (lbl_palette);
  gtk_frame_set_label_widget (GTK_FRAME (frm_palette), lbl_palette);

  hbb_action_area = GTK_DIALOG (dlg_confgeneric)->action_area;
  gtk_widget_show (hbb_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_action_area), GTK_BUTTONBOX_END);

  btn_apply = gtk_button_new_from_stock ("gtk-apply");
  gtk_widget_show (btn_apply);
  gtk_dialog_add_action_widget (GTK_DIALOG (dlg_confgeneric), btn_apply, GTK_RESPONSE_APPLY);
  GTK_WIDGET_SET_FLAGS (btn_apply, GTK_CAN_DEFAULT);

  btn_close = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (btn_close);
  gtk_dialog_add_action_widget (GTK_DIALOG (dlg_confgeneric), btn_close, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (btn_close, GTK_CAN_DEFAULT);

  gtk_widget_show (dlg_confgeneric);

  g_signal_connect ((gpointer) dlg_confgeneric, "destroy",
                    G_CALLBACK (on_dlg_confgeneric_destroy),
                    this);
  g_signal_connect ((gpointer) btn_apply, "clicked",
                    G_CALLBACK (on_btn_apply_clicked),
                    this);
  g_signal_connect_swapped ((gpointer) btn_close, "clicked",
                            G_CALLBACK (gtk_widget_destroy),
                            GTK_OBJECT (dlg_confgeneric));
}
