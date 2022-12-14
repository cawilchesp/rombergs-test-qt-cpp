#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QDockWidget>

namespace Ui {
class helpWindow;
}

class helpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit helpWindow(QWidget *parent = nullptr);
    ~helpWindow();

private:
    Ui::helpWindow *ui;

    QDockWidget *helpWidget;
};

#endif // HELPWINDOW_H
