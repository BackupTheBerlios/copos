/************************************************************************
* Fichier          : recorder.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 15:53:53 with umbrello
**************************************************************************/

#include "display/recorder.h"


static gboolean 
recorder_update_image(gpointer data)
{
  /* Variables and pre-cond */
  gint x;
  gint y;
  gint width;
  gint height;
  Recorder* this;
  Global *global = Global_get();
  Webcam *webcam = global->webcam;
  g_return_val_if_fail(webcam != NULL, FALSE);
  g_return_val_if_fail(data != NULL, FALSE);
  this = (Recorder*) data;
  g_return_val_if_fail(this->tampon != NULL, FALSE);
  
  /* Code */
  webcam->getRGB(webcam,this->tampon);
  gdk_window_get_geometry(this->dra_recorder->window,
			  &x,
			  &y,
			  &width,
			  &height,
			  NULL);
  gtk_widget_queue_draw_area(this->dra_recorder,
			     0,
			     0,
			     width,
			     height);
  return TRUE;
}

static gboolean 
recorder_store_image(gpointer data)
{
  if(recorder_update_image(data)) {
    char str[255];    
    Global *global = Global_get();
    Webcam *webcam = global->webcam;
    Recorder* this;
    g_return_val_if_fail(data != NULL, FALSE);
    this = (Recorder*) data;
    
    Storage_put(this->storage, webcam->width, webcam->height, 3, this->tampon);
    ++(this->nbRecorded);

    sprintf(str,"%d", this->nbRecorded);
    gtk_label_set_text(GTK_LABEL(this->lbl_recorded),str);
    if(this->recordTimer != NULL) {
      gdouble time = g_timer_elapsed(this->recordTimer,NULL);
      if(time > this->recordTime) {
	g_signal_emit_by_name(this->btn_stop,"clicked");
      }
    }
    return TRUE;
  }
  else {
    return FALSE;
  }
}


static gboolean
on_dra_recorder_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  Global *global = Global_get();
  Webcam *webcam = global->webcam;

  if(webcam != NULL) {
    Recorder *this;
    gdouble time;
    g_return_val_if_fail(event != NULL, FALSE);
    g_return_val_if_fail(user_data != NULL, FALSE);
    this = (Recorder*) user_data;
    g_return_val_if_fail(this->tampon != NULL, FALSE);
    g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
    if(this->tag != 0) {
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
    ++(this->nbFPS);
    time = g_timer_elapsed(this->fpsTimer,0);
    if(time > 5) {
      char str[255];
      sprintf(str,"%d", (gint) floor((gfloat)this->nbFPS/5));
      gtk_label_set_text(GTK_LABEL(this->lbl_fps),str);
      this->nbFPS = 0;
      g_timer_start(this->fpsTimer);
    }
  }
  return FALSE;
}


static void
on_btn_record_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  G_CONST_RETURN gchar* str;
  gdouble tmp = 0;
  Global *global = Global_get();
  Webcam *webcam = global->webcam;
  Recorder *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Recorder*) user_data;

  Storage_record(this->storage,webcam->width,webcam->height,3);
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function recorder_update_image"));
    return;
  }
  this->tag = g_timeout_add(33, recorder_store_image,this);
  this->nbRecorded = 0;

  str = gtk_entry_get_text(GTK_ENTRY(this->ent_timeToRecord));
  if(strcmp(str,"") != 0) {
    tmp = g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_timeToRecord)),NULL);
  }
  if(tmp != 0) {
    this->recordTime = tmp;
    this->recordTimer = g_timer_new();
  }
  gtk_widget_set_sensitive(this->btn_next, FALSE);
}


static void
on_btn_stop_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  Recorder *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Recorder*) user_data;

  if(Storage_isOpen(this->storage)) {
    Storage_close(this->storage);
    if(this->tag && (!g_source_remove(this->tag))) {
      g_warning(_("Couldn't stop the function recorder_store_image"));
      return;
    }
    this->tag = g_timeout_add(33, recorder_update_image,this);
    gtk_widget_set_sensitive(this->btn_next, TRUE);
    Global_setStorage(this->storage);
    this->storage = Storage_new();
  }
  if(this->recordTimer != NULL) {
    g_timer_destroy(this->recordTimer);
    this->recordTimer = NULL;
    this->recordTime = 0;
  }
}

static void
on_btn_next_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Recorder *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Recorder*) user_data;
  /* Code */
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function update_image"));
  }
  this->tag = 0;
  if(PickLaser_isDisplayed(this->pickLaser)) {
    PickLaser_destroy(this->pickLaser);
  }
  this->pickLaser = PickLaser_new();
  PickLaser_presentation(this->pickLaser);
}

/**
 * Constructor
 */
Recorder*  Recorder_new ()
{
  /* Variables and pre-cond */
  Global *global = Global_get();
  Webcam *webcam = global->webcam;
  Recorder* ret = (Recorder*) g_malloc(sizeof(Recorder));
  g_return_val_if_fail(webcam != NULL, NULL);
  /* Code */
  ret->mainWidget = NULL;
  ret->storage = Storage_new();
  ret->dra_recorder = NULL;
  ret->tampon = (guchar*) g_malloc(webcam->maximumSize(webcam));
  ret->tag = g_timeout_add(33, recorder_update_image, ret);
  ret->lbl_recorded = NULL; 
  ret->nbRecorded = 0;
  ret->lbl_fps = NULL;
  ret->nbFPS = 0;
  ret->fpsTimer = g_timer_new();
  ret->ent_timeToRecord = NULL;
  ret->recordTime = 0.0;
  ret->recordTimer = NULL;
  ret->btn_stop = NULL;
  ret->btn_next = NULL;
  ret->pickLaser = NULL;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  Recorder_destroy (Recorder *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  /* Code */
  if(Recorder_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if(this->tag && (!g_source_remove(this->tag))) {
    g_warning(_("Couldn't stop the function update_image"));
  }
  if(PickLaser_isDisplayed(this->pickLaser)) {
    PickLaser_destroy(this->pickLaser);
    this->pickLaser = NULL;
  }
  g_free(this->tampon);
  g_timer_destroy(this->fpsTimer);
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  Recorder_isDisplayed (Recorder *this)
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
void  Recorder_presentation (Recorder *this)
{
  /* Variables and pre-cond */
/*   GtkWidget *mainWidget; */
/*   GtkWidget *dra_recorder; */
  GtkWidget *hbx_recorder;
  GtkWidget *vbx_actionRecord;
  GtkWidget *hbb_recordStop;
  GtkWidget *btn_record;
/*   GtkWidget *btn_stop; */
  GtkWidget *hbx_timeToRecord;
/*   GtkWidget *ent_timeToRecord; */
  GtkWidget *lbl_infoTime;
  GtkWidget *tbl_infos;
  GtkWidget *lbl_title_fps;
  GtkWidget *lbl_title_record;
/*   GtkWidget *lbl_fps; */
/*   GtkWidget *lbl_recorded; */
  GtkWidget *hsp_recorder;
  GtkWidget *hbb_next;
/*   GtkWidget *btn_next; */
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  /* Code */
  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  this->dra_recorder = gtk_drawing_area_new ();
  gtk_widget_show (this->dra_recorder);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), this->dra_recorder, TRUE, TRUE, 0);

  hbx_recorder = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_recorder);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_recorder, FALSE, FALSE, 0);

  vbx_actionRecord = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_actionRecord);
  gtk_box_pack_start (GTK_BOX (hbx_recorder), vbx_actionRecord, FALSE, FALSE, 0);

  hbb_recordStop = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_recordStop);
  gtk_box_pack_start (GTK_BOX (vbx_actionRecord), hbb_recordStop, FALSE, FALSE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_recordStop), GTK_BUTTONBOX_SPREAD);

  btn_record = gtk_button_new_with_mnemonic (_("Record"));
  gtk_widget_show (btn_record);
  gtk_container_add (GTK_CONTAINER (hbb_recordStop), btn_record);
  GTK_WIDGET_SET_FLAGS (btn_record, GTK_CAN_DEFAULT);

  this->btn_stop = gtk_button_new_with_mnemonic (_("Stop"));
  gtk_widget_show (this->btn_stop);
  gtk_container_add (GTK_CONTAINER (hbb_recordStop), this->btn_stop);
  GTK_WIDGET_SET_FLAGS (this->btn_stop, GTK_CAN_DEFAULT);

  hbx_timeToRecord = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_timeToRecord);
  gtk_box_pack_start (GTK_BOX (vbx_actionRecord), hbx_timeToRecord, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbx_timeToRecord), 5);

  this->ent_timeToRecord = gtk_entry_new ();
  gtk_widget_show (this->ent_timeToRecord);
  gtk_box_pack_start (GTK_BOX (hbx_timeToRecord), this->ent_timeToRecord, FALSE, FALSE, 0);
  gtk_widget_set_size_request (this->ent_timeToRecord, 65, -1);

  lbl_infoTime = gtk_label_new (_("secondes (recording time)"));
  gtk_widget_show (lbl_infoTime);
  gtk_box_pack_start (GTK_BOX (hbx_timeToRecord), lbl_infoTime, FALSE, FALSE, 0);

  tbl_infos = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (tbl_infos);
  gtk_box_pack_start (GTK_BOX (hbx_recorder), tbl_infos, FALSE, FALSE, 0);

  lbl_title_fps = gtk_label_new (_("Number of frames per seconds :"));
  gtk_widget_show (lbl_title_fps);
  gtk_table_attach (GTK_TABLE (tbl_infos), lbl_title_fps, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_title_fps), 0, 0.5);

  lbl_title_record = gtk_label_new (_("Number of recorded frames : "));
  gtk_widget_show (lbl_title_record);
  gtk_table_attach (GTK_TABLE (tbl_infos), lbl_title_record, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_title_record), 0, 0.5);

  this->lbl_fps = gtk_label_new (_("0"));
  gtk_widget_show (this->lbl_fps);
  gtk_table_attach (GTK_TABLE (tbl_infos), this->lbl_fps, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (this->lbl_fps), 0, 0.5);

  this->lbl_recorded = gtk_label_new (_("0"));
  gtk_widget_show (this->lbl_recorded);
  gtk_table_attach (GTK_TABLE (tbl_infos), this->lbl_recorded, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (this->lbl_recorded), 0, 0.5);

  hsp_recorder = gtk_hseparator_new ();
  gtk_widget_show (hsp_recorder);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_recorder, FALSE, FALSE, 0);

  hbb_next = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_next);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_next, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_next), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_next), GTK_BUTTONBOX_END);

  this->btn_next = gtk_button_new_with_mnemonic (_("Next >"));
  gtk_widget_show (this->btn_next);
  gtk_container_add (GTK_CONTAINER (hbb_next), this->btn_next);
  GTK_WIDGET_SET_FLAGS (this->btn_next, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) this->dra_recorder, "expose_event",
                    G_CALLBACK (on_dra_recorder_expose_event),
                    this);
  g_signal_connect ((gpointer) btn_record, "clicked",
                    G_CALLBACK (on_btn_record_clicked),
                    this);
  g_signal_connect ((gpointer) this->btn_stop, "clicked",
                    G_CALLBACK (on_btn_stop_clicked),
                    this);
  g_signal_connect ((gpointer) this->btn_next, "clicked",
                    G_CALLBACK (on_btn_next_clicked),
                    this);
  gtk_widget_set_sensitive(this->btn_next, FALSE);
  Global_setLabel(_("Record"));
}

  

