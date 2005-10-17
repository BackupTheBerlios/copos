#include "display/picklaser.h"

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

void
on_btn_firstImg_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *new = NULL;
  PickLaser *this;
  Global *global = Global_get();
  Storage *storage = global->storage;
  g_return_if_fail(storage != NULL);
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
  if(! Storage_isLoaded(storage)) {
    g_return_if_fail(Storage_load(storage));
  }
  Storage_seek(storage,0);
  if(this->pixbuf != NULL) g_object_unref(this->pixbuf);
  this->pixbuf = Global_getPixbufFromStorage();
  new = update_image(this);
  gtk_image_set_from_pixbuf(GTK_IMAGE(this->image), new);
  g_object_unref(new);
  update_meter(this);
}

static void
on_btn_nextImg_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkPixbuf *new = NULL;
  PickLaser *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (PickLaser*) user_data;
  /* Code */
  if(this->pixbuf != NULL) g_object_unref(this->pixbuf);
  this->pixbuf = Global_getPixbufFromStorage();
  if(this->pixbuf == NULL) { /* if at the end of file rewind */
    Global *global = Global_get();
    Storage *storage = global->storage;
    g_return_if_fail(storage != NULL);
    Storage_seek(storage,0);
    this->pixbuf = Global_getPixbufFromStorage();
  }
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

void
on_tgb_picker_toggled                  (GtkToggleButton *togglebutton,
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
on_btn_forward_clicked                 (GtkButton       *button,
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
  ret->scale = 100.0;
  ret->hsc_clicked = FALSE;
  ret->hsc_red = NULL;
  ret->hsc_green = NULL;
  ret->hsc_blue = NULL;
  ret->lbl_nbPoints = NULL;
  ret->cursor = NULL;
  ret->positions = NULL;
  ret->pixbuf = Global_getPixbufFromStorage();
  if(ret->pixbuf == NULL) { /* if at the end of file rewind */
    Global *global = Global_get();
    Storage *storage = global->storage;
    g_return_val_if_fail(storage != NULL, NULL);
    Storage_seek(global->storage,0);
    ret->pixbuf = Global_getPixbufFromStorage();
  }  
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
  if((this->positions != NULL) && Positions_isDisplayed(this->positions)) {
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
  GtkWidget *scw_image;
  GtkWidget *vwp_image;
  GtkWidget *evb_image;
  GtkWidget *hbx_infos;
  GtkWidget *lbl_scale;
  GtkObject *spb_scale_adj;
  GtkWidget *spb_scale;
  GtkWidget *btn_firstImg;
  GtkWidget *image2;
  GtkWidget *btn_nextImg;
  GtkWidget *image3;
  GtkWidget *lbl_nbPointsInfo;
  GtkWidget *hsp_infos;
  GtkWidget *hbx_picker;
  GtkWidget *tbl_picker;
  GtkWidget *lbl_red;
  GtkWidget *lbl_green;
  GtkWidget *lbl_blue;
  GtkWidget *lbl_sp1;
  GtkWidget *lbl_sp2;
  GtkWidget *lbl_sp3;
  GtkWidget *tgb_picker;
  GtkWidget *alignment1;
  GtkWidget *hbox3;
  GtkWidget *image4;
  GtkWidget *label10;
  GtkWidget *hsp_postTreatment;
  GtkWidget *hbx_postTreatment;
  GtkWidget *chb_outlayers;
  GtkWidget *chb_smooth;
  GtkWidget *hsp_picker;
  GtkWidget *hbb_picker;
  GtkWidget *btn_forward;
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

  hbx_infos = gtk_hbox_new (FALSE, 5);
  gtk_widget_show (hbx_infos);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_infos, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbx_infos), 5);

  lbl_scale = gtk_label_new (_("Scale : "));
  gtk_widget_show (lbl_scale);
  gtk_box_pack_start (GTK_BOX (hbx_infos), lbl_scale, FALSE, FALSE, 0);

  spb_scale_adj = gtk_adjustment_new (100, 10, 300, 1, 10, 10);
  spb_scale = gtk_spin_button_new (GTK_ADJUSTMENT (spb_scale_adj), 1, 0);
  gtk_widget_show (spb_scale);
  gtk_box_pack_start (GTK_BOX (hbx_infos), spb_scale, FALSE, TRUE, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spb_scale), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spb_scale), GTK_UPDATE_IF_VALID);

  btn_firstImg = gtk_button_new ();
  gtk_widget_show (btn_firstImg);
  gtk_box_pack_start (GTK_BOX (hbx_infos), btn_firstImg, FALSE, FALSE, 0);

  image2 = gtk_image_new_from_stock ("gtk-goto-first", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image2);
  gtk_container_add (GTK_CONTAINER (btn_firstImg), image2);

  btn_nextImg = gtk_button_new ();
  gtk_widget_show (btn_nextImg);
  gtk_box_pack_start (GTK_BOX (hbx_infos), btn_nextImg, FALSE, FALSE, 0);

  image3 = gtk_image_new_from_stock ("gtk-go-forward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image3);
  gtk_container_add (GTK_CONTAINER (btn_nextImg), image3);

  lbl_nbPointsInfo = gtk_label_new (_("Number of points : "));
  gtk_widget_show (lbl_nbPointsInfo);
  gtk_box_pack_start (GTK_BOX (hbx_infos), lbl_nbPointsInfo, TRUE, FALSE, 0);

  this->lbl_nbPoints = gtk_label_new (_("0"));
  gtk_widget_show (this->lbl_nbPoints);
  gtk_box_pack_start (GTK_BOX (hbx_infos), this->lbl_nbPoints, TRUE, FALSE, 0);

  hsp_infos = gtk_hseparator_new ();
  gtk_widget_show (hsp_infos);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_infos, FALSE, FALSE, 0);

  hbx_picker = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbx_picker);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_picker, FALSE, FALSE, 0);

  tbl_picker = gtk_table_new (3, 3, FALSE);
  gtk_widget_show (tbl_picker);
  gtk_box_pack_start (GTK_BOX (hbx_picker), tbl_picker, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_picker), 5);

  lbl_red = gtk_label_new (_("Red"));
  gtk_widget_show (lbl_red);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_red, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_red), 0, 0.5);

  lbl_green = gtk_label_new (_("Green "));
  gtk_widget_show (lbl_green);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_green, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_green), 0, 0.5);

  lbl_blue = gtk_label_new (_("Blue"));
  gtk_widget_show (lbl_blue);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_blue, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_blue), 0, 0.5);

  lbl_sp1 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp1);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_sp1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp1), 0, 0.5);

  lbl_sp2 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp2);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_sp2, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp2), 0, 0.5);

  lbl_sp3 = gtk_label_new (_(":"));
  gtk_widget_show (lbl_sp3);
  gtk_table_attach (GTK_TABLE (tbl_picker), lbl_sp3, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_sp3), 0, 0.5);

  this->hsc_red = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_red);
  gtk_table_attach (GTK_TABLE (tbl_picker), this->hsc_red, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_red), 0);

  this->hsc_green = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_green);
  gtk_table_attach (GTK_TABLE (tbl_picker), this->hsc_green, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_green), 0);

  this->hsc_blue = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 255, 1, 10, 0)));
  gtk_widget_show (this->hsc_blue);
  gtk_table_attach (GTK_TABLE (tbl_picker), this->hsc_blue, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_scale_set_digits (GTK_SCALE (this->hsc_blue), 0);

  tgb_picker = gtk_toggle_button_new ();
  gtk_widget_show (tgb_picker);
  gtk_box_pack_start (GTK_BOX (hbx_picker), tgb_picker, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tgb_picker), 5);

  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (tgb_picker), alignment1);

  hbox3 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox3);

  image4 = gtk_image_new_from_stock ("gtk-color-picker", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image4);
  gtk_box_pack_start (GTK_BOX (hbox3), image4, FALSE, FALSE, 0);

  label10 = gtk_label_new_with_mnemonic (_("Picker"));
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (hbox3), label10, FALSE, FALSE, 0);

  hsp_postTreatment = gtk_hseparator_new ();
  gtk_widget_show (hsp_postTreatment);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_postTreatment, FALSE, TRUE, 0);

  hbx_postTreatment = gtk_hbox_new (FALSE, 5);
  gtk_widget_show (hbx_postTreatment);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbx_postTreatment, FALSE, TRUE, 0);

  chb_outlayers = gtk_check_button_new_with_mnemonic (_("Eliminate outlayers"));
  gtk_widget_show (chb_outlayers);
  gtk_box_pack_start (GTK_BOX (hbx_postTreatment), chb_outlayers, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (chb_outlayers, FALSE);

  chb_smooth = gtk_check_button_new_with_mnemonic (_("Smooth"));
  gtk_widget_show (chb_smooth);
  gtk_box_pack_start (GTK_BOX (hbx_postTreatment), chb_smooth, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (chb_smooth, FALSE);

  hsp_picker = gtk_hseparator_new ();
  gtk_widget_show (hsp_picker);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_picker, FALSE, TRUE, 0);

  hbb_picker = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_picker);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_picker, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_picker), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_picker), GTK_BUTTONBOX_END);

  btn_forward = gtk_button_new_from_stock ("gtk-go-forward");
  gtk_widget_show (btn_forward);
  gtk_container_add (GTK_CONTAINER (hbb_picker), btn_forward);
  GTK_WIDGET_SET_FLAGS (btn_forward, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) spb_scale, "value_changed",
                    G_CALLBACK (on_spb_scale_value_changed),
                    this);

  g_signal_connect ((gpointer) btn_firstImg, "clicked",
                    G_CALLBACK (on_btn_firstImg_clicked),
                    this);
  g_signal_connect ((gpointer) btn_nextImg, "clicked",
                    G_CALLBACK (on_btn_nextImg_clicked),
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
  g_signal_connect ((gpointer) tgb_picker, "toggled",
                    G_CALLBACK (on_tgb_picker_toggled),
                    this);
  g_signal_connect ((gpointer) evb_image, "button_release_event",
                    G_CALLBACK (on_evb_image_button_release_event),
                    this);

  g_signal_connect ((gpointer) btn_forward, "clicked",
                    G_CALLBACK (on_btn_forward_clicked),
                    this);

  Global_setLabel(_("Pick the laser"));
}

  

