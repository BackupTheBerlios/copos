/****************************************************************************
 ** Form implementation generated from reading ui file 'dlggenerate.ui'
 **
 ** Created: mer nov 30 17:57:05 2005
 **      by: The User Interface Compiler ($Id: dlggenerate.cpp,v 1.1 2006/01/16 18:28:15 cirdan Exp $)
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/

#include "./dlggenerate.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qslider.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <iostream>
#include <fstream>

using namespace display;


DlgGenerate::DlgGenerate( QWidget* parent, const char* name, bool modal, WFlags fl)
  : QDialog( parent, name, modal, fl )
{
  if ( !name )
    setName( "DlgGenerate" );
  DlgGenerateLayout = new QGridLayout( this, 1, 1, 11, 6, "DlgGenerateLayout"); 

  glView = new Viewer( this, "glView" );

  DlgGenerateLayout->addWidget( glView, 0, 1 );

  vbx_menu = new QVBoxLayout( 0, 0, 6, "vbx_menu"); 

  btg_display = new QButtonGroup( this, "btg_display" );
  btg_display->setExclusive( TRUE );
  btg_display->setColumnLayout(0, Qt::Vertical );
  btg_display->layout()->setSpacing( 6 );
  btg_display->layout()->setMargin( 11 );
  btg_displayLayout = new QVBoxLayout( btg_display->layout() );
  btg_displayLayout->setAlignment( Qt::AlignTop );

  rdb_flat = new QRadioButton( btg_display, "rdb_flat" );
  rdb_flat->setChecked( TRUE );
  btg_displayLayout->addWidget( rdb_flat );

  rdb_wire = new QRadioButton( btg_display, "rdb_wire" );
  btg_displayLayout->addWidget( rdb_wire );
  vbx_menu->addWidget( btg_display );

  gbx_file = new QGroupBox( this, "gbx_file" );
  gbx_file->setColumnLayout(0, Qt::Vertical );
  gbx_file->layout()->setSpacing( 6 );
  gbx_file->layout()->setMargin( 11 );
  gbx_fileLayout = new QVBoxLayout( gbx_file->layout() );
  gbx_fileLayout->setAlignment( Qt::AlignTop );

  btn_save = new QPushButton( gbx_file, "btn_save" );
  gbx_fileLayout->addWidget( btn_save );

  btn_exit = new QPushButton( gbx_file, "btn_exit" );
  gbx_fileLayout->addWidget( btn_exit );
  vbx_menu->addWidget( gbx_file );
//   if(widget) {
//     vbx_menu->addWidget( sld_alpha );
//   }
//   else {
//     spacer = new QSpacerItem( 21, 111, QSizePolicy::Minimum, QSizePolicy::Expanding );
//     vbx_menu->addItem( spacer );
//   }

  DlgGenerateLayout->addLayout( vbx_menu, 0, 0 );
  languageChange();
  resize( QSize(662, 502).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );

  // signals and slots connections
  connect( btn_exit, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( btn_save, SIGNAL( clicked() ), this, SLOT( btn_save_clicked() ) );
  connect( rdb_flat, SIGNAL( toggled(bool) ), this, SLOT( rdb_flat_toggled(bool) ) );
  connect( rdb_flat, SIGNAL( toggled(bool) ), glView, SLOT(updateGL ()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
DlgGenerate::~DlgGenerate()
{
  // no need to delete child widgets, Qt does it all for us
  for(std::list<RenderObject*>::iterator iter = objects.begin();
      iter != objects.end();
      ++iter) {
    delete *iter;
  }
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DlgGenerate::languageChange()
{
  setCaption( tr( "Generate" ) );
  btg_display->setTitle( tr( "Display" ) );
  rdb_wire->setText( tr( "Wire" ) );
  rdb_flat->setText( tr( "Flat" ) );
  gbx_file->setTitle( tr( "File" ) );
  btn_exit->setText( tr( "Exit" ) );
  btn_save->setText( tr( "Save..." ) );
}

void DlgGenerate::rdb_flat_toggled( bool isFlat)
{
  glView->setFlat(isFlat);
}


void DlgGenerate::btn_save_clicked()
{
  QString file = QFileDialog::getSaveFileName(NULL, // QDir::home().path(),
					      "Wavefront Object (*.obj)",
					      this,
					      "save file dialog"
					      "Choose a filename to save under" );
  if (! file.isEmpty()) {
    QFileInfo file_info( file );
    if (file_info.extension( FALSE ) != "obj")
      file += ".obj";
    std::ofstream o(file);
    o << "#" << std::endl;
    o << "#  Wavefront OBJ generated by COPOS" << std::endl;
    o << "#  " << std::endl;
    o << "#  COPOS by Ronan Billon" << std::endl;
    o << "#  email: cirdan@mail.berlios.de" << std::endl;
    o << "#  www:   http://copos.berlios.de" << std::endl;
    o << "#  " << std::endl;
    glView->dumpObjects(o);
    o.close();
  }
}

void DlgGenerate::addObject (RenderObject* object)
{
  objects.push_back(object);
  glView->addObject(object);
}

void  DlgGenerate::addWidget (QWidget* widget)
{
  vbx_menu->addWidget(widget);
}