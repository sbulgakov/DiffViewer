#ifndef DIFFVIEW_H
#define DIFFVIEW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QMenu>
#include <QMenuBar>

class QSyntaxHighlighter;

class diffview : public QWidget
{
    Q_OBJECT
    
    QPlainTextEdit *edit;
    
    QMenu   *fileMenu;
    QAction *fileActOpen;
    QAction *fileActExit;
    
    QMenu   *toolsMenu;
    QAction *toolsActFilter;
    
    QMenuBar *mainMenu;
    
    QSyntaxHighlighter *highlighter;
    
public:
    enum Operation
    {
      add,
      del,
      nop,
      end
    }; 
    
    diffview(QWidget *parent = 0);
    ~diffview();
    
public slots:
    void Open();
    
    void Filter();
    
private:
    QList<diffview::Operation> diff(const QString& a, const QString& b);
    QList<QTextEdit::ExtraSelection> select(const QList<Operation>& s, int pos1, int pos2);
    void replace(int del_beg, int del_end, int add_beg, int add_end, 
                 QList<QTextEdit::ExtraSelection> *extra);
};

#endif // DIFFVIEW_H
