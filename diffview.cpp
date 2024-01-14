#include "diffview.h"
#include "diffhighlighter.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFileDialog>

diffview::diffview(QWidget *parent)
    : QWidget(parent)
{
  edit = new QPlainTextEdit(this);
  edit->setReadOnly(true);
  
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);
  edit->setFont(font);
  
  mainMenu = new QMenuBar(this);
  
  fileMenu = mainMenu->addMenu(tr("&File"));
  
  fileActOpen = fileMenu->addAction(tr("&Open"), this, SLOT(Open()));
  fileActExit = fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()));
  
  QVBoxLayout *l = new QVBoxLayout(this);
  l->addWidget(edit);
  l->setMenuBar(mainMenu);
  l->setMargin(0);
  
  highlighter = new DiffHighlighter(edit->document());
}

diffview::~diffview()
{
    
}

void diffview::Open()
{
  QString fileName;
  
  if(fileName.isNull())
    fileName = QFileDialog::getOpenFileName(this,
      tr("Open File"), "", 
      "diff files (*.diff *.patch);;all files (*)");
    
  if(!fileName.isEmpty())
  {
    QFile file(fileName);
    if(file.open(QFile::ReadOnly|QFile::Text))
      edit->setPlainText(file.readAll());
  }
}
