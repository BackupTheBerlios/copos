/************************************************************************
* Fichier          : choosedevice.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#include "display/choosedevice.h"

/* A function to load periodicaly the picture from the webcam
   and refresh the drawing area */
static gboolean 
update_image (gpointer data)
{
  /* Variables and pre-cond */
  gint x;
  gint y;
  gint width;
  gint height;
  ChooseDevice* this;
  Global *global = Global_get();
  Webcam *webcam = global->webcam;
  g_return_val_if_fail(webcam != NULL, FALSE);
  g_return_val_if_fail(data != NULL, FALSE);
  this = (ChooseDevice*) data;
  g_return_val_if_fail(this->tampon != NULL, FALSE);
  g_return_val_if_fail(this->dra_chooseDevice != NULL, FALSE);
  /* Code */
  webcam->getRGB(webcam,this->tampon);
  gdk_window_get_geometry(this->dra_chooseDevice->window,
			  &x,
			  &y,
			  &width,
			  &height,
			  NULL);
  /* Refresh the area */
  gtk_widget_queue_draw_area(this->dra_chooseDevice,
			     0,
			     0,
			     width,
			     height);
  return TRUE;
}


static gboolean
on_dra_chooseDevice_expose_event       (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Global *global = Global_get();
  Webcam *webcam = global->webcam;
  if(webcam != NULL) {
    ChooseDevice* this;
    g_return_val_if_fail(event != NULL, FALSE);
    g_return_val_if_fail(user_data != NULL, FALSE);
    this = (ChooseDevice*) user_data;
    g_return_val_if_fail(this->tampon != NULL, FALSE);
    g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
    if(this->tag != 0) { /* Refresh if the periodic function is started */
      gdk_draw_rgb_image(widget->window,
			 widget->style->white_gc,
			 0,
			 0,
			 webcam->width,
			 webcam->height,
			 GDK_RGB_DITHER_NORMAL,
			 this->tampon,
			 webcam->width*3);
    }
  }
  return FALSE;
}


static void
on_btn_device_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Global *global;
  ChooseDevice* this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseDevice*) user_data;
  global = Global_get();
  /* Code */

  if(Recorder_isDisplayed(this->recorder)) {
    Recorder_destroy(this->recorder);
    this->recorder = NULL;
  }

  /* Stop the previous display function */
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function update_image"));
  }
  this->tag = 0;
  if(! ChooseModule_getWebcam(this->chooseModule)) {
    g_warning(_("Beware no webcam selected"));
    return;
  }
  if(this->tampon != NULL) {
    g_free(this->tampon);
  }
  this->tampon = (guchar*) g_malloc((global->webcam)->maximumSize(global->webcam));
  this->tag = g_timeout_add(33, update_image, this);
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_next), TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_configuration), TRUE);
}


static void
on_btn_configuration_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{  
  /* Variables and pre-cond */
  ChooseDevice* this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseDevice*) user_data;
  g_return_if_fail(this->chooseModule != NULL);
  /* Code */
  ChooseModule_configureWebcam(this->chooseModule);
}


static void
on_btn_next_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  ChooseDevice *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (ChooseDevice*) user_data;
  /* Code */
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function update_image"));
  }
  this->tag = 0;
  if(Recorder_isDisplayed(this->recorder)) {
    Recorder_destroy(this->recorder);
  }
  this->recorder = Recorder_new();
  Recorder_presentation(this->recorder);
}

/**
 * Constructor
 */
ChooseDevice*  ChooseDevice_new ()
{
  /* Variables and pre-cond */
  /* Code */
  ChooseDevice* ret = (ChooseDevice*) g_malloc(sizeof(ChooseDevice));
  ret->chooseModule = ChooseModule_new();
  ret->mainWidget = NULL;
  ret->btn_configuration = NULL;
  ret->btn_next = NULL;
  ret->dra_chooseDevice = NULL;
  ret->tampon = NULL;
  ret->tag = 0;
  ret->recorder = NULL;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  ChooseDevice_destroy (ChooseDevice *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  /* Code */
  if(ChooseDevice_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function update_image"));
  }
  this->tag = 0;
  g_free(this->tampon);
  Global_setWebcam(NULL);
  if(Recorder_isDisplayed(this->recorder)) {
    Recorder_destroy(this->recorder);
    this->recorder = NULL;
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 * 
 */
gboolean  ChooseDevice_isDisplayed (ChooseDevice *this)
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
void  ChooseDevice_presentation (ChooseDevice *this)
{
  /* Variables and pre-cond */
  GtkWidget *hbb_chooseDevice;
  GtkWidget *btn_device;
  GtkWidget *hsp_chooseDevice;
  GtkWidget *hbb_next;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  /* Code */

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  this->dra_chooseDevice = gtk_drawing_area_new ();
  gtk_widget_show (this->dra_chooseDevice);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), this->dra_chooseDevice, TRUE, TRUE, 0);

  hbb_chooseDevice = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_chooseDevice);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_chooseDevice, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_chooseDevice), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_chooseDevice), GTK_BUTTONBOX_END);
  gtk_box_set_spacing (GTK_BOX (hbb_chooseDevice), 10);

  btn_device = gtk_button_new_with_mnemonic (_("Device..."));
  gtk_widget_show (btn_device);
  gtk_container_add (GTK_CONTAINER (hbb_chooseDevice), btn_device);
  GTK_WIDGET_SET_FLAGS (btn_device, GTK_CAN_DEFAULT);

  this->btn_configuration = gtk_button_new_with_mnemonic (_("Configuration..."));
  gtk_widget_show (this->btn_configuration);
  gtk_container_add (GTK_CONTAINER (hbb_chooseDevice), this->btn_configuration);
  GTK_WIDGET_SET_FLAGS (this->btn_configuration, GTK_CAN_DEFAULT);

  hsp_chooseDevice = gtk_hseparator_new ();
  gtk_widget_show (hsp_chooseDevice);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_chooseDevice, FALSE, FALSE, 0);

  hbb_next = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_next);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_next, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_next), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_next), GTK_BUTTONBOX_END);

  this->btn_next = gtk_button_new_with_mnemonic (_("Next >"));
  gtk_widget_show (this->btn_next);
  gtk_container_add (GTK_CONTAINER (hbb_next), this->btn_next);
  GTK_WIDGET_SET_FLAGS (this->btn_next, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) this->dra_chooseDevice, "expose_event",
                    G_CALLBACK (on_dra_chooseDevice_expose_event),
                    this);
  g_signal_connect ((gpointer) btn_device, "clicked",
                    G_CALLBACK (on_btn_device_clicked),
                    this);
  g_signal_connect ((gpointer) this->btn_configuration, "clicked",
                    G_CALLBACK (on_btn_configuration_clicked),
                    this);
  g_signal_connect ((gpointer) this->btn_next, "clicked",
                    G_CALLBACK (on_btn_next_clicked),
                    this);
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_next), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(this->btn_configuration), FALSE);
  Global_setLabel(_("Choose device"));
}

  

