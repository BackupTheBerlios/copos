/************************************************************************
* Fichier          : confvidctrl.c
* Date de Creation : sam mai 22 2004
* Auteur           : Ronan Billon
* E-mail           : cirdan@mail.berlios.de

This file was generated on sam mai 22 2004 at 11:07:41 with umbrello
**************************************************************************/

#include "confvidctrl.h"
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "ConfVidCtrl"

void on_hsc_brightness_value_changed(GtkRange        *range,
				     gpointer         user_data)
{
  struct video_picture    vpicture;
  Webcam *webcam = (Webcam*) user_data;
  g_assert(GTK_IS_RANGE(range));

  if(ioctl(webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    return;
  }
  vpicture.brightness = gtk_range_get_value(GTK_RANGE(range));
  if(ioctl(webcam->device, VIDIOCSPICT, &vpicture) < 0) {
    g_warning("Impossible d'ecrire la configuration picture\n");
    return;
  }
}

void on_hsc_colour_value_changed(GtkRange        *range,
				 gpointer         user_data)
{
  struct video_picture    vpicture;
  Webcam *webcam = (Webcam*) user_data;
  g_assert(GTK_IS_RANGE(range));

  if(ioctl(webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    return;
  }
  vpicture.colour = gtk_range_get_value(GTK_RANGE(range));
  if(ioctl(webcam->device, VIDIOCSPICT, &vpicture) < 0) {
    g_warning("Impossible d'ecrire la configuration picture\n");
    return;
  }
}

void on_hsc_contrast_value_changed(GtkRange        *range,
				 gpointer         user_data)
{
  struct video_picture    vpicture;
  Webcam *webcam = (Webcam*) user_data;
  g_assert(GTK_IS_RANGE(range));
  
  if(ioctl(webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    return;
  }
  vpicture.contrast = gtk_range_get_value(GTK_RANGE(range));
  if(ioctl(webcam->device, VIDIOCSPICT, &vpicture) < 0) {
    g_warning("Impossible d'ecrire la configuration picture\n");
    return;
  }
}

void on_hsc_hue_value_changed(GtkRange        *range,
				 gpointer         user_data)
{
  struct video_picture    vpicture;
  Webcam *webcam = (Webcam*) user_data;
  g_assert(GTK_IS_RANGE(range));

  if(ioctl(webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    return;
  }
  vpicture.hue = gtk_range_get_value(GTK_RANGE(range));
  if(ioctl(webcam->device, VIDIOCSPICT, &vpicture) < 0) {
    g_warning("Impossible d'ecrire la configuration picture\n");
    return;
  }
}

/**
 * Affiche les widgets
 * @param *this L'objet
 * @param *parent Le containeur recevant les widgets
 */
void  ConfVidCtrl_presentation (Webcam *webcam, GtkContainer *parent)
{
  GtkWidget *tbl_vidControl;
  GtkWidget *lbl_brightness;
  GtkWidget *lbl_hue;
  GtkWidget *lbl_colour;
  GtkWidget *lbl_contrast;
  GtkWidget *hsc_brightness;
  GtkWidget *hsc_colour;
  GtkWidget *hsc_contrast;
  GtkWidget *hsc_hue;

  struct video_picture vpicture;

  if(ioctl(webcam->device, VIDIOCGPICT, &vpicture) < 0) {
    g_warning("Impossible de lire la configuration picture\n");
    return;
  }
  
  tbl_vidControl = gtk_table_new (4, 2, FALSE);
  gtk_widget_show (tbl_vidControl);
  gtk_container_add (GTK_CONTAINER (parent), tbl_vidControl);
  gtk_container_set_border_width (GTK_CONTAINER (tbl_vidControl), 10);

  lbl_brightness = gtk_label_new ("brightness : ");
  gtk_widget_show (lbl_brightness);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), lbl_brightness, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_brightness), 0, 0.5);

  lbl_hue = gtk_label_new ("hue : ");
  gtk_widget_show (lbl_hue);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), lbl_hue, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_hue), 0, 0.5);

  lbl_colour = gtk_label_new ("colour : ");
  gtk_widget_show (lbl_colour);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), lbl_colour, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_colour), 0, 0.5);

  lbl_contrast = gtk_label_new ("contrast : ");
  gtk_widget_show (lbl_contrast);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), lbl_contrast, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (lbl_contrast), 0, 0.5);

  hsc_brightness = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (vpicture.brightness, 0, 65535, 1, 10, 0)));
  gtk_widget_show (hsc_brightness);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), hsc_brightness, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 10, 0);
  gtk_scale_set_digits (GTK_SCALE (hsc_brightness), 0);

  hsc_colour = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (vpicture.colour, 0, 65535, 1, 0, 0)));
  gtk_widget_show (hsc_colour);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), hsc_colour, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 10, 0);
  gtk_scale_set_digits (GTK_SCALE (hsc_colour), 0);

  hsc_contrast = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (vpicture.contrast, 0, 65535, 1, 0, 0)));
  gtk_widget_show (hsc_contrast);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), hsc_contrast, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 10, 0);
  gtk_scale_set_digits (GTK_SCALE (hsc_contrast), 0);

  hsc_hue = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (vpicture.hue, 0, 65535, 1, 10, 0)));
  gtk_widget_show (hsc_hue);
  gtk_table_attach (GTK_TABLE (tbl_vidControl), hsc_hue, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 10, 0);
  gtk_scale_set_digits (GTK_SCALE (hsc_hue), 0);

  g_signal_connect ((gpointer) hsc_brightness, "value_changed",
                    G_CALLBACK (on_hsc_brightness_value_changed),
                    webcam);
  g_signal_connect ((gpointer) hsc_colour, "value_changed",
                    G_CALLBACK (on_hsc_colour_value_changed),
                    webcam);
  g_signal_connect ((gpointer) hsc_contrast, "value_changed",
                    G_CALLBACK (on_hsc_contrast_value_changed),
                    webcam);
  g_signal_connect ((gpointer) hsc_hue, "value_changed",
                    G_CALLBACK (on_hsc_hue_value_changed),
                    webcam);
  

}

  

