/****************************************************************************
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
** http://doc.qt.digia.com/qq/qq28-qthelp.html
****************************************************************************/

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>
#include <QDebug>

class HelpBrowser : public QTextBrowser
{
public:
    QVariant loadResource (int type, const QUrl& name);
    HelpBrowser(QHelpEngine *helpEngine, QWidget *parent);
private:
    QHelpEngine* helpEngine;
};

#endif // HELPBROWSER_H
