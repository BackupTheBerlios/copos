/************************************************************************
* Fichier          : choosemodule.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#include "display/choosemodule.h"
/* a function which try to find the getWebcam function 
   in the given module */
static gboolean openModule(const gchar *device, GModule *module)
{
  /* Variables and pre-cond */
  gpointer func; /* temporary pointer */
  Webcam* (*getWebcam)(const gchar *device) = NULL; /* prototype of the func */
  Webcam *webcam;
  guchar *tampon; /* A buffer to test the webcam */

  g_return_val_if_fail((module != NULL), FALSE);
  g_return_val_if_fail((device != NULL), FALSE);
  /* Code */
  if(g_module_symbol(module, "getWebcam", &func) == FALSE) {
    g_warning(_("getWebcam is not provided in the module"));
    return FALSE;
  }
  getWebcam = func;
  /* Switch off the previous webcam */
  Global_setWebcam(NULL);
  webcam = getWebcam(device);
  g_return_val_if_fail((webcam != NULL), FALSE);
  if(! webcam->init(webcam)) {
    g_warning(_("Impossible to initialize the webcam"));
    return FALSE;
  }
  /* Test the webcam (grab a picture) */
  tampon = (guchar*) g_malloc(webcam->maximumSize(webcam));
  if(!webcam->getRGB(webcam,tampon)) {
    g_warning(_("Impossible to use the webcam"));
    return FALSE;
  }
  g_free(tampon);
  Global_setWebcam(webcam); /* Set the webcam global */
  return TRUE;
}

/* Open a file selector */
static void on_btn_choose_device_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *file_selector;
  ChooseModule* this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseModule*) user_data;
  
  /* Create the selector */
  file_selector = gtk_file_selection_new ("Please select a file");
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(file_selector),
				  gtk_entry_get_text(GTK_ENTRY(this->ent_device)));
  gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(file_selector));
  switch (gtk_dialog_run(GTK_DIALOG(file_selector))) {
  case GTK_RESPONSE_OK:
    /* Set the entry of the dialogue box */
    gtk_entry_set_text(GTK_ENTRY(this->ent_device), gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selector)));
    break;
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_NONE:
  default:
    break;
  }
  gtk_widget_destroy(file_selector);
}
 
/**
 * Constructor
 */
ChooseModule*  ChooseModule_new ()
{
  /* Variables and pre-cond */  
  ChooseModule* ret = (ChooseModule*) g_malloc(sizeof(ChooseModule));
  /* Code */
  ret->ent_device = NULL;
  ret->activeModule = -1;
  return ret;
}

  

/**
 * Destructor
 * @param *this the object to be destroy
 */
void  ChooseModule_destroy (ChooseModule *this)
{
  /* Variables and pre-cond */ 
  g_return_if_fail(this != NULL);
  /* Code */
  g_free(this);
}

  

/**
 * Open a dialog box then return the selected webcam or NULL
 * @param *this The object which return the webcam
 */
gboolean  ChooseModule_getWebcam (ChooseModule *this)
{
  /* Variables and pre-cond */
  Global *global = Global_get();
  GtkWidget *dlg_selec_dev;
  GtkWidget *dlg_vbx;
  GtkWidget *vbx_selec_dev;
  GtkWidget *tbl_selec_dev;
  GtkWidget *btn_choose_device;
  GtkWidget *alignment1;
  GtkWidget *hbox1;
  GtkWidget *image1;
  GtkWidget *label4;
  GtkWidget *lbl_device;
  GtkWidget *lbl_module;
  GtkWidget *cmb_module;
  GSList    *iter = NULL;
  gboolean  ret = FALSE;
  /* Code */

  g_return_val_if_fail((this != NULL), FALSE);

  dlg_selec_dev = gtk_dialog_new_with_buttons(_("Select the device"),
					      NULL,
					      GTK_DIALOG_MODAL,
					      GTK_STOCK_OK,GTK_RESPONSE_OK,
					      GTK_STOCK_CANCEL,
					      GTK_RESPONSE_CANCEL,NULL);
  dlg_vbx = GTK_DIALOG (dlg_selec_dev)->vbox;
  gtk_widget_show (dlg_vbx);

  vbx_selec_dev = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_selec_dev);
  gtk_box_pack_start (GTK_BOX (dlg_vbx), vbx_selec_dev, TRUE, TRUE, 0);

  tbl_selec_dev = gtk_table_new (2, 3, FALSE);
  gtk_widget_show (tbl_selec_dev);
  gtk_box_pack_start (GTK_BOX (vbx_selec_dev), tbl_selec_dev, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_selec_dev), 10);

  btn_choose_device = gtk_button_new ();
  gtk_widget_show (btn_choose_device);
  gtk_table_attach (GTK_TABLE (tbl_selec_dev), btn_choose_device, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (btn_choose_device), alignment1);

  hbox1 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox1);

  image1 = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (hbox1), image1, FALSE, FALSE, 0);

  label4 = gtk_label_new_with_mnemonic (_("Browse"));
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox1), label4, FALSE, FALSE, 0);

  lbl_device = gtk_label_new (_("Device : "));
  gtk_widget_show (lbl_device);
  gtk_table_attach (GTK_TABLE (tbl_selec_dev), lbl_device, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_device), 0, 0.5);

  lbl_module = gtk_label_new (_("Module : "));
  gtk_widget_show (lbl_module);
  gtk_table_attach (GTK_TABLE (tbl_selec_dev), lbl_module, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_module), 0, 0.5);

  this->ent_device = gtk_entry_new ();
  gtk_widget_show (this->ent_device);
  gtk_table_attach (GTK_TABLE (tbl_selec_dev), this->ent_device, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_text (GTK_ENTRY (this->ent_device), "/dev/video0");

  cmb_module = gtk_combo_box_new_text ();
  iter = global->modulesNames;
  while(iter != NULL) {
    gtk_combo_box_append_text(GTK_COMBO_BOX(cmb_module), iter->data);
    iter =g_slist_next(iter);
  }
  gtk_widget_show(cmb_module);
  gtk_table_attach (GTK_TABLE (tbl_selec_dev), cmb_module, 1, 3, 1, 2,
                    (GtkAttachOptions) (GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  g_signal_connect ((gpointer) btn_choose_device, "clicked",
                    G_CALLBACK (on_btn_choose_device_clicked),
                    this);
  switch (gtk_dialog_run(GTK_DIALOG(dlg_selec_dev))) {
    /* L utilisateur valide */
  case GTK_RESPONSE_OK:
    {
      GModule *module;
      this->activeModule = gtk_combo_box_get_active(GTK_COMBO_BOX(cmb_module));
      if(this->activeModule < 0) {
	Global_errMessage(_("Error during the selection"));
	ret = FALSE;
      }
      else {
	module = (GModule*) g_slist_nth_data(global->modules,this->activeModule);
	ret = openModule(gtk_entry_get_text(GTK_ENTRY(this->ent_device)), module);
      }
      break;
    }
    /* L utilisateur annule */
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_NONE:
  default:
    ret = FALSE;
    break;
  }
  /* Destruction de la boite de dialogue */
  gtk_widget_destroy(dlg_selec_dev); 
  return ret;
}

  

/**
 * Open a dialog box to configure the current webcam (Global)
 * @param *this The object which displays the configuration box
 */
void  ChooseModule_configureWebcam (ChooseModule *this)
{
  /* Variables and pre-cond */
  Global *global = Global_get();
  GModule *module;
  gpointer func;
  void  (*getGtkConf)(Webcam *webcam) = NULL;

  g_return_if_fail(this != NULL);
  g_return_if_fail(this->activeModule >= 0);
  /* Code */
  module = (GModule*) g_slist_nth_data(global->modules,this->activeModule);
  
  if(g_module_symbol(module, "getGtkConf", &func) == FALSE) {
    Global_errMessage(_("getGtkConf is not provided in the module"));
    return;
  }
  getGtkConf = func;
  getGtkConf(global->webcam);
}

  

