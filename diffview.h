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
    
    QMenuBar *mainMenu;
    
    QSyntaxHighlighter *highlighter;
    
public:
    diffview(QWidget *parent = 0);
    ~diffview();
    
public slots:
    void Open();
};

#endif // DIFFVIEW_H
