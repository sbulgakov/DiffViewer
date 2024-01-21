#include "diffview.h"
#include "diffhighlighter.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFileDialog>

#include <QString>
#include <QList>

QList<diffview::Operation> diffview::diff(const QString& a, const QString& b)
{
  //
  int rows = b.length();
  int cols = a.length();
  
  
  QList< QList<Operation> > list;
  
  
  //----
  QList<Operation> script;
  int len = 0;
  int ops = 0;
  //
  int max   = -1;
  int max_i = -1;
  int min   = rows+cols+1;
  int min_i = -1;
  for(int i = 0; i < rows; ++i)
  {
    int r = i;
    int c = 0;
    
    while(c<cols)
    {
      if(b[r]==a[c])
      {
        script.append(nop);
        ++r;
        ++c;
        ++len;
        continue;
      }
     
      script.append(del);
      ++c;
      ++ops;
    }
    while(r<rows)
    {
      script.append(add);
      ++r;
      ++ops;
    }
    
    script.append(end);
    list.append(script);
    if(len > max){ max = len; max_i = i; }
    if(ops < min){ min = ops; min_i = i; }
    script.clear();
    len = 0;
    ops = 0;
    for(int j = 0; j <=i; ++j)
    {
      script.append(add);
      ++ops;
    }
  }
  //----
  
  
  if(max > 2) return list.at(max_i);
  else
  {
    QList<Operation> empty;
    empty.append(end);
    return empty;
  }
}

QList<QTextEdit::ExtraSelection> diffview::select(const QList<Operation>& s, int pos1, int pos2)
{
  QList<QTextEdit::ExtraSelection> list;
  
  QTextCursor cur = edit->textCursor();
  
  QTextCharFormat red;
  red.setBackground(QColor(Qt::red).lighter(175));
  
  QTextCharFormat grn;
  grn.setBackground(QColor(Qt::green).lighter(175));
  
  //
  int pos_beg = 0;
  int pos_end = 0;
  Operation o = s.at(0);
  for(int i=0; i < s.size(); ++i)
  {
    if(s.at(i) == o) ++pos_end;
    else
    {
      QTextEdit::ExtraSelection sel;
      int pos;
      int len = pos_end-pos_beg;
      if(o == del || o == nop) pos = pos1;
      else                     pos = pos2;
      cur.setPosition(pos + pos_beg);
      cur.setPosition(pos + pos_end, QTextCursor::KeepAnchor);
      switch(o)
      {
        case add: sel.cursor = cur;
                  sel.format = grn;
                  list.append(sel);
                  pos2 += len;
                  break;
        case del: sel.cursor = cur;
                  sel.format = red;
                  list.append(sel);
                  pos1 += len;
                  break;
        case nop: pos1 += len;
                  pos2 += len;
                  break;
      }
      pos_beg = 0;
      pos_end = 1;
      o = s.at(i);
    }
  }
  //
  
  return list;
}

void diffview::replace(int del_beg, int del_end, int add_beg, int add_end, 
                       QList<QTextEdit::ExtraSelection> *extra)
{
  //qDebug() << "REP:" << "-:" << del_beg << "," << del_end
  //                   << "+:" << add_beg << "," << add_end;
  
  int pos1 = edit->document()->findBlockByNumber(del_beg).position();
  QString a;
  while(del_beg <= del_end)
  {
    a += edit->document()->findBlockByNumber(del_beg++).text();
    a += '\n';
  }
  
  int pos2 = edit->document()->findBlockByNumber(add_beg).position();
  QString b;
  while(add_beg <= add_end)
  {
    b += edit->document()->findBlockByNumber(add_beg++).text();
    b += '\n';
  }
  
  QList<diffview::Operation> eds = diff(a,b);
  if(eds.size() > 1)
  {
    extra->append(select(eds, pos1, pos2));
  }
}

diffview::diffview(QWidget *parent)
    : QWidget(parent)
{
  edit = new QPlainTextEdit(this);
  edit->setReadOnly(true);
  edit->setLineWrapMode(QPlainTextEdit::NoWrap);
  
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);
  edit->setFont(font);
  
  QPalette pal = edit->palette();
  pal.setColor(QPalette::Base, QColor(252, 255, 233)); //Ivory
  edit->setPalette(pal);
  
  mainMenu = new QMenuBar(this);
  
  fileMenu = mainMenu->addMenu(tr("&File"));
  
  fileActOpen = fileMenu->addAction(tr("&Open"), this, SLOT(Open()));
  fileActExit = fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()));
  
  toolsMenu = mainMenu->addMenu(tr("&Tools"));
  
  toolsActFilter = toolsMenu->addAction(tr("&Filter"), this, SLOT(Filter()));
  
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

void diffview::Filter()
{
  QList<QTextEdit::ExtraSelection> extraSelections;
  
  QTextBlock block = edit->document()->begin();
  
  int  del_beg  = -1;
  int  del_end  = -1;
  bool del_flag = false;
  int  add_beg  = -1;
  int  add_end  = -1;
  bool add_flag = false;
  
  while(block != edit->document()->end())
  {
    QString line = block.text();
    
    /* 
    ** DIF -+--> INF (index)
    **      |     v
    **      +--> OLD (---)
    ** 
    ** OLD ----> NEW (+++)
    ** 
    ** NEW ----> LOC (@@)
    ** 
    ** LOC -+--> CTX ( )
    **      |
    **      +--> DEL (-)
    **      |     v
    **      +--> ADD (+)
    ** 
    */
    if(line.startsWith(QString("diff")))
    {
      //qDebug() << "DIF:" << line;
      if(del_flag && add_flag)
      {
        replace(del_beg, del_end, add_beg, add_end, &extraSelections);
      }
      del_beg  = -1;
      del_end  = -1;
      del_flag = false;
      add_beg  = -1;
      add_end  = -1;
      add_flag = false;
    }
    else if(line.startsWith(QString("---")))
    {
      //qDebug() << "OLD:" << line;
    }
    else if(line.startsWith(QString("+++")))
    {
      //qDebug() << "NEW:" << line;
    }
    else if(line.startsWith(QString("@@")))
    {
      //qDebug() << "LOC:" << line;
      if(del_flag && add_flag)
      {
        replace(del_beg, del_end, add_beg, add_end, &extraSelections);
      }
      del_beg  = -1;
      del_end  = -1;
      del_flag = false;
      add_beg  = -1;
      add_end  = -1;
      add_flag = false;
    }
    else if(line.startsWith(QChar(' ')))
    {
      //qDebug() << "CTX:" << line;
      if(del_flag && add_flag)
      {
        replace(del_beg, del_end, add_beg, add_end, &extraSelections);
      }
      del_beg  = -1;
      del_end  = -1;
      del_flag = false;
      add_beg  = -1;
      add_end  = -1;
      add_flag = false;
    }
    else if(line.startsWith(QChar('-')))
    {
      //qDebug() << "DEL:" << line;
      if(!del_flag){ del_beg = block.blockNumber(); del_flag = true;}
      if( del_flag)  del_end = block.blockNumber();
    }
    else if(line.startsWith(QChar('+')))
    {
      //qDebug() << "ADD:" << line;
      if(!add_flag){ add_beg = block.blockNumber(); add_flag = true;}
      if( add_flag)  add_end = block.blockNumber();
    }
    else
    {
      //qDebug() << "UNC:" << line;
    }
    
    block = block.next();
  }
  
  edit->setExtraSelections(extraSelections);
}
