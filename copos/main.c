/* Fichier          : /home/bug/projet/copos2/src/main.c		*/
/* Date de Creation : 07/10/2005			*/
/* Auteur           : Ronan Billon			*/
/* E-mail           : r.billon@caramail.com		*/
/* Commentaires     : 				*/

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "global.h"
#include "display/mainwindow.h"

int main(int argc, char *argv[])
{
  MainWindow* mainWin  = MainWindow_new();
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_init (&argc, &argv);
  gtk_gl_init (&argc, &argv);

  MainWindow_presentation(mainWin);
  gtk_main();
  MainWindow_destroy(mainWin);
  Global_destroy();
  return EXIT_SUCCESS;
}
