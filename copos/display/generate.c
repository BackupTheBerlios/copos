/************************************************************************
* Fichier          : generate.c
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#include "display/generate.h"

static void
update_image(Generate *this, GdkPixbuf *pixbuf)
{
  guint height = gdk_pixbuf_get_height(pixbuf);
  guint i;
  g_return_if_fail(this != NULL);
  g_return_if_fail(pixbuf != NULL);

  for(i=0;i<height;++i) {
    Global_putPixel(pixbuf, this->rotationCenterPixel, i, 255, 255, 255);
  }
}

static void
on_btn_firstImg_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *pixbuf = NULL;
  Generate *this;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_if_fail(storage != NULL);
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  if(! Storage_isLoaded(storage)) {
    g_return_if_fail(Storage_load(storage));
  }
  Storage_seek(storage,0);
  pixbuf = Global_getPixbufFromStorage();
  update_image(this, pixbuf);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->img_generate), pixbuf);
  g_object_unref(pixbuf);
}


static void
on_btn_nextImg_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *pixbuf = NULL;
  Generate *this;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_if_fail(storage != NULL);
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  if(! Storage_isLoaded(storage)) {
    g_return_if_fail(Storage_load(storage));
  }
  pixbuf = Global_getPixbufFromStorage();
  if(pixbuf == NULL) { /* if at the end of file rewind */
    Global *global = Global_get();
    Storage *storage = global->storage;
    g_return_if_fail(storage != NULL);
    Storage_seek(storage,0);
    pixbuf = Global_getPixbufFromStorage();
  }
  update_image(this, pixbuf);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->img_generate), pixbuf);
  g_object_unref(pixbuf);
}

static void
on_rdb_rotation_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Generate *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  gtk_entry_set_text(GTK_ENTRY(this->ent_rotation), "0.0");
  gtk_entry_set_text(GTK_ENTRY(this->ent_translation), "0.0");
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_rotation), TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_translation), FALSE);
}


static void
on_rdb_translation_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Generate *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  gtk_entry_set_text(GTK_ENTRY(this->ent_rotation), "0.0");
  gtk_entry_set_text(GTK_ENTRY(this->ent_translation), "0.0");
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_rotation), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_translation), TRUE);
}

static void
on_btn_cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Generate *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  this->interrupt = TRUE;
}

static void
showDialog(Generate *this, Collector *collector)
{
  /* Variables and pre-cond */
  GtkWidget *dlg_generate;
  GtkWidget *vbx_generate;
  GtkWidget *vbx_dlgGenerate;
  GtkWidget *lbl_state;
  GtkWidget *prb_state;
  GtkWidget *hbb_cancel;
  GtkWidget *btn_cancel;
  gboolean next = TRUE;
  guint cpt=0;
  Global *global = Global_get();
  /* Code */
  dlg_generate = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dlg_generate), _("Computation"));
  gtk_window_set_type_hint (GTK_WINDOW (dlg_generate), GDK_WINDOW_TYPE_HINT_DIALOG);

  vbx_generate = GTK_DIALOG (dlg_generate)->vbox;
  gtk_widget_show (vbx_generate);

  vbx_dlgGenerate = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_dlgGenerate);
  gtk_box_pack_start (GTK_BOX (vbx_generate), vbx_dlgGenerate, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbx_dlgGenerate), 20);

  lbl_state = gtk_label_new (_("Analyse each picture"));
  gtk_widget_show (lbl_state);
  gtk_box_pack_start (GTK_BOX (vbx_dlgGenerate), lbl_state, FALSE, FALSE, 10);

  prb_state = gtk_progress_bar_new ();
  gtk_widget_show (prb_state);
  gtk_box_pack_start (GTK_BOX (vbx_dlgGenerate), prb_state, FALSE, FALSE, 0);

  hbb_cancel = GTK_DIALOG (dlg_generate)->action_area;
  gtk_widget_show (hbb_cancel);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_cancel), GTK_BUTTONBOX_END);

  btn_cancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (btn_cancel);
  gtk_dialog_add_action_widget (GTK_DIALOG (dlg_generate), btn_cancel, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (btn_cancel, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) btn_cancel, "clicked",
                    G_CALLBACK (on_btn_cancel_clicked),
                    this);
  gtk_widget_show (dlg_generate);

  while((next) && (!this->interrupt)) {
    if(cpt <= global->storage->nbImages) {
      gdouble fraction = (gdouble)cpt / (gdouble) global->storage->nbImages;
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prb_state), fraction);
    }
    next = Collector_iteration(collector);

    while (g_main_context_iteration(NULL, FALSE));
    ++cpt;
  }
  if(this->interrupt) {
    this->interrupt = FALSE;
  }
  gtk_widget_destroy (dlg_generate);
}

static void
on_btn_apply_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  G_CONST_RETURN gchar* str;
  gdouble rotation;
  gdouble translation;
  Generate *this;
  Collector *collector;
  Global *global = Global_get();
  g_return_if_fail(global->storage != NULL);
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  
  /* Code */
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_rotation));
  rotation = g_strtod(str,NULL);
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_translation));
  translation = g_strtod(str,NULL);
  if((rotation == 0.0) && (translation == 0.0)) {
    Global_errMessage(_("Please, add a value greater than 0.0"));
    return;
  }
  collector = Collector_new();
  Collector_begin(collector, global->imageAnalyse, global->perspectiveCreator, global->storage);

  showDialog(this, collector);

  if(rotation != 0.0) {
    Point3D *center = Point3D_new(0.0,0.0,this->rotationCenterReal);
    Collector_endRotation(collector, center, rotation);
  }
  else {
    Collector_endTranslation (collector, translation);
  }

  Global_setCollector(collector);
  if((this->visualization != NULL) && Visualization_isDisplayed(this->visualization)) {
    Visualization_destroy(this->visualization);
    this->visualization = NULL;
  }
  this->visualization = Visualization_new();
  Visualization_presentation(this->visualization);
}

/**
 * Constructor
 */
Generate*  Generate_new ()
{
  /* Variables and pre-cond */
  Global *global = Global_get();
  Point3D *pt3D;
  Point2D *pt2D;
  Generate* ret = (Generate*) g_malloc(sizeof(Generate));  
  g_return_val_if_fail(global->perspectiveCreator != NULL, NULL);
  /* Code */
  ret->mainWidget = NULL;
  ret->img_generate = NULL;
  ret->ent_rotation = NULL;
  ret->ent_translation = NULL;
  ret->interrupt = FALSE;
  ret->visualization = NULL;

  pt3D = Point3D_new(0.0, 0.0, global->perspectiveCreator->distCenter);
  pt2D = PerspectiveCreator_realToPixel(global->perspectiveCreator, pt3D);
  ret->rotationCenterPixel = pt2D->x;
  ret->rotationCenterReal = global->perspectiveCreator->distCenter;
  Point2D_destroy(pt2D);
  Point3D_destroy(pt3D);
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  Generate_destroy (Generate *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(Generate_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if((this->visualization != NULL) && Visualization_isDisplayed(this->visualization)) {
    Visualization_destroy(this->visualization);
    this->visualization = NULL;
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  Generate_isDisplayed (Generate *this)
{
  /* Variables and pre-cond */
  /* Code */
  if(this == NULL) {
    return FALSE;
  }
  if(this->mainWidget != NULL) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * It displays all the widgets in the global container (see Global)
 * @param *this The object to be displayed
 */
void  Generate_presentation (Generate *this)
{
  /* Variables and pre-cond */
  GtkWidget *scw_generate;
  GtkWidget *vwp_generate;
  GtkWidget *hbb_player;
  GtkWidget *btn_firstImg;
  GtkWidget *img_firstImg;
  GtkWidget *btn_nextImg;
  GtkWidget *img_nextImg;
  GtkWidget *lbl_information;
  GtkWidget *hsp_infos;
  GtkWidget *tbl_generate;
  GtkWidget *rdb_rotation;
  GSList *rdb_rotation_group = NULL;
  GtkWidget *rdb_translation;
  GtkWidget *lbl_sp1;
  GtkWidget *lbl_sp2;
  GtkWidget *hsp_apply;
  GtkWidget *hbb_apply;
  GtkWidget *btn_apply;
  GdkPixbuf *pixbuf;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  g_return_if_fail(global->storage != NULL);
  /* Code */

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);
 
  scw_generate = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scw_generate);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), scw_generate, TRUE, TRUE, 0);

  vwp_generate = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (vwp_generate);
  gtk_container_add (GTK_CONTAINER (scw_generate), vwp_generate);


  pixbuf = Global_getPixbufFromStorage();
  if(pixbuf == NULL) { /* if at the end of file rewind */
    Storage_seek(global->storage,0);
    pixbuf = Global_getPixbufFromStorage();
  }
  update_image(this, pixbuf);
  this->img_generate = gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  gtk_widget_show (this->img_generate);
  gtk_container_add (GTK_CONTAINER (vwp_generate), this->img_generate);

  hbb_player = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_player);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_player, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_player), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_player), GTK_BUTTONBOX_START);

  btn_firstImg = gtk_button_new ();
  gtk_widget_show (btn_firstImg);
  gtk_container_add (GTK_CONTAINER (hbb_player), btn_firstImg);
  GTK_WIDGET_SET_FLAGS (btn_firstImg, GTK_CAN_DEFAULT);

  img_firstImg = gtk_image_new_from_stock ("gtk-goto-first", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_firstImg);
  gtk_container_add (GTK_CONTAINER (btn_firstImg), img_firstImg);

  btn_nextImg = gtk_button_new ();
  gtk_widget_show (btn_nextImg);
  gtk_container_add (GTK_CONTAINER (hbb_player), btn_nextImg);
  GTK_WIDGET_SET_FLAGS (btn_nextImg, GTK_CAN_DEFAULT);

  img_nextImg = gtk_image_new_from_stock ("gtk-go-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_nextImg);
  gtk_container_add (GTK_CONTAINER (btn_nextImg), img_nextImg);

  lbl_information = gtk_label_new (_("<b>Check</b> that the centre of rotation (symbolized by the white line)\nis correctly positioned. \nIf it is not the case, start again the preceding stage."));
  gtk_widget_show (lbl_information);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), lbl_information, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (lbl_information), TRUE);
  gtk_label_set_justify (GTK_LABEL (lbl_information), GTK_JUSTIFY_CENTER);
  gtk_misc_set_padding (GTK_MISC (lbl_information), 0, 3);

  hsp_infos = gtk_hseparator_new ();
  gtk_widget_show (hsp_infos);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_infos, FALSE, TRUE, 0);

  tbl_generate = gtk_table_new (2, 3, FALSE);
  gtk_widget_show (tbl_generate);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), tbl_generate, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_generate), 5);

  rdb_rotation = gtk_radio_button_new_with_mnemonic (NULL, _("Rotation of the object (in degree)"));
  gtk_widget_show (rdb_rotation);
  gtk_table_attach (GTK_TABLE (tbl_generate), rdb_rotation, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_rotation), rdb_rotation_group);
  rdb_rotation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_rotation));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rdb_rotation), TRUE);

  rdb_translation = gtk_radio_button_new_with_mnemonic (NULL, _("Translation of the object (in centimeter)"));
  gtk_widget_show (rdb_translation);
  gtk_table_attach (GTK_TABLE (tbl_generate), rdb_translation, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_translation), rdb_rotation_group);
  rdb_rotation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_translation));

  lbl_sp1 = gtk_label_new (_(" : "));
  gtk_widget_show (lbl_sp1);
  gtk_table_attach (GTK_TABLE (tbl_generate), lbl_sp1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp1), 0, 0.5);

  lbl_sp2 = gtk_label_new (_(" : "));
  gtk_widget_show (lbl_sp2);
  gtk_table_attach (GTK_TABLE (tbl_generate), lbl_sp2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp2), 0, 0.5);

  this->ent_rotation = gtk_entry_new ();
  gtk_widget_show (this->ent_rotation);
  gtk_table_attach (GTK_TABLE (tbl_generate), this->ent_rotation, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_text (GTK_ENTRY (this->ent_rotation), _("0.0"));

  this->ent_translation = gtk_entry_new ();
  gtk_widget_show (this->ent_translation);
  gtk_table_attach (GTK_TABLE (tbl_generate), this->ent_translation, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_text (GTK_ENTRY (this->ent_translation), _("0.0"));
  gtk_widget_set_sensitive (this->ent_translation, FALSE);

  hsp_apply = gtk_hseparator_new ();
  gtk_widget_show (hsp_apply);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_apply, FALSE, TRUE, 0);

  hbb_apply = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_apply);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_apply, FALSE, TRUE, 0);

  btn_apply = gtk_button_new_from_stock ("gtk-apply");
  gtk_widget_show (btn_apply);
  gtk_container_add (GTK_CONTAINER (hbb_apply), btn_apply);
  gtk_container_set_border_width (GTK_CONTAINER (btn_apply), 5);
  GTK_WIDGET_SET_FLAGS (btn_apply, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) btn_firstImg, "clicked",
                    G_CALLBACK (on_btn_firstImg_clicked),
                    this);
  g_signal_connect ((gpointer) btn_nextImg, "clicked",
                    G_CALLBACK (on_btn_nextImg_clicked),
                    this);
  g_signal_connect ((gpointer) rdb_rotation, "clicked",
                    G_CALLBACK (on_rdb_rotation_clicked),
                    this);
  g_signal_connect ((gpointer) rdb_translation, "clicked",
                    G_CALLBACK (on_rdb_translation_clicked),
                    this);
  g_signal_connect ((gpointer) btn_apply, "clicked",
                    G_CALLBACK (on_btn_apply_clicked),
                    this);

  Global_setLabel(_("Generate"));
}

  

