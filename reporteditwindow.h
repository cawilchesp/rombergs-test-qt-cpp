#ifndef REPORTEDITWINDOW_H
#define REPORTEDITWINDOW_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class ReportEditWindow;
}

class ReportEditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReportEditWindow(QWidget *parent = nullptr);
    ~ReportEditWindow();

    int languageSet, temaDefault;
    QString dataFolder, resultsFolder;
    QStringList reportTexts;

public slots:


private slots:
    void on_btnAceptar_clicked();
    void on_btnCancelar_clicked();

private:
    Ui::ReportEditWindow *ui;
};

#endif // REPORTEDITWINDOW_H
