#include "diffhighlighter.h"

DiffHighlighter::DiffHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{
  QRegExp exp;
  QTextCharFormat fmt;
  
  //diff ... a/file b/file
  exp.setPattern("^diff .*$");
  fmt.setFontWeight(QFont::Bold);
  fmt.setForeground(Qt::darkBlue);
  rules.append(qMakePair(exp,fmt));
  
  //index ...
  exp.setPattern("^index .*$");
  fmt.setFontWeight(QFont::Normal);
  fmt.setForeground(Qt::gray);
  rules.append(qMakePair(exp,fmt));
  
  //@@ ... @@ ...
  exp.setPattern("^@@ [^@]*@@");
  fmt.setForeground(Qt::magenta);
  rules.append(qMakePair(exp,fmt));
  
  //+...
  exp.setPattern("^\\+.*$");
  fmt.setFontWeight(QFont::Bold);
  fmt.setForeground(Qt::green);
  rules.append(qMakePair(exp,fmt));
  //-...
  exp.setPattern("^-.*$");
  fmt.setForeground(Qt::red);
  rules.append(qMakePair(exp,fmt));
  
  //--- a/file
  exp.setPattern("^--- .*$");
  fmt.setFontWeight(QFont::Normal);
  fmt.setForeground(Qt::darkCyan);
  rules.append(qMakePair(exp,fmt));
  //+++ b/file
  exp.setPattern("^\\+\\+\\+ .*$");
  rules.append(qMakePair(exp,fmt));
  
}

void DiffHighlighter::highlightBlock(const QString &text)
{
  QPair<QRegExp,QTextCharFormat> r;
  
  foreach(r, rules)
  {
    QRegExp exp(r.first);
    int index = exp.indexIn(text);
    while(index >= 0)
    {
      int len = exp.matchedLength();
      setFormat(index, len, r.second);
      index = exp.indexIn(text, index + len);
    }
  }
  
}
