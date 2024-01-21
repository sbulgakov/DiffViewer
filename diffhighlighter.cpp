#include "diffhighlighter.h"

DiffHighlighter::DiffHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{
  /* 
  ** https://www.schemecolor.com/relaxing-to-the-eye-color-palette.php
  ** 
  ** Relaxing To The Eye Color Scheme
  ** 
  ** The Relaxing To The Eye Color Scheme palette has 5 colors which are 
  **   * Water (#CAF4F4), 
  **   * Ivory (#FCFFE9), 
  **   * Blanched Almond (#FFF2CC), 
  **   * Pale Pink (#FDE0D9) 
  **   * and Tea Green (#CAEFD1).
  ** 
  ** This color combination was created by user Ratna. 
  ** The Hex, RGB and CMYK codes are in the table below.
  ** 
  ** Note: English language names are approximate equivalents of the hexadecimal color codes.
  ** 
  ** COLOR	    INFORMATION
  ** --------------------
  ** WATER	    Name: Water
  **            Hex: #CAF4F4
  **            RGB: (202, 244, 244)
  **            CMYK: 0.172, 0, 0, 0.043
  ** 
  ** IVORY	    Name: Ivory
  **            Hex: #FCFFE9
  **            RGB: (252, 255, 233)
  **            CMYK: 0.011, 0, 0.086, 0
  ** 
  ** BLANCHED   Name: Blanched Almond
  ** ALMOND     Hex: #FFF2CC
  **            RGB: (255, 242, 204)
  **            CMYK: 0, 0.050, 0.2, 0
  ** 
  ** PALE PINK  Name: Pale Pink
  **            Hex: #FDE0D9
  **            RGB: (253, 224, 217)
  **            CMYK: 0, 0.114, 0.142, 0.007
  ** 
  ** TEA GREEN  Name: Tea Green
  **            Hex: #CAEFD1
  **            RGB: (202, 239, 209)
  **            CMYK: 0.154, 0, 0.125, 0.062
  ** 
  */
  QColor Ivory(252, 255, 233);
  QColor PalePink(253, 224, 217);
  QColor TeaGreen(202, 239, 209);
  
  QRegExp exp;
  QTextCharFormat fmt;
  
  fmt.setBackground(Ivory);
  
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
  fmt.setForeground(QColor(Qt::green).darker(200));
  //fmt.setBackground(QColor(Qt::green).lighter(175));
  fmt.setBackground(TeaGreen);
  rules.append(qMakePair(exp,fmt));
  //-...
  exp.setPattern("^-.*$");
  fmt.setForeground(QColor(Qt::red).darker(200));
  //fmt.setBackground(QColor(Qt::red).lighter(175));
  fmt.setBackground(PalePink);
  rules.append(qMakePair(exp,fmt));
  
  fmt.setBackground(Ivory);
  
  //--- a/file
  exp.setPattern("^--- .*$");
  fmt.setFontWeight(QFont::Normal);
  fmt.setForeground(Qt::darkCyan);
  rules.append(qMakePair(exp,fmt));
  //+++ b/file
  exp.setPattern("^\\+\\+\\+ .*$");
  rules.append(qMakePair(exp,fmt));
  
  //spaces
  exp.setPattern("[ ]+$");
  fmt.setBackground(QColor(Qt::red));
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
