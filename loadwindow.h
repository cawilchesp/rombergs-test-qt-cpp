#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QDialog>
#include <QtCore>
#include <QFileSystemModel>

namespace Ui {
class LoadWindow;
}

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(QWidget *parent = 0);
    ~LoadWindow();

    QStringList textFileName , mainEstudios, fileNames;
    QString textEstudioName;
    int languageSet, temaDefault;

public slots:
    void receiveLoadEstudios(QStringList &estudios);

signals:
    void sendLoadEstudios(QVector<QStringList> &);

private slots:
    void on_btnAgregar_clicked();

    void on_btnCancelar_clicked();

    void on_treeView_expanded();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_editNombreEstudio_textChanged();

private:
    Ui::LoadWindow *ui;

    QFileSystemModel *dirmodel;
};

#endif // LOADWINDOW_H
