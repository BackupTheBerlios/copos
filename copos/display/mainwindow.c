/************************************************************************
* Fichier          : mainwindow.c
* Date de Creation : mar aoû 10 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on mar aoû 10 2004 at 11:05:55 with umbrello
**************************************************************************/

#include "display/mainwindow.h"

static void
on_new_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  MainWindow* this;
  g_return_if_fail(GTK_IS_MENU_ITEM(menuitem));
  g_return_if_fail(user_data != NULL);
  this = (MainWindow*) user_data;
  /* Code */
  /* Reload the first page */
  if(Introduction_isDisplayed(this->introduction)) {
    Introduction_destroy(this->introduction);
  }
  this->introduction = Introduction_new();  
  Introduction_presentation(this->introduction);
}


static void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  MainWindow* this;
  g_return_if_fail(GTK_IS_MENU_ITEM(menuitem));
  g_return_if_fail(user_data != NULL);
  this = (MainWindow*) user_data;
  /* Code */
  /* TODO display a dialogue box */
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
  if(this->introduction != NULL) { 
    Introduction_destroy(this->introduction);
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
  GtkWidget *sp_mni;
  GtkWidget *mni_quit;
  GtkWidget *mni_help;
  GtkWidget *mni_help_menu;
  GtkWidget *mni_about;
  GtkWidget *ntb_main;
  GtkWidget *stt_main;
  GtkAccelGroup *accel_group;
  Global *global = Global_get();
  g_return_if_fail(global->parent == NULL);

  accel_group = gtk_accel_group_new ();

  wnd_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (wnd_main), _("Copos"));
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

  sp_mni = gtk_menu_item_new ();
  gtk_widget_show (sp_mni);
  gtk_container_add (GTK_CONTAINER (mni_file_menu), sp_mni);
  gtk_widget_set_sensitive (sp_mni, FALSE);

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

  stt_main = gtk_statusbar_new ();
  gtk_widget_show (stt_main);
  gtk_box_pack_start (GTK_BOX (vbx_main), stt_main, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) mni_new, "activate",
                    G_CALLBACK (on_new_activate),
                    this);
  g_signal_connect ((gpointer) mni_quit, "activate",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) mni_about, "activate",
                    G_CALLBACK (on_about_activate),
                    this);
  gtk_widget_show(wnd_main);
}
