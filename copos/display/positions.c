/************************************************************************
* Fichier          : positions.c
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#include "display/positions.h"
static gboolean
on_fxd_positions_expose_event          (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Positions *this;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data != NULL, FALSE);
  this = (Positions*) user_data;
  g_return_val_if_fail(this->pixbuf != NULL, FALSE);
  /* Code */
  gdk_draw_pixbuf(widget->window,
                  widget->style->white_gc,
                  this->pixbuf,
                  0,
                  0,
                  0,
                  0,
                  -1,
                  -1,
                  GDK_RGB_DITHER_NORMAL,
                  0,
                  0);
  return FALSE;
}

static gboolean
checkEntry                             (Positions *this,
					guint   *focal,
					gdouble *distCamera,
					gdouble *angleCamera,
					gdouble *distCenter)
{
  /* Variables and pre-cond */
  G_CONST_RETURN gchar* str;
  GString* err;
  gboolean ret = TRUE;
  g_return_val_if_fail(focal       != NULL, FALSE);
  g_return_val_if_fail(distCamera  != NULL, FALSE);
  g_return_val_if_fail(angleCamera != NULL, FALSE);
  g_return_val_if_fail(distCenter  != NULL, FALSE);
  /* Code */
  err = g_string_new("");
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_distCam));
  if(strcmp(str,"") == 0) {
    err = g_string_append(err,_("Add the value of the distance between webcam and the laser pointer\n"));
  }
  else {
    *distCamera = g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_distCam)),NULL);
  }
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_distCenter));
  if(strcmp(str,"") == 0) {
    err = g_string_append(err,"Add the value of the distance between the departure of the laser and the table\n");
  }
  else {
    *distCenter = g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_distCenter)),NULL);
  }
  str = gtk_entry_get_text(GTK_ENTRY(this->ent_focal));
  if(strcmp(str,"") == 0) {
    err = g_string_append(err,"Add the value of the focal distance of the  webcam\n");
  }
  else {
    *focal = (guint) g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_focal)),NULL);
  }

  str = gtk_entry_get_text(GTK_ENTRY(this->ent_angle));
  if(strcmp(str,"") == 0) {
    err = g_string_append(err,"Add the value of the angle formed by the webcam\n");
  }
  else {
    *angleCamera = g_strtod(gtk_entry_get_text(GTK_ENTRY(this->ent_angle)),NULL);
  }
  /* If there's errors, return false and display an error message */
  if(strcmp(err->str,"") != 0) {
    Global_errMessage(err->str);
    ret = FALSE;
  }

  return ret;
}

static void
on_btn_next_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  guint   focal = 0;
  gdouble distCamera = 0.0;
  gdouble angleCamera = 0.0;
  gdouble distCenter = 0.0;
  Positions *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Positions*) user_data;
  /* Code */
  if(! checkEntry(this,&focal,&distCamera,&angleCamera,&distCenter)) {
    return;
  }
  else {
    PerspectiveCreator *perspectiveCreator = PerspectiveCreator_new();
    perspectiveCreator->focal = focal;
    perspectiveCreator->distCamera = distCamera;
    perspectiveCreator->angleCamera = angleCamera;
    Global_setPerspectiveCreator(perspectiveCreator);

    if(Generate_isDisplayed(this->generate)) {
      Generate_destroy(this->generate);
      this->generate = NULL;
    }
    this->generate = Generate_new(distCenter);
    if(this->generate == NULL) {
      Global_errMessage(_("Check your values,\nThere's a problem during the compute"));
      return;
    }
    Generate_presentation(this->generate);
  }
}

/**
 * Constructor
 */
Positions*  Positions_new ()
{
  /* Variables and pre-cond */
  GError *err = NULL;
  Positions* ret = (Positions*) g_malloc(sizeof(Positions));
  /* Code */
  ret->mainWidget = NULL;
  ret->ent_distCam = NULL;
  ret->ent_distCenter = NULL;
  ret->ent_focal = NULL;
  ret->ent_angle = NULL;
  ret->generate = NULL;
  ret->pixbuf = gdk_pixbuf_new_from_file(POSITIONS_PIX,&err);
  if (err != NULL) {
    g_warning("%s", err->message);
    g_error_free (err);
    return NULL;
  }
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  Positions_destroy (Positions *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(Positions_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if((this->generate != NULL) && Generate_isDisplayed(this->generate)) {
    Generate_destroy(this->generate);
    this->generate = NULL;
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  Positions_isDisplayed (Positions *this)
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
void  Positions_presentation (Positions *this)
{
  /* Variables and pre-cond */
/*   GtkWidget *mainWidget; */
  GtkWidget *fxd_positions;
/*   GtkWidget *ent_focal; */
/*   GtkWidget *ent_distCam; */
/*   GtkWidget *ent_angle; */
/*   GtkWidget *ent_distCenter; */
  GtkWidget *hsp_positions;
  GtkWidget *hbb_next;
  GtkWidget *btn_next;
  GtkTooltips *tooltips;
  GList    *focusable_widgets = NULL;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  /* Code */


  tooltips = gtk_tooltips_new ();

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  fxd_positions = gtk_fixed_new ();
  gtk_widget_show (fxd_positions);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), fxd_positions, TRUE, TRUE, 0);

  this->ent_focal = gtk_entry_new ();
  focusable_widgets = g_list_append(focusable_widgets, this->ent_focal);
  gtk_widget_show (this->ent_focal);
  gtk_fixed_put (GTK_FIXED (fxd_positions), this->ent_focal, 8, 404);
  gtk_widget_set_size_request (this->ent_focal, 80, 21);
  gtk_tooltips_set_tip (tooltips, this->ent_focal, _("The focal distance of the webcam (in pixels, see documentations) "), NULL);

  this->ent_distCam = gtk_entry_new ();
  focusable_widgets = g_list_append(focusable_widgets, this->ent_distCam);
  gtk_widget_show (this->ent_distCam);
  gtk_fixed_put (GTK_FIXED (fxd_positions), this->ent_distCam, 175, 375);
  gtk_widget_set_size_request (this->ent_distCam, 80, 21);
  gtk_tooltips_set_tip (tooltips, this->ent_distCam, _("Distance between the webcam and the laser pointer"), NULL);

  this->ent_angle = gtk_entry_new ();
  focusable_widgets = g_list_append(focusable_widgets, this->ent_angle);
  gtk_widget_show (this->ent_angle);
  gtk_fixed_put (GTK_FIXED (fxd_positions), this->ent_angle, 168, 310);
  gtk_widget_set_size_request (this->ent_angle, 50, 21);
  gtk_tooltips_set_tip (tooltips, this->ent_angle, _("The angle formed by the webcam "), NULL);

  this->ent_distCenter = gtk_entry_new ();
  focusable_widgets = g_list_append(focusable_widgets, this->ent_distCenter);
  gtk_widget_show (this->ent_distCenter);
  gtk_fixed_put (GTK_FIXED (fxd_positions), this->ent_distCenter, 335, 230);
  gtk_widget_set_size_request (this->ent_distCenter, 80, 21);
  gtk_tooltips_set_tip (tooltips, this->ent_distCenter, _("Distance between the departure of the laser and the table "), NULL);

  gtk_container_set_focus_chain(GTK_CONTAINER(fxd_positions), focusable_widgets);

  hsp_positions = gtk_hseparator_new ();
  gtk_widget_show (hsp_positions);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_positions, FALSE, FALSE, 0);

  hbb_next = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_next);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_next, FALSE, FALSE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_next), GTK_BUTTONBOX_END);

  btn_next = gtk_button_new_with_mnemonic (_("Next >"));
  gtk_widget_show (btn_next);
  gtk_container_add (GTK_CONTAINER (hbb_next), btn_next);
  gtk_container_set_border_width (GTK_CONTAINER (btn_next), 5);
  GTK_WIDGET_SET_FLAGS (btn_next, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) fxd_positions, "expose_event",
                    G_CALLBACK (on_fxd_positions_expose_event),
                    this);
  g_signal_connect ((gpointer) btn_next, "clicked",
                    G_CALLBACK (on_btn_next_clicked),
                    this);
  
  Global_setLabel(_("Set the positions"));
  gtk_widget_grab_focus (this->ent_focal);
}
