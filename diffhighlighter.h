#ifndef DIFFHIGHLIGHTER_H
#define DIFFHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class DiffHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
    
    QList< QPair<QRegExp,QTextCharFormat> > rules;
    
public:
    DiffHighlighter(QTextDocument *parent = 0);
    
    void enableRulesBackbround(bool on);
    
protected:
    void highlightBlock(const QString &text);
};

#endif // DIFFHIGHLIGHTER_H
