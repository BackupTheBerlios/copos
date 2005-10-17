#include "display/choosefile.h"


static gboolean load  (ChooseFile* this)
{
  /* Variables and pre-cond */
  G_CONST_RETURN gchar* filename;
  /* Code */
  filename = gtk_entry_get_text    (GTK_ENTRY(this->ent_browse));

  Storage *storage = Storage_new();
  Storage_setFileName(storage, filename);
  storage->hasChangedOrientation = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->chb_orientation));

  if(Storage_load(storage)) {
    guint width;
    guint height;
    gchar str[256];
    Storage_getInfos (storage, &width, &height, NULL, str);
    gtk_label_set_text(GTK_LABEL(this->lbl_video), str);
    snprintf(str, 255, "%dx%d", width, height);
    gtk_label_set_text(GTK_LABEL(this->lbl_size), str);
    snprintf(str, 255, "%d", storage->nbImages);
    gtk_label_set_text(GTK_LABEL(this->lbl_nbImg), str);
    Global_setStorage(storage);
    return TRUE;
  }
  else {
    gtk_label_set_text(GTK_LABEL(this->lbl_video), "");
    gtk_label_set_text(GTK_LABEL(this->lbl_size), "");
    gtk_label_set_text(GTK_LABEL(this->lbl_nbImg), "");
    Global_setStorage(NULL);
    return FALSE;
  }
}

static void
on_btn_forward_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  ChooseFile *this;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_if_fail(storage != NULL);
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseFile*) user_data;
  /* Code */
  storage->hasChangedOrientation = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->chb_orientation));
  if(PickLaser_isDisplayed(this->pickLaser)) {
    PickLaser_destroy(this->pickLaser);
  }
  this->pickLaser = PickLaser_new();
  PickLaser_presentation(this->pickLaser);
}


static void
on_ent_browse_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{
/* Variables and pre-cond */
  ChooseFile *this;
  g_return_if_fail(GTK_IS_EDITABLE(editable));
  g_return_if_fail(user_data != NULL);
  this = (ChooseFile*) user_data;
  /* Code */
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_forward), FALSE);
  if(load(this)) {
    gtk_widget_set_sensitive(GTK_WIDGET(this->btn_forward), TRUE);
  }
}


static void
on_btn_browse_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GtkWidget *dialog;
  ChooseFile *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseFile*) user_data;
  /* Code */
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_forward), FALSE);
  dialog = gtk_file_chooser_dialog_new (_("Open File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					NULL);
  
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    gtk_entry_set_text (GTK_ENTRY(this->ent_browse), filename);    
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
  if(load(this)) {
    gtk_widget_set_sensitive(GTK_WIDGET(this->btn_forward), TRUE);
  }
}

/**
 * Constructor
 */
ChooseFile*  ChooseFile_new ()
{
  /* Variables and pre-cond */
  ChooseFile* ret = (ChooseFile*) g_malloc(sizeof(ChooseFile));
  /* Code */
  ret->mainWidget = NULL;
  ret->lbl_video = NULL;
  ret->lbl_size = NULL;
  ret->lbl_nbImg = NULL;
  ret->ent_browse = NULL;
  ret->chb_orientation = NULL;
  ret->btn_forward = NULL;
  ret->pickLaser = NULL;
  ret->btn_forward = NULL;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroy
 */
void  ChooseFile_destroy (ChooseFile *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(ChooseFile_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if(this->pickLaser != 0) {
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  ChooseFile_isDisplayed (ChooseFile *this)
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
void  ChooseFile_presentation (ChooseFile *this)
{
  /* Variables and pre-cond */
  GtkWidget *frm_choose;
  GtkWidget *alg_choose;
  GtkWidget *tbl_choose;
  GtkWidget *btn_browse;
  GtkWidget *lbl_choose;
  GtkWidget *frm_informations;
  GtkWidget *alg_informations;
  GtkWidget *tbl_informations;
  GtkWidget *lbl_sizeInfos;
  GtkWidget *lbl_nbImgInfos;
  GtkWidget *lbl_sp3;
  GtkWidget *lbl_sp1;
  GtkWidget *lbl_videoInfos;
  GtkWidget *lbl_sp2;
  GtkWidget *lbl_videoInformations;
  GtkWidget *frm_orientation;
  GtkWidget *alg_orientation;
  GtkWidget *lbl_orientation;
  GtkWidget *hsp_chooseFile;
  GtkWidget *lbl_void;
  GtkWidget *hbb_forward;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));

  /* Code */
  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  frm_choose = gtk_frame_new (NULL);
  gtk_widget_show (frm_choose);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), frm_choose, FALSE, TRUE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (frm_choose), 15);

  alg_choose = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alg_choose);
  gtk_container_add (GTK_CONTAINER (frm_choose), alg_choose);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alg_choose), 0, 0, 12, 0);

  tbl_choose = gtk_table_new (1, 2, FALSE);
  gtk_widget_show (tbl_choose);
  gtk_container_add (GTK_CONTAINER (alg_choose), tbl_choose);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_choose), 5);

  this->ent_browse = gtk_entry_new ();
  gtk_widget_show (this->ent_browse);
  gtk_table_attach (GTK_TABLE (tbl_choose), this->ent_browse, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);

  btn_browse = gtk_button_new_with_mnemonic (_("Browse..."));
  gtk_widget_show (btn_browse);
  gtk_table_attach (GTK_TABLE (tbl_choose), btn_browse, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  lbl_choose = gtk_label_new (_("<b>Choose video file</b>"));
  gtk_widget_show (lbl_choose);
  gtk_frame_set_label_widget (GTK_FRAME (frm_choose), lbl_choose);
  gtk_label_set_use_markup (GTK_LABEL (lbl_choose), TRUE);

  frm_informations = gtk_frame_new (NULL);
  gtk_widget_show (frm_informations);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), frm_informations, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_informations), 15);

  alg_informations = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alg_informations);
  gtk_container_add (GTK_CONTAINER (frm_informations), alg_informations);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alg_informations), 0, 0, 12, 0);

  tbl_informations = gtk_table_new (3, 3, TRUE);
  gtk_widget_show (tbl_informations);
  gtk_container_add (GTK_CONTAINER (alg_informations), tbl_informations);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_informations), 5);
  gtk_table_set_row_spacings (GTK_TABLE (tbl_informations), 5);
  gtk_table_set_col_spacings (GTK_TABLE (tbl_informations), 5);

  lbl_sizeInfos = gtk_label_new (_("Size"));
  gtk_widget_show (lbl_sizeInfos);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_sizeInfos, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sizeInfos), 0, 0.5);

  lbl_nbImgInfos = gtk_label_new (_("Number of images"));
  gtk_widget_show (lbl_nbImgInfos);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_nbImgInfos, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_nbImgInfos), 0, 0.5);

  lbl_sp3 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp3);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_sp3, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp3), 0, 0.5);

  this->lbl_size = gtk_label_new ("");
  gtk_widget_show (this->lbl_size);
  gtk_table_attach (GTK_TABLE (tbl_informations), this->lbl_size, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (this->lbl_size), 0, 0.5);

  this->lbl_nbImg = gtk_label_new ("");
  gtk_widget_show (this->lbl_nbImg);
  gtk_table_attach (GTK_TABLE (tbl_informations), this->lbl_nbImg, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (this->lbl_nbImg), 0, 0.5);

  lbl_sp1 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp1);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_sp1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp1), 0, 0.5);

  lbl_videoInfos = gtk_label_new (_("Video"));
  gtk_widget_show (lbl_videoInfos);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_videoInfos, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_videoInfos), 0, 0.5);

  this->lbl_video = gtk_label_new ("");
  gtk_widget_show (this->lbl_video);
  gtk_table_attach (GTK_TABLE (tbl_informations), this->lbl_video, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (this->lbl_video), 0, 0.5);

  lbl_sp2 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp2);
  gtk_table_attach (GTK_TABLE (tbl_informations), lbl_sp2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp2), 0, 0.5);

  lbl_videoInformations = gtk_label_new (_("<b>Video informations</b>"));
  gtk_widget_show (lbl_videoInformations);
  gtk_frame_set_label_widget (GTK_FRAME (frm_informations), lbl_videoInformations);
  gtk_label_set_use_markup (GTK_LABEL (lbl_videoInformations), TRUE);

  frm_orientation = gtk_frame_new (NULL);
  gtk_widget_show (frm_orientation);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), frm_orientation, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frm_orientation), 15);

  alg_orientation = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alg_orientation);
  gtk_container_add (GTK_CONTAINER (frm_orientation), alg_orientation);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alg_orientation), 0, 0, 12, 0);

  this->chb_orientation = gtk_check_button_new_with_mnemonic (_("Change the orientation"));
  gtk_widget_show (this->chb_orientation);
  gtk_container_add (GTK_CONTAINER (alg_orientation), this->chb_orientation);
  gtk_container_set_border_width (GTK_CONTAINER (this->chb_orientation), 5);

  lbl_orientation = gtk_label_new (_("<b>Orientation</b>"));
  gtk_widget_show (lbl_orientation);
  gtk_frame_set_label_widget (GTK_FRAME (frm_orientation), lbl_orientation);
  gtk_label_set_use_markup (GTK_LABEL (lbl_orientation), TRUE);

  lbl_void = gtk_label_new ("");
  gtk_widget_show (lbl_void);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), lbl_void, TRUE, FALSE, 0);

  hsp_chooseFile = gtk_hseparator_new ();
  gtk_widget_show (hsp_chooseFile);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_chooseFile, FALSE, TRUE, 0);

  hbb_forward = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_forward);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_forward, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_forward), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_forward), GTK_BUTTONBOX_END);

  this->btn_forward = gtk_button_new_from_stock ("gtk-go-forward");
  gtk_widget_show (this->btn_forward);
  gtk_container_add (GTK_CONTAINER (hbb_forward), this->btn_forward);
  GTK_WIDGET_SET_FLAGS (this->btn_forward, GTK_CAN_DEFAULT);
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_forward), FALSE);

  g_signal_connect ((gpointer) this->ent_browse, "changed",
                    G_CALLBACK (on_ent_browse_changed),
                    this);
  g_signal_connect ((gpointer) btn_browse, "clicked",
                    G_CALLBACK (on_btn_browse_clicked),
                    this);
  g_signal_connect ((gpointer) this->btn_forward, "clicked",
                    G_CALLBACK (on_btn_forward_clicked),
                    this);

  Global_setLabel(_("Choose File"));
}

  

