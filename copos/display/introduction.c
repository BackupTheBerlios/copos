#include "display/introduction.h"

/* Load different style for the text */
static void create_tags (GtkTextBuffer *buffer)
{
  gtk_text_buffer_create_tag (buffer, "heading",
			      "weight", PANGO_WEIGHT_BOLD,
			      "size", 15 * PANGO_SCALE,
			      NULL);
  gtk_text_buffer_create_tag (buffer, "italic",
			      "style", PANGO_STYLE_ITALIC, NULL);
  gtk_text_buffer_create_tag (buffer, "bold",
			      "weight", PANGO_WEIGHT_BOLD, NULL);  
  gtk_text_buffer_create_tag (buffer, "big_gap_before_line",
			      "pixels_above_lines", 30, NULL);
  gtk_text_buffer_create_tag (buffer, "big_gap_after_line",
			      "pixels_below_lines", 30, NULL);

  gtk_text_buffer_create_tag (buffer, "word_wrap",
			      "wrap_mode", GTK_WRAP_WORD, NULL);

  gtk_text_buffer_create_tag (buffer, "char_wrap",
			      "wrap_mode", GTK_WRAP_CHAR, NULL);

  gtk_text_buffer_create_tag (buffer, "no_wrap",
			      "wrap_mode", GTK_WRAP_NONE, NULL);
  gtk_text_buffer_create_tag (buffer, "center",
			      "justification", GTK_JUSTIFY_CENTER, NULL);
  gtk_text_buffer_create_tag (buffer, "right_justify",
			      "justification", GTK_JUSTIFY_RIGHT, NULL);
  gtk_text_buffer_create_tag (buffer, "wide_margins",
			      "left_margin", 25, "right_margin", 25,
			      NULL);
}

/* Insert the text and the balise in the text buffer */
static void insert_text (GtkTextBuffer *buffer)
{
  GtkTextIter iter;
  GtkTextIter start, end;

  gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);

  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, _("The cheapest 3D Scanner !\n"), -1, "heading", "wide_margins", "big_gap_before_line", NULL);
  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, _("The next steps are :\n-Choose a video file\n-Select the laser on picture\n-Enter the position of each piece in the real world\n-Check out if these values are correct\n-Enter the last value and generate the cloud of points\n-Visualize the result and save it\n\nOk now, click on the forward button"), -1, "wide_margins", NULL);
  gtk_text_buffer_get_bounds (buffer, &start, &end);
  gtk_text_buffer_apply_tag_by_name (buffer, "word_wrap", &start, &end);
}



static void
on_btn_forward_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /* Variables and pre-cond */
  Introduction *this;
  g_return_if_fail(GTK_IS_BUTTON(button));
  g_return_if_fail(user_data != NULL);
  this = (Introduction*) user_data;
  /* Code */
  if(ChooseFile_isDisplayed(this->chooseFile)) {
    ChooseFile_destroy(this->chooseFile);
  }
  this->chooseFile = ChooseFile_new();
  ChooseFile_presentation(this->chooseFile);
}


/**
 * Constructor
 */
Introduction*  Introduction_new ()
{
  /* Variables and pre-cond */
  Introduction* ret = (Introduction*) g_malloc(sizeof(Introduction));
  /* Code */
  ret->mainWidget = NULL;
  ret->chooseFile = NULL;
  return ret;
}

  

/**
 * Destructor
 * @param *this The object to be destroy
 */
void  Introduction_destroy (Introduction *this)
{
  /* Variables and pre-cond */
  g_assert(this != NULL);
  /* Code */
  if(Introduction_isDisplayed(this)) {
    gtk_widget_destroy(this->mainWidget);
  }
  if(this->chooseFile != NULL) {
    ChooseFile_destroy(this->chooseFile);
  }
  g_free(this);
}

  

/**
 * Test if the page is displayed
 * @param *this The object to be tested
 */
gboolean  Introduction_isDisplayed (Introduction *this)
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
void  Introduction_presentation (Introduction *this)
{
  /* Variables and pre-cond */
  GtkWidget *scw_introduction;
  GtkWidget *txv_introduction;
  GtkWidget *hsp_introduction;
  GtkWidget *hbb_introduction;
  GtkWidget *btn_forward;
  GtkTextBuffer *txb_introduction;
  Global *global = Global_get();
  g_return_if_fail(GTK_IS_CONTAINER(global->parent));
  /* Code */

  this->mainWidget = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (this->mainWidget);
  gtk_container_add (GTK_CONTAINER (global->parent), this->mainWidget);

  scw_introduction = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scw_introduction);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), scw_introduction, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scw_introduction), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  
  txv_introduction = gtk_text_view_new ();
  gtk_widget_show (txv_introduction);
  gtk_container_add (GTK_CONTAINER (scw_introduction), txv_introduction);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (txv_introduction), FALSE);
  gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (txv_introduction), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (txv_introduction), FALSE);
  txb_introduction = gtk_text_view_get_buffer (GTK_TEXT_VIEW (txv_introduction));
  /* Add the text with the static functions */
  create_tags (txb_introduction);
  insert_text (txb_introduction);

  hsp_introduction = gtk_hseparator_new ();
  gtk_widget_show (hsp_introduction);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hsp_introduction, FALSE, FALSE, 0);

  hbb_introduction = gtk_hbutton_box_new ();
  gtk_widget_show (hbb_introduction);
  gtk_box_pack_start (GTK_BOX (this->mainWidget), hbb_introduction, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbb_introduction), 5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbb_introduction), GTK_BUTTONBOX_END);

  btn_forward = gtk_button_new_from_stock ("gtk-go-forward");
  gtk_widget_show (btn_forward);
  gtk_container_add (GTK_CONTAINER (hbb_introduction), btn_forward);
  GTK_WIDGET_SET_FLAGS (btn_forward, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) btn_forward, "clicked",
                    G_CALLBACK (on_btn_forward_clicked),
                    this);

  Global_setLabel(_("Introduction"));
}
