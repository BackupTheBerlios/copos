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

static GdkPixbuf*
get_image(Generate *this, gboolean next)
{
  guint width;
  guint height;
  guint bytesPerPixel;
  guchar *tampon;
  GdkPixbuf* pixbuf;
  gboolean success;
  g_return_val_if_fail(Storage_isOpen(this->storage), NULL);
  
  Storage_getInfos(this->storage, &width, &height, &bytesPerPixel);
  tampon = (guchar*) g_malloc(sizeof(guchar)*width*height*bytesPerPixel);
  if(next) {
    success = Storage_get(this->storage,tampon);
  }
  else {
    success = Storage_getReverse(this->storage,tampon);
  }
  if(!success) {
    Storage_close(this->storage);
    g_free(tampon);
    return NULL;
  }
  pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			  FALSE,
			  8,
			  width,
                          height);
  Global_gucharToGdkPixbuf(tampon,pixbuf);
  g_free(tampon);
  return pixbuf;
}

static void
on_btn_previous_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *pixbuf = NULL;
  Generate *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  if(! Storage_isOpen(this->storage)) {
    Storage_readEnd(this->storage);
  }
  pixbuf = get_image(this, FALSE);
  if(pixbuf != NULL) {
    update_image(this, pixbuf);
    gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), pixbuf);
    g_object_unref(pixbuf);
  }
}

static void
on_btn_next_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
    /* Variables and pre-cond */
  GdkPixbuf *pixbuf = NULL;
  Generate *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  /* Code */
  if(! Storage_isOpen(this->storage)) {
    Storage_readBegin(this->storage);
  }
  pixbuf = get_image(this, TRUE);
  if(pixbuf != NULL) {
    update_image(this, pixbuf);
    gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), pixbuf);
    g_object_unref(pixbuf);
  }
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
  Storage_close(this->storage); /* to stop the computation we close the storage object */
}

/* This create a dialogbox with a progress bar and a label for informations */
static GtkWidget*
create_dlg_generate (Generate *this, GtkWidget **prb_info, GtkWidget **lbl_info)
{
  GtkWidget *dlg_generate;
  GtkWidget *vbx_main;
  GtkWidget *vbx_progress;
  GtkWidget *hbb_action_area;
  GtkWidget *btn_cancel;

  dlg_generate = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dlg_generate), _("Analyze"));

  vbx_main = GTK_DIALOG (dlg_generate)->vbox;
  gtk_widget_show (vbx_main);

  vbx_progress = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_progress);
  gtk_box_pack_start (GTK_BOX (vbx_main), vbx_progress, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbx_progress), 20);

  *lbl_info = gtk_label_new (_("Analyze each image"));
  gtk_widget_show (*lbl_info);
  gtk_box_pack_start (GTK_BOX (vbx_progress), *lbl_info, TRUE, FALSE, 0);

  *prb_info = gtk_progress_bar_new ();
  gtk_widget_show (*prb_info);
  gtk_box_pack_start (GTK_BOX (vbx_progress), *prb_info, TRUE, FALSE, 0);
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (*prb_info), 1);

  hbb_action_area = GTK_DIALOG (dlg_generate)->action_area;
  gtk_widget_show (hbb_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_action_area), GTK_BUTTONBOX_END);

  btn_cancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (btn_cancel);
  gtk_dialog_add_action_widget (GTK_DIALOG (dlg_generate), btn_cancel, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (btn_cancel, GTK_CAN_DEFAULT);
  g_signal_connect ((gpointer) btn_cancel, "clicked",
                    G_CALLBACK (on_btn_cancel_clicked),
                    this);
  return dlg_generate;
}


static void
on_btn_generate_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GtkWidget *dlg_generate;
  GtkWidget *lbl_info;
  GtkWidget *prb_info;
  Generate *this;
  gdouble rotation;
  gdouble translation;
  G_CONST_RETURN gchar* str;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Generate*) user_data;
  Global *global = Global_get();
  gboolean next = TRUE;
  Collector *collector;
  guint cpt=0;
  /* Code */
  dlg_generate = create_dlg_generate(this, &prb_info, &lbl_info);
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prb_info), 0.0);

  str = gtk_entry_get_text(GTK_ENTRY(this->ent_rotation));
  rotation = g_strtod(str,NULL);
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_translation));
  translation = g_strtod(str,NULL);
  if((rotation == 0.0) && (translation == 0.0)) {
    Global_errMessage(_("Please, add a value greater than 0.0"));
    return;
  }
  /* Test if this a rotation or a translation */
  collector = Collector_new();
  if(rotation != 0.0) {
    Point3D *center = Point3D_new(0.0,0.0,this->rotationCenterReal);
    Collector_beginRot (collector, center, rotation, this->storage);
    center = NULL;
  }
  else {
    Collector_beginTrans (collector, translation, this->storage);
  }

  /* Start the analyze of each image */
  gtk_widget_show (dlg_generate);
  gtk_grab_add(dlg_generate);
  /* test if there's another image and if the storage object is always open */
  while(next && Storage_isOpen(this->storage)) {
    gdouble fraction = (gdouble)cpt / (gdouble) this->storage->nbImages;
    
    next = Collector_iteration(collector, 
			       global->imageAnalyse, 
			       global->perspectiveCreator);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prb_info), fraction);
    ++cpt;
    /* Display the progress bar */
    gtk_main_iteration ();
  }
  gtk_label_set_text(GTK_LABEL(lbl_info),_("Normalize the points. That can take time (and freeze the application)"));
  gtk_main_iteration (); /* Update the display */
  Collector_end(collector);
  gtk_grab_remove(dlg_generate);
  gtk_widget_destroy(dlg_generate);
  /* End of the Analyze */
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
 * @param center Center of the table (distance between laser and table)
 */
Generate*  Generate_new (gdouble center)
{
  /* Variables and pre-cond */
  guint width;
  guint height;
  Point2D *pt2D = NULL;
  Point3D *pt3D = Point3D_new(0.0, 0.0, center);
  Global *global = Global_get();
  Generate* ret = (Generate*) g_malloc(sizeof(Generate));
  g_return_val_if_fail(global->perspectiveCreator != NULL, NULL);
  g_return_val_if_fail(global->storage != NULL, NULL);
  
  /* Code */
  ret->mainWidget = NULL;
  ret->image = NULL;
  ret->ent_rotation = NULL;
  ret->ent_translation = NULL;
  ret->storage = global->storage;
  ret->visualization = NULL;
  /* Get the information about the image of the webcam via storage */
  Storage_readBegin(ret->storage);
  Storage_getInfos(ret->storage, &width, &height, NULL);
  Storage_close(ret->storage);

  (global->perspectiveCreator)->CCDwidth = width;
  (global->perspectiveCreator)->CCDheight = height;
  pt2D =  PerspectiveCreator_realToPixel(global->perspectiveCreator, pt3D);
  g_return_val_if_fail(pt2D != NULL, NULL);

  ret->rotationCenterPixel = pt2D->x;
  ret->rotationCenterReal = center;

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
/*   GtkWidget *mainWidget; */
  GtkWidget *scw_image;
  GtkWidget *vwp_image;
/*   GtkWidget *image; */
  GtkWidget *lbl_warning;
  GtkWidget *hbx_navigate;
  GtkWidget *img_first;
  GtkWidget *btn_previous;
  GtkWidget *img_back;
  GtkWidget *lbl_info;
  GtkWidget *btn_next;
  GtkWidget *img_forward;
  GtkWidget *img_last;
  GtkWidget *tbl_rot_trans;
  GtkWidget *rdb_rotation;
  GSList *rdb_rotation_group = NULL;
  GtkWidget *rdb_translation;
/*   GtkWidget *ent_rotation; */
/*   GtkWidget *ent_translation; */ 
  GtkWidget *hbb_generate;
  GtkWidget *btn_generate;
  GtkWidget *alignment2;
  GtkWidget *hbox5;
  GtkWidget *img_ok;
  GtkWidget *lbl_generate;
  GdkPixbuf *pixbuf;
  GdkPixbuf *new;
  guint width, dest_width;
  guint height, dest_height;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  g_return_if_fail(this->storage != NULL);
  /* Code */

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  scw_image = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scw_image);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), scw_image, TRUE, TRUE, 0);

  vwp_image = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (vwp_image);
  gtk_container_add (GTK_CONTAINER (scw_image), vwp_image);

  Storage_readBegin(this->storage);
  pixbuf = get_image(this, TRUE);
  /* Compute the size of the thumb image */
  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);
  dest_height = 50;
  dest_width = (guint) ((gdouble) width * (gdouble) dest_height / (gdouble) height);

  update_image(this,pixbuf);
  this->image = gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  gtk_widget_show (this->image);
  gtk_container_add (GTK_CONTAINER (vwp_image), this->image);

  lbl_warning = gtk_label_new (_("Check that the centre of rotation (symbolized by the white line) is correctly positioned. \nIf it is not the case, start again the preceding stage. "));
  gtk_widget_show (lbl_warning);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), lbl_warning, FALSE, FALSE, 0);
  gtk_misc_set_padding (GTK_MISC (lbl_warning), 10, 10);

  hbx_navigate = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_navigate);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_navigate, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbx_navigate), 5);

  Storage_readBegin(this->storage);
  pixbuf = get_image(this, TRUE);  
  new = gdk_pixbuf_scale_simple(pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);
  img_first = gtk_image_new_from_pixbuf(new);
  g_object_unref(new);
  g_object_unref(pixbuf);
  gtk_widget_show (img_first);
  gtk_box_pack_start (GTK_BOX (hbx_navigate), img_first, TRUE, TRUE, 0);

  btn_previous = gtk_button_new ();
  gtk_widget_show (btn_previous);
  gtk_box_pack_start (GTK_BOX (hbx_navigate), btn_previous, FALSE, FALSE, 0);

  img_back = gtk_image_new_from_stock ("gtk-go-back", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_back);
  gtk_container_add (GTK_CONTAINER (btn_previous), img_back);

  lbl_info = gtk_label_new (_("Navigate"));
  gtk_widget_show (lbl_info);
  gtk_box_pack_start (GTK_BOX (hbx_navigate), lbl_info, FALSE, FALSE, 0);
  gtk_misc_set_padding (GTK_MISC (lbl_info), 10, 10);

  btn_next = gtk_button_new ();
  gtk_widget_show (btn_next);
  gtk_box_pack_start (GTK_BOX (hbx_navigate), btn_next, FALSE, FALSE, 0);

  img_forward = gtk_image_new_from_stock ("gtk-go-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_forward);
  gtk_container_add (GTK_CONTAINER (btn_next), img_forward);

  Storage_readEnd(this->storage);
  pixbuf = get_image(this, FALSE);
  new = gdk_pixbuf_scale_simple(pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);
  img_last = gtk_image_new_from_pixbuf(new);
  g_object_unref(new);
  g_object_unref(pixbuf);
  gtk_widget_show (img_last);
  gtk_box_pack_start (GTK_BOX (hbx_navigate), img_last, TRUE, TRUE, 0);

  tbl_rot_trans = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_rot_trans);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), tbl_rot_trans, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_rot_trans), 5);

  rdb_rotation = gtk_radio_button_new_with_mnemonic (NULL, _("Rotation of the object (in degree)"));
  gtk_widget_show (rdb_rotation);
  gtk_table_attach (GTK_TABLE (tbl_rot_trans), rdb_rotation, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_rotation), rdb_rotation_group);
  rdb_rotation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_rotation));

  rdb_translation = gtk_radio_button_new_with_mnemonic (NULL, _("Translation of the object"));
  gtk_widget_show (rdb_translation);
  gtk_table_attach (GTK_TABLE (tbl_rot_trans), rdb_translation, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rdb_translation), rdb_rotation_group);
  rdb_rotation_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rdb_translation));

  this->ent_rotation = gtk_entry_new ();
  gtk_widget_show (this->ent_rotation);
  gtk_table_attach (GTK_TABLE (tbl_rot_trans), this->ent_rotation, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  this->ent_translation = gtk_entry_new ();
  gtk_widget_show (this->ent_translation);
  gtk_table_attach (GTK_TABLE (tbl_rot_trans), this->ent_translation, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  hbb_generate = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_generate);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_generate, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_generate), 5);

  btn_generate = gtk_button_new ();
  gtk_widget_show (btn_generate);
  gtk_container_add (GTK_CONTAINER (hbb_generate), btn_generate);
  GTK_WIDGET_SET_FLAGS (btn_generate, GTK_CAN_DEFAULT);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (btn_generate), alignment2);

  hbox5 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox5);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox5);

  img_ok = gtk_image_new_from_stock ("gtk-ok", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (img_ok);
  gtk_box_pack_start (GTK_BOX (hbox5), img_ok, FALSE, FALSE, 0);

  lbl_generate = gtk_label_new_with_mnemonic (_("Generate"));
  gtk_widget_show (lbl_generate);
  gtk_box_pack_start (GTK_BOX (hbox5), lbl_generate, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) btn_previous, "clicked",
                    G_CALLBACK (on_btn_previous_clicked),
                    this);
  g_signal_connect ((gpointer) btn_next, "clicked",
                    G_CALLBACK (on_btn_next_clicked),
		    this);
  g_signal_connect ((gpointer) rdb_rotation, "clicked",
                    G_CALLBACK (on_rdb_rotation_clicked),
                    this);
  g_signal_connect ((gpointer) rdb_translation, "clicked",
                    G_CALLBACK (on_rdb_translation_clicked),
                    this);
  g_signal_connect ((gpointer) btn_generate, "clicked",
                    G_CALLBACK (on_btn_generate_clicked),
                    this);

  /* Restart the reading from the begining */
  Storage_readBegin(this->storage);
  gtk_entry_set_text(GTK_ENTRY(this->ent_rotation), "0.0");
  gtk_entry_set_text(GTK_ENTRY(this->ent_translation), "0.0");
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_rotation), TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(this->ent_translation), FALSE);
  Global_setLabel(_("Generate"));

  gtk_widget_grab_focus (this->ent_rotation);
}

  

