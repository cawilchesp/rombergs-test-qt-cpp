#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLabel>
#include <QTranslator>

#include <opencv2/core/core.hpp>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() Q_DECL_OVERRIDE;

    int languageSet, graphNumber, temaDefault;
    QString dataFolder, resultsFolder;
    QStringList estudios;
    QList<QVector<QVector<double>>> estudiosSenales;
    QVector<QLabel*> labelsParametros;

    QList<QColor> graphColors;

    // Reporte ----------------------------------
    QStringList reportTexts;

public slots:
    void receiveEstudios(QVector<QStringList> &dataEstudios);
    void receiveAnalisis(QVector<QStringList> &dataEstudios);
    void generarImagenes(QString selectedFolder);

    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

signals:
    void sendEstudios(QStringList &);

private slots:
    // Archivo
    void on_btnNuevoEstudio_clicked();
    void on_btnAnalisis_clicked();
    void on_btnGenerarReporte_clicked();
    void on_btnEditarReporte_clicked();

    // Configuración
    void on_temaComboBox_currentIndexChanged(int index);
    void on_languageComboBox_activated(int index);
    void on_btnDatosFolderPath_clicked();
    void on_btnResultsFolderPath_clicked();

    // Paciente
    void on_btnNuevo_clicked();
    void on_btnCargar_clicked();
    void on_btnGuardar_clicked();

    // Datos
    void on_editApellidos_textChanged(const QString &arg1);
    void on_editNombres_textChanged(const QString &arg1);
    void on_spinPeso_valueChanged(double arg1);
    void on_spinAltura_valueChanged(double arg1);
    void on_cmbxPeso_currentIndexChanged(int index);
    void on_cmbxAltura_currentIndexChanged(int index);
    void on_editOcupacion_textChanged(const QString &arg1);
    void on_editProyecto_textChanged(const QString &arg1);
    void on_editEvaluador_textChanged(const QString &arg1);

    // Gráficas
    void on_cmbxParametros_currentIndexChanged(int index);

    // Ayuda
    void on_manualButton_clicked();
    void on_aboutButton_clicked();
    void on_aboutQtButton_clicked();

    // Funciones
    QVector<QVector<double> > ellipseStandard(QVector<QVector<double> > dataSenales);
    QVector<QVector<double> > convexHull(QVector<QVector<double> > dataSenales);
    QVector<QVector<double> > ellipsePCA(QVector<QVector<double> > dataSenales);

private:
    Ui::MainWindow *ui;

    QTranslator T;
};

#endif // MAINWINDOW_H
