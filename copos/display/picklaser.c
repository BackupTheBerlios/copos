/************************************************************************
* Fichier          : picklaser.c
* Date de Creation : jeu aoû 12 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on jeu aoû 12 2004 at 12:03:33 with umbrello
**************************************************************************/

#include "picklaser.h"

/* Return true if the user clicks on the image and update the coordinate x and y
   to adjust them to the image ; else the function return false */
static 
gboolean check_coordinate(GtkWidget *widget, gint *x, gint *y)
{
  /* Variables and pre-cond */
  GtkImage *image;
  GdkPixbuf *pixbuf;
  gint width, height;
  gint evb_width  = 0;
  gint evb_height = 0;
  gint bord_x = 0;
  gint bord_y = 0;
  gboolean ret = TRUE;
  g_return_val_if_fail(GTK_IS_BIN(widget),FALSE);
  /* Code */
  /* get the geometry of the event box */
  gdk_window_get_geometry(GTK_WIDGET(widget)->window,
			  NULL,
			  NULL,
			  &evb_width,
			  &evb_height,
			  NULL);
  image = GTK_IMAGE(gtk_bin_get_child(GTK_BIN(widget)));
  pixbuf = gtk_image_get_pixbuf(image);
  width = gdk_pixbuf_get_width(pixbuf);
  height = gdk_pixbuf_get_height(pixbuf);
  /* Determine if the border is not null */
  if((evb_width - width) > 0) {
    bord_x = floor((float) (evb_width - width) / 2);
  }
  if((evb_height - height) > 0) {
    bord_y = floor((float) (evb_height - height) / 2);
  }
  /* Adjuste the coordinate */
  if((*x > bord_x) && (*x < (evb_width-bord_x))) {
    *x = *x-bord_x;
  }
  else {
    *x = 0;
    ret = FALSE;
  }
  if((*y >bord_y) && (*y < (evb_height-bord_y))) {
    *y = *y-bord_y;
  }
  else {
    *y = 0;
    ret = FALSE;
  }
  return ret;
}

static void
update_pixbuf(PickLaser* this, gboolean next)
{
  /* Variables and pre-cond */
  guint width;
  guint height;
  guint bytesPerPixel;
  guchar *tampon;
  gboolean isLoaded;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_if_fail(storage != NULL);
  /* Code */
  if(! Storage_isOpen(storage)) {
    if(next) {
      Storage_readBegin(storage);
    }
    else {
      Storage_readEnd(storage);
    }
  }
  Storage_getInfos(storage, &width, &height, &bytesPerPixel);
  tampon = (guchar*) g_malloc(sizeof(guchar)*width*height*bytesPerPixel);

  if(next) {
    isLoaded = Storage_get(storage,tampon);
  }
  else {
    isLoaded = Storage_getReverse(storage,tampon);
  }
  if(isLoaded) {
    GdkPixbuf *pixbuf;
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			    FALSE,
			    8,
			    width,
			    height);
    Global_gucharToGdkPixbuf(tampon,pixbuf);
    if(this->pixbuf != NULL) g_object_unref(this->pixbuf);
    this->pixbuf = g_object_ref(pixbuf); 
  }
  g_free(tampon);
}

static GdkPixbuf* 
update_image(PickLaser* this)
{
  /* Variables and pre-cond */
  gint dest_width;
  gint dest_height;
  GdkPixbuf *ret;
  GSList    *points;
  GSList    *iter;
  g_return_val_if_fail(this != NULL, NULL);
  g_return_val_if_fail(this->pixbuf != NULL, NULL);
  /* Code */
  if(this->imageAnalyse == NULL) {
    this->imageAnalyse = ImageAnalyse_new();
  }
  /* scaling */
  dest_width  = (this->scale/100.0) * gdk_pixbuf_get_width(this->pixbuf);
  dest_height = (this->scale/100.0) * gdk_pixbuf_get_height(this->pixbuf);

  ret = gdk_pixbuf_scale_simple(this->pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);

  points = ImageAnalyse_computePixbuf(this->imageAnalyse,ret);
  iter = points;
  while(iter != NULL) {
    Point2D *pt = (Point2D*) iter->data;
    Global_putPixel(ret, pt->x, pt->y, 255, 255, 255);
    iter = g_slist_next(iter);
  }
  g_slist_foreach(points,(GFunc) Point2D_destroy,NULL);
  g_slist_free(points);

  return ret;
}

static void
update_meter(PickLaser *this)
{
  /* Variables and pre-cond */
  GSList    *points;
  gchar str[256];
  g_return_if_fail(this != NULL);
  g_return_if_fail(this->pixbuf != NULL);
  g_return_if_fail(this->lbl_nbPoints != NULL);
  /* Code */
  if(this->imageAnalyse == NULL) {
    this->imageAnalyse = ImageAnalyse_new();
  }
  points = ImageAnalyse_computePixbuf(this->imageAnalyse,this->pixbuf);
  sprintf(str,"%d",g_slist_length(points));
  gtk_label_set_text(GTK_LABEL(this->lbl_nbPoints),str);
  g_slist_foreach(points,(GFunc) Point2D_destroy,NULL);
  g_slist_free(points);
}

static gboolean
on_evb_image_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  gint x, y;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data != NULL, FALSE);
  this = (PickLaser*) user_data;
  g_return_val_if_fail(GTK_IS_RANGE(this->hsc_red), FALSE);
  g_return_val_if_fail(GTK_IS_RANGE(this->hsc_green), FALSE);
  g_return_val_if_fail(GTK_IS_RANGE(this->hsc_blue), FALSE);
  /* Code */
  x = floor(event->x);
  y = floor(event->y);
  if((this->cursor != NULL) && (check_coordinate(widget,&x,&y))) {
    gint dest_x;
    gint dest_y;
    gint has_alpha, src_rowstride, bytes_per_pixel;
    guchar *src_line;
    guchar *src_pixel;
    dest_x =  (gint) floor((gdouble) x / (this->scale/100.0));
    dest_y =  (gint) floor((gdouble) y / (this->scale/100.0));
    g_return_val_if_fail(GDK_IS_PIXBUF(this->pixbuf),FALSE);
    has_alpha = gdk_pixbuf_get_has_alpha (this->pixbuf);
    bytes_per_pixel = has_alpha ? 4 : 3;
    src_rowstride = gdk_pixbuf_get_rowstride (this->pixbuf);
    src_line = gdk_pixbuf_get_pixels (this->pixbuf);
    /* find the clicked pixel */
    src_pixel = src_line + (dest_y*src_rowstride) + (dest_x*bytes_per_pixel);
    /* update the horizontale scale */
    gtk_range_set_value(GTK_RANGE(this->hsc_red), src_pixel[0]);
    gtk_range_set_value(GTK_RANGE(this->hsc_green), src_pixel[1]);
    gtk_range_set_value(GTK_RANGE(this->hsc_blue), src_pixel[2]);
  }
  return FALSE;
}


static void
on_spb_scale_value_changed             (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  GdkPixbuf* new;
  g_return_if_fail(GTK_IS_SPIN_BUTTON(spinbutton));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */ 
  this->scale = gtk_spin_button_get_value(spinbutton);
  new = update_image(this);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
  g_object_unref(new);
}


static void
on_btn_image_previous_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *new = NULL;
  PickLaser *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
  update_pixbuf(this,FALSE);
  new = update_image(this);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
  g_object_unref(new);
  update_meter(this);
}

static void
on_btn_image_next_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *new = NULL;
  PickLaser *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
  update_pixbuf(this,TRUE);
  new = update_image(this);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
  g_object_unref(new);
  update_meter(this);
}


static gboolean
on_hsc_button_press_event              (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data != NULL, FALSE);
  this = (PickLaser*) user_data;
  /* Code */
  this->hsc_clicked = TRUE;
  return FALSE;
}


static gboolean
on_hsc_button_release_event            (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  GdkPixbuf* new;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data != NULL, FALSE);
  this = (PickLaser*) user_data;
  /* Code */

  if(this->imageAnalyse == NULL) {
    this->imageAnalyse = ImageAnalyse_new();
  }
  this->imageAnalyse->edgeOfRed = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_red));
  this->imageAnalyse->edgeOfGreen = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_green));
  this->imageAnalyse->edgeOfBlue = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_blue));
  
  /* updating image */
  this->hsc_clicked = FALSE;
  new = update_image(this);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
  g_object_unref(new);
  update_meter(this);
  return FALSE;
}


void
on_hsc_value_changed                   (GtkRange        *range,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  g_return_if_fail(GTK_IS_RANGE(range));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
    if(this->imageAnalyse == NULL) {
    this->imageAnalyse = ImageAnalyse_new();
  }
  if(! this->hsc_clicked) {
    GdkPixbuf* new;
    this->imageAnalyse->edgeOfRed = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_red));
    this->imageAnalyse->edgeOfGreen = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_green));
    this->imageAnalyse->edgeOfBlue = (guchar) gtk_range_get_value(GTK_RANGE(this->hsc_blue));
    /* updating image */
    new = update_image(this);
    gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
    g_object_unref(new);
    update_meter(this);
  }
}


void
on_tgb_pick_toggled                    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  g_return_if_fail(GTK_IS_TOGGLE_BUTTON(togglebutton));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
    this->cursor = gdk_cursor_new(GDK_CROSS);
    gdk_window_set_cursor (this->image->window, this->cursor);
  }
  else {
    gdk_cursor_unref(this->cursor);
    this->cursor = NULL;
    gdk_window_set_cursor(this->image->window,NULL);
  }
}


static void
on_btn_next_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  PickLaser *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  g_return_if_fail(this->imageAnalyse != NULL);
  /* Code */
  Global_setImageAnalyse(this->imageAnalyse);
  this->imageAnalyse = NULL;
  if((this->positions != NULL) && Positions_isDisplayed(this->positions)) {
    Positions_destroy(this->positions);
    this->positions = NULL;
  }
  this->positions = Positions_new();
  Positions_presentation(this->positions);
}

/**
 * Constructor
 */
PickLaser*  PickLaser_new ()
{
  /* Variables and pre-cond */
  PickLaser* ret = (PickLaser*) g_malloc(sizeof(PickLaser));
  /* Code */
  ret->mainWidget = NULL;
  ret->imageAnalyse = ImageAnalyse_new();
  ret->image = NULL;
  ret->pixbuf = NULL;
  ret->scale = 100.0;
  ret->cursor = NULL;
  ret->hsc_clicked = FALSE;
  ret->hsc_red = NULL;
  ret->hsc_green = NULL;
  ret->hsc_blue = NULL;
  ret->lbl_nbPoints = NULL;
  ret->positions = NULL;
  update_pixbuf(ret, TRUE);
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to destroy
 */
void  PickLaser_destroy (PickLaser *this)
{
  /* Variables and pre-cond */
  g_return_if_fail(this != NULL);
  /* Code */

  if(PickLaser_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if(this->imageAnalyse != NULL) {
    ImageAnalyse_destroy(this->imageAnalyse);
  }
  if(Positions_isDisplayed(this->positions)) {
    Positions_destroy(this->positions);
    this->positions = NULL;
  }
  g_object_unref(this->pixbuf);
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  PickLaser_isDisplayed (PickLaser *this)
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
void  PickLaser_presentation (PickLaser *this)
{
  /* Variables and pre-cond */
/*   GtkWidget *mainWidget; */
  GtkWidget *scw_image;
  GtkWidget *vwp_image;
  GtkWidget *evb_image;
/*   GtkWidget *image; */
  GtkWidget *hbx_infos;
  GtkWidget *lbl_scale;
  GtkObject *spb_scale_adj;
  GtkWidget *spb_scale;
  GtkWidget *btn_image_previous;
  GtkWidget *image2;
  GtkWidget *btn_image_next;
  GtkWidget *image3;
  GtkWidget *lbl_infoNbPoints;
/*   GtkWidget *lbl_nbPoints; */
  GtkWidget *hsp_scale_pick;
  GtkWidget *hbx_selectColor;
  GtkWidget *tbl_cursors;
  GtkWidget *lb_red;
  GtkWidget *lbl_green;
  GtkWidget *lbl_blue;
/*   GtkWidget *hsc_red; */
/*   GtkWidget *hsc_green; */
/*   GtkWidget *hsc_blue; */
  GtkWidget *tgb_pick;
  GtkWidget *alignment1;
  GtkWidget *hbox3;
  GtkWidget *image4;
  GtkWidget *label8;
  GtkWidget *hsp_pick_next;
  GtkWidget *hbb_next;
  GtkWidget *btn_next;
  GdkPixbuf *new;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
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

  evb_image = gtk_event_box_new ();
  gtk_widget_show (evb_image);
  gtk_container_add (GTK_CONTAINER (vwp_image), evb_image);

  new = update_image(this);
  this->image = gtk_image_new_from_pixbuf(new);
  g_object_unref(new);
  gtk_widget_show (this->image);
  gtk_container_add (GTK_CONTAINER (evb_image), this->image);

  hbx_infos = gtk_hbox_new (FALSE, 3);
  gtk_widget_show (hbx_infos);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_infos, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbx_infos), 5);

  lbl_scale = gtk_label_new (_("Scale : "));
  gtk_widget_show (lbl_scale);
  gtk_box_pack_start (GTK_BOX (hbx_infos), lbl_scale, FALSE, FALSE, 0);

  spb_scale_adj = gtk_adjustment_new (100, 10, 300, 1, 10, 10);
  spb_scale = gtk_spin_button_new (GTK_ADJUSTMENT (spb_scale_adj), 1, 0);
  gtk_widget_show (spb_scale);
  gtk_box_pack_start (GTK_BOX (hbx_infos), spb_scale, FALSE, FALSE, 0);

  btn_image_previous = gtk_button_new ();
  gtk_widget_show (btn_image_previous);
  gtk_box_pack_start (GTK_BOX (hbx_infos), btn_image_previous, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (btn_image_previous), 5);

  image2 = gtk_image_new_from_stock ("gtk-go-back", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image2);
  gtk_container_add (GTK_CONTAINER (btn_image_previous), image2);

  btn_image_next = gtk_button_new ();
  gtk_widget_show (btn_image_next);
  gtk_box_pack_start (GTK_BOX (hbx_infos), btn_image_next, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (btn_image_next), 5);

  image3 = gtk_image_new_from_stock ("gtk-go-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image3);
  gtk_container_add (GTK_CONTAINER (btn_image_next), image3);

  lbl_infoNbPoints = gtk_label_new (_("Number of points : "));
  gtk_widget_show (lbl_infoNbPoints);
  gtk_box_pack_start (GTK_BOX (hbx_infos), lbl_infoNbPoints, FALSE, FALSE, 0);

  this->lbl_nbPoints = gtk_label_new (_("0"));
  gtk_widget_show (this->lbl_nbPoints);
  gtk_box_pack_start (GTK_BOX (hbx_infos), this->lbl_nbPoints, FALSE, FALSE, 0);

  hsp_scale_pick = gtk_hseparator_new ();
  gtk_widget_show (hsp_scale_pick);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_scale_pick, FALSE, FALSE, 0);

  hbx_selectColor = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_selectColor);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_selectColor, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbx_selectColor), 5);

  tbl_cursors = gtk_table_new (3, 2, FALSE);
  gtk_widget_show (tbl_cursors);
  gtk_box_pack_start (GTK_BOX (hbx_selectColor), tbl_cursors, TRUE, TRUE, 0);

  lb_red = gtk_label_new (_("Red : "));
  gtk_widget_show (lb_red);
  gtk_table_attach (GTK_TABLE (tbl_cursors), lb_red, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lb_red), 0, 0.5);

  lbl_green = gtk_label_new (_("Green : "));
  gtk_widget_show (lbl_green);
  gtk_table_attach (GTK_TABLE (tbl_cursors), lbl_green, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_green), 0, 0.5);

  lbl_blue = gtk_label_new (_("Blue : "));
  gtk_widget_show (lbl_blue);
  gtk_table_attach (GTK_TABLE (tbl_cursors), lbl_blue, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_blue), 0, 0.5);

  this->hsc_red = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_red);
  gtk_table_attach (GTK_TABLE (tbl_cursors), this->hsc_red, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_red), 0);
  this->hsc_green = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_green);
  gtk_table_attach (GTK_TABLE (tbl_cursors), this->hsc_green, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_green), 0);
  this->hsc_blue = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_blue);
  gtk_table_attach (GTK_TABLE (tbl_cursors), this->hsc_blue, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_blue), 0);
  tgb_pick = gtk_toggle_button_new ();
  gtk_widget_show (tgb_pick);
  gtk_box_pack_start (GTK_BOX (hbx_selectColor), tgb_pick, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tgb_pick), 10);

  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (tgb_pick), alignment1);

  hbox3 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox3);

  image4 = gtk_image_new_from_stock ("gtk-color-picker", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image4);
  gtk_box_pack_start (GTK_BOX (hbox3), image4, FALSE, FALSE, 0);

  label8 = gtk_label_new_with_mnemonic (_("Pick"));
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox3), label8, FALSE, FALSE, 0);

  hsp_pick_next = gtk_hseparator_new ();
  gtk_widget_show (hsp_pick_next);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_pick_next, FALSE, FALSE, 0);

  hbb_next = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_next);
  gtk_box_pack_end (GTK_BOX (this->mainWidget), hbb_next, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_next), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_next), GTK_BUTTONBOX_END);

  btn_next = gtk_button_new_with_mnemonic (_("Next >"));
  gtk_widget_show (btn_next);
  gtk_container_add (GTK_CONTAINER (hbb_next), btn_next);
  GTK_WIDGET_SET_FLAGS (btn_next, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) evb_image, "button_release_event",
                    G_CALLBACK (on_evb_image_button_release_event),
                    this);
  g_signal_connect ((gpointer) spb_scale, "value_changed",
                    G_CALLBACK (on_spb_scale_value_changed),
                    this);
  g_signal_connect ((gpointer) btn_image_previous, "clicked",
                    G_CALLBACK (on_btn_image_previous_clicked),
                    this);
  g_signal_connect ((gpointer) btn_image_next, "clicked",
                    G_CALLBACK (on_btn_image_next_clicked),
                    this);
  g_signal_connect ((gpointer) this->hsc_red, "button_press_event",
                    G_CALLBACK (on_hsc_button_press_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_red, "button_release_event",
                    G_CALLBACK (on_hsc_button_release_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_red, "value_changed",
                    G_CALLBACK (on_hsc_value_changed),
                    this);
  g_signal_connect ((gpointer) this->hsc_green, "button_press_event",
                    G_CALLBACK (on_hsc_button_press_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_green, "button_release_event",
                    G_CALLBACK (on_hsc_button_release_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_green, "value_changed",
                    G_CALLBACK (on_hsc_value_changed),
                    this);
  g_signal_connect ((gpointer) this->hsc_blue, "button_press_event",
                    G_CALLBACK (on_hsc_button_press_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_blue, "button_release_event",
                    G_CALLBACK (on_hsc_button_release_event),
                    this);
  g_signal_connect ((gpointer) this->hsc_blue, "value_changed",
                    G_CALLBACK (on_hsc_value_changed),
                    this);
  g_signal_connect ((gpointer) tgb_pick, "toggled",
                    G_CALLBACK (on_tgb_pick_toggled),
                    this);
  g_signal_connect ((gpointer) btn_next, "clicked",
                    G_CALLBACK (on_btn_next_clicked),
                    this);
  Global_setLabel(_("Pick the laser"));
}

  

