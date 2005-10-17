/************************************************************************
* Fichier          : visualization.c
* Date de Creation : lun aoû 16 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on lun aoû 16 2004 at 16:14:00 with umbrello
**************************************************************************/

#include "display/visualization.h"

#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define VIEW_INIT_AXIS_X 1.0
#define VIEW_INIT_AXIS_Y 0.0
#define VIEW_INIT_AXIS_Z 0.0
#define VIEW_INIT_ANGLE  20.0

#define VIEW_SCALE_MAX 3.0
#define VIEW_SCALE_MIN 0.5

static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

static float begin_x = 0.0;
static float begin_y = 0.0;

static void
on_dra_gl_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);
  g_return_if_fail(GTK_IS_WIDGET(widget));
  g_return_if_fail(user_data == NULL);
  /* Code */
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    return;
  }
  /*** Fill in the details here. ***/
  glClearColor (0.5, 0.5, 0.8, 1.0);
  glClearDepth (1.0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glShadeModel (GL_SMOOTH);
  glEnable (GL_NORMALIZE);

  /*** Init View ***/
  {
    float sine = sin (0.5 * VIEW_INIT_ANGLE * DIG_2_RAD);
    view_quat[0] = VIEW_INIT_AXIS_X * sine;
    view_quat[1] = VIEW_INIT_AXIS_Y * sine;
    view_quat[2] = VIEW_INIT_AXIS_Z * sine;
    view_quat[3] = cos (0.5 * VIEW_INIT_ANGLE * DIG_2_RAD);
    view_scale = 1.0;
  }
  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/
}


static gboolean
on_dra_gl_configure_event              (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data == NULL, FALSE);
  /* Code */
  GLfloat w = widget->allocation.width;
  GLfloat h = widget->allocation.height;
  GLfloat aspect;

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    return FALSE;
  }
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  if (w > h) {
    aspect = w / h;
    glFrustum (-aspect, aspect, -1.0, 1.0, 2.0, 60.0);
  }
  else {
    aspect = h / w;
    glFrustum (-1.0, 1.0, -aspect, aspect, 2.0, 60.0);
  }
  glMatrixMode (GL_MODELVIEW);
  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/
  return TRUE;
}


static gboolean
on_dra_gl_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Global *global = Global_get();
  Collector *collector;
  GSList *points;
  GSList *iter = NULL;
  float m[4][4];
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);
  GLfloat currentSize;

  g_return_val_if_fail(global != NULL, FALSE);
  collector = global->collector;
  g_return_val_if_fail(collector != NULL, FALSE);
  points = collector->points;
  g_return_val_if_fail(points != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data == NULL, FALSE);
  /* Code */
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    return FALSE;
  }
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  /* View transformation. */
  glTranslatef (0.0, 0.0, -30.0);
  glScalef (view_scale, view_scale, view_scale);
  build_rotmatrix (m, view_quat);
  glMultMatrixf (&m[0][0]);
  /* Draw the points */
  glColor3f(1.0,1.0,0.0);
  glGetFloatv(GL_POINT_SIZE,&currentSize);
  glPointSize(2.0);
  glBegin(GL_POINTS);
  iter = points;
  while(iter != NULL) {
    Point3D *pt = (Point3D*) iter->data;
    glVertex3f(pt->x, pt->y, pt->z);
    iter = g_slist_next(iter);
  }
  glEnd();
  glPointSize(currentSize);
  if (gdk_gl_drawable_is_double_buffered (gldrawable)) {
    gdk_gl_drawable_swap_buffers (gldrawable);
  }
  else {
    glFlush ();
  }

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/
  return TRUE;
}


static void
on_dra_gl_unrealize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);
  g_return_if_fail(GTK_IS_WIDGET(widget));
  g_return_if_fail(user_data == NULL);
  /* Code */
  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    return;
  }
  /*** Fill in the details here. ***/
  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/
}


static gboolean
on_dra_gl_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data == NULL, FALSE);
  /* Code */
  begin_x = event->x;
  begin_y = event->y;
  return FALSE;
}


static gboolean
on_dra_gl_motion_notify_event          (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  float w = widget->allocation.width;
  float h = widget->allocation.height;
  float x = event->x;
  float y = event->y;
  float d_quat[4];
  gboolean redraw = FALSE;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(user_data == NULL, FALSE);
  /* Code */
    /* Rotation. */
  if (event->state & GDK_BUTTON1_MASK) {
    trackball (d_quat,
	       (2.0 * begin_x - w) / w,
	       (h - 2.0 * begin_y) / h,
	       (2.0 * x - w) / w,
	       (h - 2.0 * y) / h);
    add_quats (d_quat, view_quat, view_quat);
    redraw = TRUE;
  }
  /* Scaling. */
  if (event->state & GDK_BUTTON2_MASK) {
    view_scale = view_scale * (1.0 + (y - begin_y) / h);
    if (view_scale > VIEW_SCALE_MAX)
      view_scale = VIEW_SCALE_MAX;
    else if (view_scale < VIEW_SCALE_MIN)
      view_scale = VIEW_SCALE_MIN;
    redraw = TRUE;
  }
  begin_x = x;
  begin_y = y;

  if (redraw)
    gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
  return TRUE;
}


static void
on_btn_save_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GtkWidget *file_selector;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data == NULL);
  /* Code */
  file_selector = gtk_file_chooser_dialog_new (_("Save File"),
					       NULL,
					       GTK_FILE_CHOOSER_ACTION_SAVE,
					       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					       GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
					       NULL);
  
  if (gtk_dialog_run (GTK_DIALOG (file_selector)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    Global *global = Global_get();
    Collector *collector = global->collector;
    g_return_if_fail(global != NULL);
    g_return_if_fail(collector != NULL);
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_selector));
    Collector_save(collector, filename);
    g_free (filename);
  }
  gtk_widget_destroy (file_selector);
}

/**
 * Constructor
 */
Visualization*  Visualization_new ()
{
  /* Variables and pre-cond */
  Visualization* ret = (Visualization*) g_malloc(sizeof(Visualization));
  /* Code */
  ret->mainWidget = NULL;

  return ret;
}


/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  Visualization_destroy (Visualization *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(Visualization_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this the object to be tested
 */
gboolean  Visualization_isDisplayed (Visualization *this)
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
void  Visualization_presentation (Visualization *this)
{
  /* Variables and pre-cond */
/*   GtkWidget *mainWidget; */ 
  GtkWidget *dra_gl;
  GtkWidget *hbb_save;
  GtkWidget *btn_save;
  GdkGLConfig *glconfig;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  /* Code */
  
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB    |
					GDK_GL_MODE_DEPTH  |
					GDK_GL_MODE_DOUBLE);
  if (glconfig == NULL) {
    g_warning ("\n*** Cannot find the double-buffered visual.");
    g_warning ("\n*** Trying single-buffered visual.");
    /* Try single-buffered visual */
    glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB   |
						GDK_GL_MODE_DEPTH);
    if (glconfig == NULL) {
	g_warning("*** No appropriate OpenGL-capable visual found.");
	return;
    }
  }

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  dra_gl = gtk_drawing_area_new ();
  gtk_widget_set_gl_capability (dra_gl,
				glconfig,
				NULL,
				TRUE,
				GDK_GL_RGBA_TYPE);
  gtk_widget_add_events (dra_gl,
			 GDK_BUTTON1_MOTION_MASK    |
			 GDK_BUTTON2_MOTION_MASK    |
			 GDK_BUTTON_PRESS_MASK      |
			 GDK_VISIBILITY_NOTIFY_MASK);
  g_signal_connect_after ((gpointer) dra_gl, "realize",
                          G_CALLBACK (on_dra_gl_realize),
                          NULL);
  g_signal_connect ((gpointer) dra_gl, "configure_event",
                    G_CALLBACK (on_dra_gl_configure_event),
                    NULL);
  g_signal_connect ((gpointer) dra_gl, "expose_event",
                    G_CALLBACK (on_dra_gl_expose_event),
                    NULL);
  g_signal_connect ((gpointer) dra_gl, "unrealize",
                    G_CALLBACK (on_dra_gl_unrealize),
                    NULL);
  g_signal_connect ((gpointer) dra_gl, "button_press_event",
                    G_CALLBACK (on_dra_gl_button_press_event),
                    NULL);
  g_signal_connect ((gpointer) dra_gl, "motion_notify_event",
                    G_CALLBACK (on_dra_gl_motion_notify_event),
                    NULL);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), dra_gl, TRUE, TRUE, 0);
  gtk_widget_show (dra_gl);

  hbb_save = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_save);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_save, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_save), 10);

  btn_save = gtk_button_new_from_stock ("gtk-save");
  gtk_widget_show (btn_save);
  gtk_container_add (GTK_CONTAINER (hbb_save), btn_save);
  GTK_WIDGET_SET_FLAGS (btn_save, GTK_CAN_DEFAULT);


  g_signal_connect ((gpointer) btn_save, "clicked",
                    G_CALLBACK (on_btn_save_clicked),
                    NULL);

  Global_setLabel(_("Visualization"));
}

  

