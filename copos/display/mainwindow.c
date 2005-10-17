#include "display/mainwindow.h"

void
on_mni_new_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  MainWindow* this;
  g_return_if_fail(GTK_IS_MENU_ITEM(menuitem));
  g_return_if_fail(user_data != NULL);
  this = (MainWindow*) user_data;
  /* Code */
  if(Introduction_isDisplayed(this->introduction)) {
    Introduction_destroy(this->introduction);
  }
  this->introduction = Introduction_new();
  Introduction_presentation(this->introduction);
}

void
on_mni_about_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  GtkWidget *dlg_about;
  GtkWidget *vbx_about;
  GtkWidget *vbx_informations;
  GtkWidget *img_logo;
  GtkWidget *lbl_about;
  GtkWidget *scw_about;
  GtkWidget *tvw_personal;
  GtkWidget *hbb_about;
  GtkWidget *closebutton1;

  MainWindow* this;
  g_return_if_fail(GTK_IS_MENU_ITEM(menuitem));
  g_return_if_fail(user_data != NULL);
  this = (MainWindow*) user_data;
  /* Code */
  dlg_about = gtk_dialog_new ();
  gtk_widget_set_size_request (dlg_about, 380, 290);
  gtk_window_set_title (GTK_WINDOW (dlg_about), _("About COPOS"));
  gtk_window_set_type_hint (GTK_WINDOW (dlg_about), GDK_WINDOW_TYPE_HINT_DIALOG);

  vbx_about = GTK_DIALOG (dlg_about)->vbox;
  gtk_widget_show (vbx_about);

  vbx_informations = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_informations);
  gtk_box_pack_start (GTK_BOX (vbx_about), vbx_informations, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbx_informations), 10);

  img_logo = gtk_image_new_from_file(ABOUT_PIX);
  gtk_widget_show (img_logo);
  gtk_box_pack_start (GTK_BOX (vbx_informations), img_logo, TRUE, TRUE, 0);

  lbl_about = gtk_label_new (_("<b>COPOS</b>\nAn analyser of video file to make a cloud of 3D points"));
  gtk_widget_show (lbl_about);
  gtk_box_pack_start (GTK_BOX (vbx_informations), lbl_about, FALSE, FALSE, 5);
  gtk_label_set_use_markup (GTK_LABEL (lbl_about), TRUE);
  gtk_label_set_justify (GTK_LABEL (lbl_about), GTK_JUSTIFY_CENTER);

  scw_about = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scw_about);
  gtk_box_pack_start (GTK_BOX (vbx_informations), scw_about, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scw_about), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scw_about), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scw_about), GTK_SHADOW_IN);

  tvw_personal = gtk_text_view_new ();
  gtk_widget_show (tvw_personal);
  gtk_container_add (GTK_CONTAINER (scw_about), tvw_personal);
  gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (tvw_personal)), _("Version: 0.8:q\nAuthor: Ronan Billon <cirdan@mail.berlios.de>\nLicense: GNU General Public License\nHomepage: http://copos.berlios.de"), -1);

  hbb_about = GTK_DIALOG (dlg_about)->action_area;
  gtk_widget_show (hbb_about);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_about), GTK_BUTTONBOX_END);

  closebutton1 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (closebutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dlg_about), closebutton1, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (closebutton1, GTK_CAN_DEFAULT);

  gtk_dialog_run (GTK_DIALOG (dlg_about));
  gtk_widget_destroy (dlg_about);
}

/**
 * Constructor
 */
MainWindow*  MainWindow_new ()
{
  MainWindow* ret = (MainWindow*) g_malloc(sizeof(MainWindow));
  ret->introduction = NULL;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroyed
 */
void  MainWindow_destroy (MainWindow *this)
{
  g_assert(this != NULL);
  /* Destroy in cascading the pages */
  if(this->introduction != 0) { 
/*     Introduction_destroy(this->introduction); */
  } 
  g_free(this);
}

/**
 * Displays the main window
 * @param *this The object to be displayed
 */
void  MainWindow_presentation (MainWindow *this)
{
  GtkWidget *wnd_main;
  GtkWidget *vbx_main;
  GtkWidget *mnb_main;
  GtkWidget *mni_file;
  GtkWidget *mni_file_menu;
  GtkWidget *mni_new;
  GtkWidget *mni_separator;
  GtkWidget *mni_quit;
  GtkWidget *mni_help;
  GtkWidget *mni_help_menu;
  GtkWidget *mni_about;
  GtkWidget *ntb_main;
  GtkAccelGroup *accel_group;
  Global *global = Global_get();
  g_return_if_fail(global->parent == NULL);


  accel_group = gtk_accel_group_new ();

  wnd_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (wnd_main), _("copos"));
  gtk_window_set_default_size (GTK_WINDOW (wnd_main), 480, 640);

  vbx_main = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbx_main);
  gtk_container_add (GTK_CONTAINER (wnd_main), vbx_main);

  mnb_main = gtk_menu_bar_new ();
  gtk_widget_show (mnb_main);
  gtk_box_pack_start (GTK_BOX (vbx_main), mnb_main, FALSE, FALSE, 0);

  mni_file = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (mni_file);
  gtk_container_add (GTK_CONTAINER (mnb_main), mni_file);

  mni_file_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (mni_file), mni_file_menu);

  mni_new = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
  gtk_widget_show (mni_new);
  gtk_container_add (GTK_CONTAINER (mni_file_menu), mni_new);

  mni_separator = gtk_separator_menu_item_new ();
  gtk_widget_show (mni_separator);
  gtk_container_add (GTK_CONTAINER (mni_file_menu), mni_separator);
  gtk_widget_set_sensitive (mni_separator, FALSE);

  mni_quit = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (mni_quit);
  gtk_container_add (GTK_CONTAINER (mni_file_menu), mni_quit);

  mni_help = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (mni_help);
  gtk_container_add (GTK_CONTAINER (mnb_main), mni_help);

  mni_help_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (mni_help), mni_help_menu);

  mni_about = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (mni_about);
  gtk_container_add (GTK_CONTAINER (mni_help_menu), mni_about);

  ntb_main = gtk_notebook_new ();
  gtk_widget_show (ntb_main);
  gtk_box_pack_start (GTK_BOX (vbx_main), ntb_main, TRUE, TRUE, 0);

  global->parent = g_object_ref(ntb_main);

  g_signal_connect ((gpointer) wnd_main, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) mni_new, "activate",
                    G_CALLBACK (on_mni_new_activate),
                    this);
  g_signal_connect ((gpointer) mni_quit, "activate",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) mni_about, "activate",
                    G_CALLBACK (on_mni_about_activate),
                    this);

  gtk_window_add_accel_group (GTK_WINDOW (wnd_main), accel_group);

  gtk_widget_show(wnd_main);
}
