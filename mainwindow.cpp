#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadwindow.h"
#include "senales.h"
#include "parametros.h"
#include "helpwindow.h"
#include "reporteditwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
#include <QPainter>
#include <QPdfWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //---------------------------- Valores de Configuración -----------------------------
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);                                  // Archivo de configuración
    dataFolder = testRombergSettings.value("Data_Folder","C:\\").toString();                             // Carpeta de datos
    resultsFolder = testRombergSettings.value("Results_Folder","C:\\").toString();                       // Carpeta de resultados
    languageSet = testRombergSettings.value("Language",0).toInt();                                       // Idioma seleccionado
    temaDefault = testRombergSettings.value("Theme",0).toInt();
    reportTexts = testRombergSettings.value("Report_Texts").toStringList();

    ui->lblDatosFolderPath->setText(dataFolder);
    ui->lblResultsFolderPath->setText(resultsFolder);

    QRegExp regExp1 ("[A-Za-zÁÉÍÓÚáéíóú ]{1,44}");
    ui->editApellidos->setValidator(new QRegExpValidator (regExp1, this));
    ui->editNombres->setValidator(new QRegExpValidator (regExp1, this));
    ui->editOcupacion->setValidator(new QRegExpValidator (regExp1, this));
    ui->editProyecto->setValidator(new QRegExpValidator (regExp1, this));
    ui->editEvaluador->setValidator(new QRegExpValidator (regExp1, this));

    QRegExp regExp2 ("[0-9]{1,10}");
    ui->editId->setValidator(new QRegExpValidator (regExp2, this));
    graphNumber = 0;

    graphColors.push_back(QColor(0, 114, 189));
    graphColors.push_back(QColor(217, 83, 25));
    graphColors.push_back(QColor(237, 177, 32));
    graphColors.push_back(QColor(126, 47, 142));
    graphColors.push_back(QColor(119, 172, 48));
    graphColors.push_back(QColor(77, 190, 238));
    graphColors.push_back(QColor(162, 20, 47));

    bool oldState = ui->cmbxParametros->blockSignals(true);
    ui->cmbxParametros->addItem("");
    ui->cmbxParametros->blockSignals(oldState);

    // ----------------------------- Títulos de las gráficas ----------------------------
    ui->plotLateral->yAxis->setLabel("Lateral (mm)");
    ui->plotAnteroPosterior->yAxis->setLabel("Antero-Posterior (mm)");
    ui->plotElipse->xAxis->setLabel("Lateral (mm)");
    ui->plotElipse->yAxis->setLabel("Antero-Posterior (mm)");
    ui->plotElipse->replot();
    ui->plotConvex->xAxis->setLabel("Lateral (mm)");
    ui->plotConvex->yAxis->setLabel("Antero-Posterior (mm)");
    ui->plotConvex->replot();
    ui->plotPca->xAxis->setLabel("Lateral (mm)");
    ui->plotPca->yAxis->setLabel("Antero-Posterior (mm)");
    ui->plotPca->replot();

    // ------------------------------ Definición del idioma -----------------------------
    T.load(":/traducciones/ingles.qm");
    ui->languageComboBox->setCurrentIndex(languageSet);
    on_languageComboBox_activated(languageSet);

    // ------------------------------- Definición del tema ------------------------------
    ui->temaComboBox->setCurrentIndex(temaDefault);
    on_temaComboBox_currentIndexChanged(temaDefault);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------------------------------------------------------------------------
// SETTINGS
// ---------------------------------------------------------------------------------------

void MainWindow::on_btnDatosFolderPath_clicked()
{
    //----------------------------- Selección de la carpeta -----------------------------
    QString dataFolderTemp = dataFolder;
    dataFolderTemp = QFileDialog::getExistingDirectory(this, tr("Seleccione la carpeta con los archivos de datos"), dataFolder, QFileDialog::ShowDirsOnly);
    if (dataFolderTemp != "") {
        dataFolder = dataFolderTemp;
    }
    ui->lblDatosFolderPath->setText(dataFolder);
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);
    testRombergSettings.setValue("Data_Folder", dataFolder);
}

void MainWindow::on_btnResultsFolderPath_clicked()
{
    //----------------------------- Selección de la carpeta -----------------------------
    QString resultsFolderTemp = resultsFolder;
    resultsFolderTemp = QFileDialog::getExistingDirectory(this, tr("Seleccione la carpeta donde se guardarán los resultados"), resultsFolder, QFileDialog::ShowDirsOnly);
    if (resultsFolderTemp != "") {
        resultsFolder = resultsFolderTemp;
    }
    ui->lblResultsFolderPath->setText(resultsFolder);
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);
    testRombergSettings.setValue("Results_Folder", resultsFolder);
}

void MainWindow::on_languageComboBox_activated(int index)
{
    //---------------------------------- Configuración ----------------------------------
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);

    //------------------------------ Definición del idioma ------------------------------
    if (index == 0) {
        languageSet = 0;
        testRombergSettings.setValue("Language",languageSet);

        qApp->removeTranslator(&T);
        ui->retranslateUi(this);

        ui->plotLateral->xAxis->setLabel("Tiempo (s)");
        ui->plotAnteroPosterior->xAxis->setLabel("Tiempo (s)");
    } else if (index == 1) {
        languageSet = 1;
        testRombergSettings.setValue("Language",languageSet);

        qApp->installTranslator(&T);
        ui->retranslateUi(this);

        ui->plotLateral->xAxis->setLabel("Time (s)");
        ui->plotAnteroPosterior->xAxis->setLabel("Time (s)");
    }
    ui->plotLateral->replot();
    ui->plotAnteroPosterior->replot();
}

void MainWindow::on_temaComboBox_currentIndexChanged(int index)
{
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);
    if (index == 0) {
        // Tema Claro
        this->setStyleSheet("QMainWindow#MainWindow {background-color: rgb(240,240,240);}");
        ui->frame->setStyleSheet("QFrame#frame {background-color: rgb(240,240,240);}");
        // Configuración ----------------------------------------------------------------
        ui->lblDatosFolderPath->setStyleSheet("QLabel { border-radius:12px; background-color: white; padding: 0 15 0 15 }");
        ui->lblResultsFolderPath->setStyleSheet("QLabel { border-radius:12px; background-color: white; padding: 0 15 0 15 }");
        ui->btnDatosFolderPath->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12;	background-color: rgb(178,178,178)}");
        ui->btnResultsFolderPath->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12;	background-color: rgb(178,178,178)}");
        ui->languageComboBox->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);}");
        ui->temaComboBox->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);}");
        // Paciente ---------------------------------------------------------------------
        ui->btnNuevo->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnCargar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnGuardar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        // Estudio ----------------------------------------------------------------------
        ui->btnGenerarReporte->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnNuevoEstudio->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnAnalisis->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnEditarReporte->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        // Ayuda ------------------------------------------------------------------------
        ui->manualButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->aboutButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        ui->aboutQtButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)}");
        // Datos ------------------------------------------------------------------------
        ui->pacienteTitle->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser1->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser2->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser3->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser4->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser5->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser6->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser7->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser8->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser9->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser10->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser11->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUser12->setStyleSheet("QLabel {color: rgb(0,0,0)}");

        ui->editApellidos->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editNombres->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->cmbxId->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->editId->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->dateNacimiento->setStyleSheet("QDateEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 0 0 12} QDateEdit::up-button {border-color: white} QDateEdit::down-button {border-color: white} QDateEdit::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDateEdit::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDateEdit:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxGenero->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->spinPeso->setStyleSheet("QDoubleSpinBox {border: 0px solid; border-radius: 12; background-color: white; padding: 0 0 0 12} QDoubleSpinBox::up-button {border-color: white} QDoubleSpinBox::down-button {border-color: white} QDoubleSpinBox::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDoubleSpinBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDoubleSpinBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxPeso->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->spinAltura->setStyleSheet("QDoubleSpinBox {border: 0px solid; border-radius: 12; background-color: white; padding: 0 0 0 12} QDoubleSpinBox::up-button {border-color: white} QDoubleSpinBox::down-button {border-color: white} QDoubleSpinBox::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDoubleSpinBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDoubleSpinBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxAltura->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->lblBMI->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->dateSesion->setStyleSheet("QDateEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 0 0 12} QDateEdit::up-button {border-color: white} QDateEdit::down-button {border-color: white} QDateEdit::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDateEdit::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDateEdit:!Enabled {background-color: rgb(178,178,178)}");
        ui->editOcupacion->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editProyecto->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editEvaluador->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");

        // Parámetros -------------------------------------------------------------------
        ui->parametrosTitle->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam1->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam2->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam3->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam4->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam5->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam6->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam7->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam8->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam9->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam10->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam11->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblParam12->setStyleSheet("QLabel {color: rgb(0,0,0)}");

        ui->lblValueParam1->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam2->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam3->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam4->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam5->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam6->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam7->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam8->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam9->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam10->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam11->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblValueParam12->setStyleSheet("QLabel {color: rgb(0,0,0)}");

        ui->lblUnitsParam->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_2->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_3->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_4->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_5->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_6->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_7->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_8->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_9->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_10->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_11->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblUnitsParam_12->setStyleSheet("QLabel {color: rgb(0,0,0)}");

        // Gráficas ---------------------------------------------------------------------
        ui->cmbxParametros->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: white; padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->lblConvexTitle->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblElipseTItle->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->lblPCATitle->setStyleSheet("QLabel {color: rgb(0,0,0)}");
    } else if (index == 1) {
        // Tema Oscuro
        this->setStyleSheet("QMainWindow#MainWindow {background-color: rgb(68,68,68);}");
        ui->frame->setStyleSheet("QFrame#frame {background-color: rgb(178,178,178);}");
        // Configuración ----------------------------------------------------------------
        ui->lblDatosFolderPath->setStyleSheet("QLabel { border-radius:12px; background-color: rgb(240,240,240); padding: 0 15 0 15 }");
        ui->lblResultsFolderPath->setStyleSheet("QLabel { border-radius:12px; background-color: rgb(240,240,240); padding: 0 15 0 15 }");
        ui->btnDatosFolderPath->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240)}");
        ui->btnResultsFolderPath->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240)}");
        ui->languageComboBox->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(240, 240, 240);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);}");
        ui->temaComboBox->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(240, 240, 240);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);}");
        // Paciente ---------------------------------------------------------------------
        ui->btnNuevo->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnCargar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnGuardar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        // Estudio ----------------------------------------------------------------------
        ui->btnGenerarReporte->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnNuevoEstudio->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnAnalisis->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnEditarReporte->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        // Ayuda ------------------------------------------------------------------------
        ui->manualButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12;	background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->aboutButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        ui->aboutQtButton->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)}");
        // Datos ------------------------------------------------------------------------
        ui->pacienteTitle->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser1->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser2->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser3->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser4->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser5->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser6->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser7->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser8->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser9->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser10->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser11->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUser12->setStyleSheet("QLabel {color: rgb(178,178,178)}");

        ui->editApellidos->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editNombres->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->cmbxId->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->editId->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->dateNacimiento->setStyleSheet("QDateEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 0 0 12} QDateEdit::up-button {border-color: white} QDateEdit::down-button {border-color: white} QDateEdit::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDateEdit::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDateEdit:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxGenero->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->spinPeso->setStyleSheet("QDoubleSpinBox {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 0 0 12} QDoubleSpinBox::up-button {border-color: white} QDoubleSpinBox::down-button {border-color: white} QDoubleSpinBox::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDoubleSpinBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDoubleSpinBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxPeso->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->spinAltura->setStyleSheet("QDoubleSpinBox {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 0 0 12} QDoubleSpinBox::up-button {border-color: white} QDoubleSpinBox::down-button {border-color: white} QDoubleSpinBox::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDoubleSpinBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDoubleSpinBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->cmbxAltura->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->lblBMI->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->dateSesion->setStyleSheet("QDateEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 0 0 12} QDateEdit::up-button {border-color: white} QDateEdit::down-button {border-color: white} QDateEdit::up-arrow {image: url(:/imagenes/Imagenes/UpTriangle.png)} QDateEdit::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png)} QDateEdit:!Enabled {background-color: rgb(178,178,178)}");
        ui->editOcupacion->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editProyecto->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");
        ui->editEvaluador->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12} QLineEdit:!Enabled {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178); padding: 0 12 0 12}");

        // Parámetros -------------------------------------------------------------------
        ui->parametrosTitle->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam1->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam2->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam3->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam4->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam5->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam6->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam7->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam8->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam9->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam10->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam11->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblParam12->setStyleSheet("QLabel {color: rgb(178,178,178)}");

        ui->lblValueParam1->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam2->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam3->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam4->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam5->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam6->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam7->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam8->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam9->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam10->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam11->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblValueParam12->setStyleSheet("QLabel {color: rgb(178,178,178)}");

        ui->lblUnitsParam->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_2->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_3->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_4->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_5->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_6->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_7->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_8->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_9->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_10->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_11->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblUnitsParam_12->setStyleSheet("QLabel {color: rgb(178,178,178)}");

        // Gráficas ---------------------------------------------------------------------
        ui->cmbxParametros->setStyleSheet("QComboBox {border: 0px solid; border-radius: 12;	background-color: rgb(240,240,240); padding: 0 12 0 12} QComboBox::drop-down { border-color: rgb(255, 255, 255);} QComboBox::down-arrow {image: url(:/imagenes/Imagenes/DownTriangle.png);} QComboBox:!Enabled {background-color: rgb(178,178,178)}");
        ui->lblConvexTitle->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblElipseTItle->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->lblPCATitle->setStyleSheet("QLabel {color: rgb(178,178,178)}");
    }
    temaDefault = index;
    testRombergSettings.setValue("Theme",temaDefault);
}

// ---------------------------------------------------------------------------------------
// MENU
// ---------------------------------------------------------------------------------------

void MainWindow::on_btnNuevoEstudio_clicked()
{
    LoadWindow loadFileWindow;
    connect(this, SIGNAL(sendEstudios(QStringList&)), &loadFileWindow, SLOT(receiveLoadEstudios(QStringList&)));
    connect(&loadFileWindow , SIGNAL(sendLoadEstudios(QVector<QStringList>&)), this, SLOT(receiveEstudios(QVector<QStringList>&)));
    emit sendEstudios(estudios);
    loadFileWindow.exec();
}

void MainWindow::receiveEstudios(QVector<QStringList> &dataEstudios)
{
    estudios = dataEstudios.at(0);
    QStringList dataFiles = dataEstudios.at(1);
    if (dataFiles.length() != 0) {
        QVector<QVector<double>> dataSenales = Senales(dataFiles);
        estudiosSenales.push_back(dataSenales);

        // Gráfica Oscilación Lateral ---------------------------------------------------
        ui->plotLateral->addGraph();
        ui->plotLateral->graph(graphNumber)->setData(dataSenales[2], dataSenales[0]);
        ui->plotLateral->graph(graphNumber)->setPen(QPen(graphColors.at(graphNumber % 7)));
        ui->plotLateral->graph(graphNumber)->setName(estudios.last());
        ui->plotLateral->legend->setVisible(true);
        ui->plotLateral->xAxis->rescale(true);
        ui->plotLateral->yAxis->rescale(true);
        ui->plotLateral->replot();

        // Gráfica Oscilación Antero-posterior ------------------------------------------
        ui->plotAnteroPosterior->addGraph();
        ui->plotAnteroPosterior->graph(graphNumber)->setData(dataSenales[2],dataSenales[1]);
        ui->plotAnteroPosterior->graph(graphNumber)->setPen(QPen(graphColors.at(graphNumber % 7)));
        ui->plotAnteroPosterior->graph(graphNumber)->setName(estudios.last());
        ui->plotAnteroPosterior->legend->setVisible(true);
        ui->plotAnteroPosterior->xAxis->rescale(true);
        ui->plotAnteroPosterior->yAxis->rescale(true);
        ui->plotAnteroPosterior->replot();

        graphNumber++;

        // Parámetros -------------------------------------------------------------------
        ui->cmbxParametros->addItem(estudios.last());
        ui->cmbxParametros->setEnabled(true);

        ui->btnGenerarReporte->setEnabled(true);
        ui->statusBar->showMessage(tr("Señal cargada"));
    }
}

void MainWindow::on_btnAnalisis_clicked()
{
    LoadWindow loadFileWindow;
    connect(&loadFileWindow , SIGNAL(sendLoadEstudios(QVector<QStringList>&)), this, SLOT(receiveAnalisis(QVector<QStringList>&)));
    loadFileWindow.exec();
}

void MainWindow::receiveAnalisis(QVector<QStringList> &dataEstudios)
{
    QString titulo = dataEstudios[0][0];
    QStringList dataFiles = dataEstudios[1];
    if (dataFiles.length() != 0) {

        for (int j = 0; j < dataFiles.length(); j++) {
            estudios.push_back(QString("%1 %2").arg(titulo).arg(j+1));
            QStringList File({ dataFiles[j] });

            QVector<QVector<double>> dataSenales = Senales(File);
            estudiosSenales.push_back(dataSenales);

            // Gráfica Oscilación Lateral ---------------------------------------------------
            ui->plotLateral->addGraph();
            ui->plotLateral->graph(graphNumber)->setData(dataSenales[2], dataSenales[0]);
            ui->plotLateral->graph(graphNumber)->setPen(QPen(graphColors.at(graphNumber % 7)));
            ui->plotLateral->graph(graphNumber)->setName(estudios.last());
            ui->plotLateral->legend->setVisible(true);
            ui->plotLateral->xAxis->rescale(true);
            ui->plotLateral->yAxis->rescale(true);
            ui->plotLateral->replot();

            // Gráfica Oscilación Antero-posterior ------------------------------------------
            ui->plotAnteroPosterior->addGraph();
            ui->plotAnteroPosterior->graph(graphNumber)->setData(dataSenales[2],dataSenales[1]);
            ui->plotAnteroPosterior->graph(graphNumber)->setPen(QPen(graphColors.at(graphNumber % 7)));
            ui->plotAnteroPosterior->graph(graphNumber)->setName(estudios.last());
            ui->plotAnteroPosterior->legend->setVisible(true);
            ui->plotAnteroPosterior->xAxis->rescale(true);
            ui->plotAnteroPosterior->yAxis->rescale(true);
            ui->plotAnteroPosterior->replot();

            graphNumber++;

            // Parámetros -------------------------------------------------------------------
            ui->cmbxParametros->addItem(estudios.last());
            ui->cmbxParametros->setEnabled(true);
        }
        ui->btnGenerarReporte->setEnabled(true);
        ui->statusBar->showMessage(tr("Análisis múltiple terminado"));
    }
}

void MainWindow::on_btnGenerarReporte_clicked()
{
    QString title, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12;

    QString textFileName = QFileDialog::getSaveFileName(this, tr("Guardar el reporte del estudio en..."), resultsFolder, tr("Documento PDF (*.pdf)"));

    if (textFileName != "") {
        QPdfWriter printer(textFileName);
        printer.setPageSize(QPagedPaintDevice::Letter);
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setPageMargins(QMarginsF(15,15,15,15), QPageLayout::Millimeter);
        printer.setResolution(1200); // High resolution
        QPainter generate(&printer);
        generate.end();

        QFileInfo pdfFile(textFileName);
        QString selectedFolder = pdfFile.canonicalPath();

        QDir dir(QString("%1/Imagenes").arg(selectedFolder));
        dir.mkpath(QString("%1/Imagenes").arg(selectedFolder));
        generarImagenes(selectedFolder);

        QPen titlePen(QColor( 23, 54, 93 ));
        QFont titleFont = QFont("Cambria", 26);
        QPen paragraphPen(QColor( 0, 0, 0 ));
        QFont paragraphFont = QFont("Calibri", 11);
        QPen subtitle1Pen(QColor( 255, 140, 0 ));
        QFont subtitle1Font = QFont("Cambria", 13, QFont::Bold);
        QPen subtitle2Pen(QColor( 23, 54, 93 ));
        QFont subtitle2Font = QFont("Cambria", 11, QFont::Bold, true);

        QPainter painter(&printer);
        painter.setFont(titleFont);
        painter.setPen(titlePen);
        painter.drawText( QRect(0, 700, 8780, 500), Qt::AlignHCenter, reportTexts[0]);

        // INTRODUCCIÓN------------------------------------------------------------------
        painter.setFont(paragraphFont);
        painter.setPen(paragraphPen);
        painter.drawText( QRect(0, 1600, 8780, 900), Qt::TextWordWrap | Qt::AlignJustify, reportTexts[1] );

//        tr("A continuación, se presentan los resultados del Test de Romberg. Cada prueba consistió en 3 mediciones de la oscilación del centro de presión en un eje determinado (X: Lateral, Y: Antero-posterior) durante un tiempo determinado, y se promediaron. Se realizaron 4 pruebas, donde se alteraron los sentidos visual y vestibular, encargados de mantener el equilibrio en un sujeto.")

        // DATOS DEL SUJETO -------------------------------------------------------------
        painter.setFont(subtitle1Font);
        painter.setPen(subtitle1Pen);
        painter.drawText( QRect(0, 2700, 8780, 300), reportTexts[2] ); // tr("DATOS DEL PACIENTE")

        painter.setFont(paragraphFont);
        painter.setPen(paragraphPen);
        painter.drawText( QRect(0, 3100, 3000, 300), QString(tr("APELLIDOS:")) );
        painter.drawText( QRect(0, 3400, 3000, 300), QString(tr("NOMBRES:")) );
        painter.drawText( QRect(0, 3700, 3000, 300), QString(tr("IDENTIFICACIÓN:")) );
        painter.drawText( QRect(0, 4000, 3000, 300), QString(tr("FECHA DE NACIMIENTO:")) );
        painter.drawText( QRect(0, 4300, 3000, 300), QString(tr("GÉNERO:")) );
        painter.drawText( QRect(0, 4600, 3000, 300), QString(tr("PESO:")) );
        painter.drawText( QRect(0, 4900, 3000, 300), QString(tr("ALTURA:")) );
        painter.drawText( QRect(0, 5200, 3000, 300), QString(tr("ÍNDICE DE MASA CORPORAL:")) );
        painter.drawText( QRect(0, 5500, 3000, 300), QString(tr("FECHA DE SESIÓN:")) );
        painter.drawText( QRect(0, 5800, 3000, 300), QString(tr("OCUPACIÓN:")) );
        painter.drawText( QRect(0, 6100, 3000, 300), QString(tr("PROYECTO:")) );
        painter.drawText( QRect(0, 6400, 3000, 300), QString(tr("EVALUADO POR:")) );

        painter.drawText( QRect(3000, 3100, 4000, 300), QString("%1").arg(ui->editApellidos->text()) );
        painter.drawText( QRect(3000, 3400, 4000, 300), QString("%1").arg(ui->editNombres->text()) );
        painter.drawText( QRect(3000, 3700, 4000, 300), QString("%1 %2").arg(ui->cmbxId->currentText()).arg(ui->editId->text()) );
        painter.drawText( QRect(3000, 4000, 4000, 300), QString("%1").arg(ui->dateNacimiento->text()) );
        painter.drawText( QRect(3000, 4300, 4000, 300), QString("%1").arg(ui->cmbxGenero->currentText()) );
        if (ui->cmbxPeso->currentIndex() == 0) {
            painter.drawText( QRect(3000, 4600, 4000, 300), QString("%1 Kg").arg(ui->spinPeso->value()) );
        } else {
            painter.drawText( QRect(3000, 4600, 4000, 300), QString("%1 Lb").arg(ui->spinPeso->value()) );
        }
        if (ui->cmbxAltura->currentIndex() == 0) {
            painter.drawText( QRect(3000, 4900, 4000, 300), QString("%1 m").arg(ui->spinAltura->value()) );
        } else {
            painter.drawText( QRect(3000, 4900, 4000, 300), QString("%1 ft.in").arg(ui->spinAltura->value()) );
        }
        painter.drawText( QRect(3000, 5200, 4000, 300), QString("%1").arg(ui->lblBMI->text()) );
        painter.drawText( QRect(3000, 5500, 4000, 300), QString("%1").arg(ui->dateSesion->text()) );
        painter.drawText( QRect(3000, 5800, 4000, 300), QString("%1").arg(ui->editOcupacion->text()) );
        painter.drawText( QRect(3000, 6100, 4000, 300), QString("%1").arg(ui->editProyecto->text()) );
        painter.drawText( QRect(3000, 6400, 4000, 300), QString("%1").arg(ui->editEvaluador->text()) );

        // SEÑALES DE OSCILACIONES ------------------------------------------------------
        painter.setFont(subtitle1Font);
        painter.setPen(subtitle1Pen);
        painter.drawText( QRect(0, 7000, 8780, 300), reportTexts[3] ); // tr("SEÑALES DE OSCILACIONES")

        painter.setFont(paragraphFont);
        painter.setPen(paragraphPen);
        painter.drawText( QRect(0, 7500, 8780, 300), Qt::TextWordWrap | Qt::AlignJustify, reportTexts[4] );
//        tr("Inicialmente, se presentan las señales de la oscilación Lateral y oscilación antero-posterior, respecto al tiempo.")

        QImage senalLateral(QString("%1/Imagenes/SenalLateral.png").arg(selectedFolder));
        QRect lateralRect;
        lateralRect.setX(0);
        lateralRect.setY(7800);
        lateralRect.setWidth(8780);
        lateralRect.setHeight(8780 * senalLateral.height() / senalLateral.width() );
        painter.drawImage( lateralRect, senalLateral );

        QImage senalAnteroPosterior(QString("%1/Imagenes/senalAnteroPosterior.png").arg(selectedFolder));
        QRect AnteroPosteriorRect;
        AnteroPosteriorRect.setX(0);
        AnteroPosteriorRect.setY(8000 + lateralRect.height());
        AnteroPosteriorRect.setWidth(8780);
        AnteroPosteriorRect.setHeight( 8780 * senalAnteroPosterior.height() / senalAnteroPosterior.width() );
        painter.drawImage( AnteroPosteriorRect, senalAnteroPosterior );

        printer.newPage();

        // ÁREAS DE OSCILACIONES ------------------------------------------------------
        int startObjX = 0, startObjY = 700;

        painter.setFont(subtitle1Font);
        painter.setPen(subtitle1Pen);
        painter.drawText( QRect(startObjX, startObjY, 8780, 300), reportTexts[5] ); // tr("ÁREAS DE OSCILACIONES")

        startObjY = 1200;
        painter.setFont(subtitle2Font);
        painter.setPen(subtitle2Pen);
        painter.drawText( QRect(startObjX, startObjY, 8780, 300), reportTexts[6] ); // tr("ELIPSE")

        QStringList fileSuffix;
        QStringList imageFileList = dir.entryList( QDir::Files , QDir::Name );
        for (int i = 0; i < ((imageFileList.length() - 2) / 3); i ++) {
            QString s = imageFileList.at(i);
            s.remove("Convex");
            s.remove(".png");
            fileSuffix.push_back(s);
        }

        startObjY = 1700;
        for (int i = 1; i <= fileSuffix.length(); i++) {
            if (startObjY > 8000) {
                printer.newPage();
                startObjY = 700;
            }
            QImage elipseImage(QString("%1/Imagenes/Elipse%2.png").arg(selectedFolder).arg(fileSuffix.at(i-1)));
            QRect elipseRect;
            elipseRect.setX(startObjX);
            elipseRect.setY(startObjY);
            elipseRect.setHeight(8780 / 2);
            elipseRect.setWidth(8780 / 2);
            painter.drawImage( elipseRect, elipseImage );
            if (i % 2 == 1) {
                startObjX = 8780 / 2;
            } else {
                startObjX = 0;
                startObjY = startObjY + 8780 / 2;
            }
        }

        if (fileSuffix.length() % 2 == 1) {
            startObjY = startObjY + (8780 / 2) + 200;
        }
        if (startObjY > 8000) {
            printer.newPage();
            startObjY = 700;
        }
        startObjX = 0;
        painter.drawText( QRect(startObjX, startObjY, 8780, 300), reportTexts[7] ); // tr("POLÍGONO ENVOLVENTE")

        startObjY = startObjY + 300;
        for (int i = 1; i <= fileSuffix.length(); i++) {
            if (startObjY > 8000) {
                printer.newPage();
                startObjY = 700;
            }
            QImage convexImage(QString("%1/Imagenes/Convex%2.png").arg(selectedFolder).arg(fileSuffix.at(i-1)));
            QRect convexRect;
            convexRect.setX(startObjX);
            convexRect.setY(startObjY);
            convexRect.setHeight(8780 / 2);
            convexRect.setWidth(8780 / 2);
            painter.drawImage( convexRect, convexImage );
            if (i % 2 == 1) {
                startObjX = 8780 / 2;
            } else {
                startObjX = 0;
                startObjY = startObjY + 8780 / 2;
            }
        }

        if (fileSuffix.length() % 2 == 1) {
            startObjY = startObjY + (8780 / 2) + 200;
        }
        if (startObjY > 8000) {
            printer.newPage();
            startObjY = 700;
        }
        startObjX = 0;
        painter.drawText( QRect(startObjX, startObjY, 8780, 300), reportTexts[8] ); // tr("ELIPSE ORIENTADA")

        startObjY = startObjY + 300;
        for (int i = 1; i <= fileSuffix.length(); i++) {
            if (startObjY > 8000) {
                printer.newPage();
                startObjY = 700;
            }
            QImage pcaImage(QString("%1/Imagenes/ElipsePca%2.png").arg(selectedFolder).arg(fileSuffix.at(i-1)));
            QRect pcaRect;
            pcaRect.setX(startObjX);
            pcaRect.setY(startObjY);
            pcaRect.setHeight(8780 / 2);
            pcaRect.setWidth(8780 / 2);
            painter.drawImage( pcaRect, pcaImage );
            if (i % 2 == 1) {
                startObjX = 8780 / 2;
            } else {
                startObjX = 0;
                startObjY = startObjY + 8780 / 2;
            }
        }

        if (fileSuffix.length() % 2 == 1) {
            startObjY = startObjY + (8780 / 2) + 200;
        }
        if (startObjY > 8000) {
            printer.newPage();
            startObjY = 700;
        }
        startObjX = 0;

        // PARÁMETROS -------------------------------------------------------------------
        painter.setFont(subtitle1Font);
        painter.setPen(subtitle1Pen);
        painter.drawText( QRect(0, startObjY, 8780, 300), reportTexts[9] ); // tr("PARÁMETROS")
        startObjY = startObjY + 1000;

        QString title = QString(tr("RESULTADOS ESTUDIO"));
        QString var1 = QString(tr("Rango de Oscilación Lateral (mm)"));
        QString var2 = QString(tr("Rango de Oscilación Antero-Posterior (mm)"));
        QString var3 = QString(tr("Velocidad Media Oscilación Lateral (mm/s)"));
        QString var4 = QString(tr("Velocidad Media Oscilación Antero-Posterior (mm/s)"));
        QString var5 = QString(tr("Velocidad Media Oscilación del Centro de Presión (mm/s)"));
        QString var6 = QString(tr("Oscilación Lateral RMS (mm)"));
        QString var7 = QString(tr("Oscilación Antero-Posterior RMS (mm)"));
        QString var8 = QString(tr("Distancia Media (mm)"));
        QString var9 = QString(tr("Frecuencia Media (Hz)"));
        QString var10 = QString(tr("Área de la Elipse (mm²)"));
        QString var11 = QString(tr("Área del Polígono Envolvente (mm²)"));
        QString var12 = QString(tr("Área de la Elipse Orientada (mm²)"));

        painter.setFont(paragraphFont);
        painter.setPen(paragraphPen);
        painter.drawText( QRect(0, startObjY        , 4500, 300 ), var1 );
        painter.drawText( QRect(0, startObjY + 300  , 4500, 300 ), var2 );
        painter.drawText( QRect(0, startObjY + 600  , 4500, 300 ), var3 );
        painter.drawText( QRect(0, startObjY + 900  , 4500, 300 ), var4 );
        painter.drawText( QRect(0, startObjY + 1200 , 4500, 300 ), var5 );
        painter.drawText( QRect(0, startObjY + 1500 , 4500, 300 ), var6 );
        painter.drawText( QRect(0, startObjY + 1800 , 4500, 300 ), var7 );
        painter.drawText( QRect(0, startObjY + 2100 , 4500, 300 ), var8 );
        painter.drawText( QRect(0, startObjY + 2400 , 4500, 300 ), var9 );
        painter.drawText( QRect(0, startObjY + 2700 , 4500, 300 ), var10 );
        painter.drawText( QRect(0, startObjY + 3000 , 4500, 300 ), var11 );
        painter.drawText( QRect(0, startObjY + 3300 , 4500, 300 ), var12 );

        for (int i = 1; i <= fileSuffix.length(); i++) {
            ui->cmbxParametros->setCurrentIndex(i);
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY - 500  , 1000, 300 ), QString("%1").arg(fileSuffix.at(i - 1)) );
            title = title + QString("\t%1").arg(fileSuffix.at(i - 1));
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY        , 1000, 300 ), QString("%1").arg(ui->lblValueParam1->text()) );
            var1 = var1 + QString("\t%1").arg(ui->lblValueParam1->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 300  , 1000, 300 ), QString("%1").arg(ui->lblValueParam2->text()) );
            var2 = var2 + QString("\t%1").arg(ui->lblValueParam2->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 600  , 1000, 300 ), QString("%1").arg(ui->lblValueParam3->text()) );
            var3 = var3 + QString("\t%1").arg(ui->lblValueParam3->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 900  , 1000, 300 ), QString("%1").arg(ui->lblValueParam4->text()) );
            var4 = var4 + QString("\t%1").arg(ui->lblValueParam4->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 1200 , 1000, 300 ), QString("%1").arg(ui->lblValueParam5->text()) );
            var5 = var5 + QString("\t%1").arg(ui->lblValueParam5->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 1500 , 1000, 300 ), QString("%1").arg(ui->lblValueParam6->text()) );
            var6 = var6 + QString("\t%1").arg(ui->lblValueParam6->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 1800 , 1000, 300 ), QString("%1").arg(ui->lblValueParam7->text()) );
            var7 = var7 + QString("\t%1").arg(ui->lblValueParam7->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 2100 , 1000, 300 ), QString("%1").arg(ui->lblValueParam8->text()) );
            var8 = var8 + QString("\t%1").arg(ui->lblValueParam8->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 2400 , 1000, 300 ), QString("%1").arg(ui->lblValueParam9->text()) );
            var9 = var9 + QString("\t%1").arg(ui->lblValueParam9->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 2700 , 1000, 300 ), QString("%1").arg(ui->lblValueParam10->text()) );
            var10 = var10 + QString("\t%1").arg(ui->lblValueParam10->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 3000 , 1000, 300 ), QString("%1").arg(ui->lblValueParam11->text()) );
            var11 = var11 + QString("\t%1").arg(ui->lblValueParam11->text());
            painter.drawText( QRect(4500 + ((i - 1) * 1000), startObjY + 3300 , 1000, 300 ), QString("%1").arg(ui->lblValueParam12->text()) );
            var12 = var12 + QString("\t%1").arg(ui->lblValueParam12->text());
        }
        title = title + QString("\r\n");
        var1 = var1 + QString("\r\n");
        var2 = var2 + QString("\r\n");
        var3 = var3 + QString("\r\n");
        var4 = var4 + QString("\r\n");
        var5 = var5 + QString("\r\n");
        var6 = var6 + QString("\r\n");
        var7 = var7 + QString("\r\n");
        var8 = var8 + QString("\r\n");
        var9 = var9 + QString("\r\n");
        var10 = var10 + QString("\r\n");
        var11 = var11 + QString("\r\n");

        painter.end();

        dir.removeRecursively();

        QFile file(QString("%1/%2.txt").arg(pdfFile.canonicalPath()).arg(pdfFile.baseName()));
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << title << var1 << var2 << var3 << var4 << var5 << var6 << var7 << var8 << var9 << var10 << var11 << var12;

        ui->statusBar->showMessage(tr("Reporte generado"));
    } else {
        ui->statusBar->showMessage(tr("No se generó el reporte"));
    }
}

void MainWindow::on_btnEditarReporte_clicked()
{
    ReportEditWindow editarWindow;
    editarWindow.exec();

    //---------------------------- Valores de Configuración -----------------------------
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);
    reportTexts = testRombergSettings.value("Report_Texts").toStringList();
}

// --------------------------------------------------------------------------------------
// AYUDA
// --------------------------------------------------------------------------------------

void MainWindow::on_manualButton_clicked()
{
    helpWindow helpVideoApp;
    helpVideoApp.exec();
}

void MainWindow::on_aboutButton_clicked()
{
    //------------------------------ Definición del idioma ------------------------------
    QString message = tr("TEST DE ROMBERG\n"
                         "\n"
                         "ING. CARLOS ANDRÉS WILCHES PÉREZ, MSc.\n"
                         "e-mail: cawilchesp@outlook.com\n"
                         "\n"
                         "ING. ÓSCAR JULIÁN PERDOMO CHARRY, MSc.\n"
                         "e-mail: oscarjulian.perdomo@gmail.com\n"
                         "\n"
                         "UNIVERSIDAD MANUELA BELTRÁN\n"
                         "FACULTAD DE INGENIERÍA BIOMÉDICA\n"
                         "BOGOTÁ, COLOMBIA\n"
                         "2015");

    //------------------------------- Mensaje Acerca de... ------------------------------
    QMessageBox about;
    about.setWindowTitle(tr("Acerca de..."));
//    about.setWindowIcon(QIcon(QPixmap(":/imagenes/Imagenes/Icono.png")));
    about.setText(message);
    about.setIconPixmap(QPixmap(":/imagenes/Imagenes/Escudo_UMB.png").scaledToWidth(300,Qt::SmoothTransformation));
    about.setStandardButtons(QMessageBox::Ok);
    about.setDefaultButton(QMessageBox::Ok);
    about.setStyleSheet("QLabel {min-width: 300px; max-width: 300px; min-height: 300px; max-height: 300px; }");
    about.exec();
}

void MainWindow::on_aboutQtButton_clicked()
{
    QMessageBox::aboutQt(this, tr("Acerca de Qt"));
}

// ----------------------------------------------------------------------------------------
// DATOS
// ----------------------------------------------------------------------------------------

void MainWindow::on_btnNuevo_clicked()
{
    // Reajustar campos de datos --------------------------------------------------------
    ui->editApellidos->clear();
    ui->editApellidos->setEnabled(true);
    ui->editNombres->clear();
    ui->editNombres->setEnabled(true);
    ui->cmbxId->setCurrentIndex(-1);
    ui->cmbxId->setEnabled(true);
    ui->editId->clear();
    ui->editId->setEnabled(true);
    ui->dateNacimiento->setDate(QDate(2000,1,1));
    ui->dateNacimiento->setEnabled(true);
    ui->cmbxGenero->setCurrentIndex(-1);
    ui->cmbxGenero->setEnabled(true);
    ui->spinPeso->setValue(0);
    ui->spinPeso->setEnabled(true);
    ui->cmbxPeso->setCurrentIndex(-1);
    ui->cmbxPeso->setEnabled(true);
    ui->spinAltura->setValue(0);
    ui->spinAltura->setEnabled(true);
    ui->cmbxAltura->setCurrentIndex(-1);
    ui->cmbxAltura->setEnabled(true);
    ui->lblBMI->clear();
    ui->dateSesion->setDate(QDate(2000,1,1));
    ui->dateSesion->setEnabled(true);
    ui->editOcupacion->clear();
    ui->editOcupacion->setEnabled(true);
    ui->editProyecto->clear();
    ui->editProyecto->setEnabled(true);
    ui->editEvaluador->clear();
    ui->editEvaluador->setEnabled(true);

    // Reajustar gráficas ---------------------------------------------------------------
    ui->plotLateral->clearGraphs();
    ui->plotLateral->replot();
    ui->plotAnteroPosterior->clearGraphs();
    ui->plotAnteroPosterior->replot();
    ui->plotElipse->clearPlottables();
    ui->plotElipse->replot();
    ui->plotConvex->clearPlottables();
    ui->plotConvex->replot();
    ui->plotPca->clearPlottables();
    ui->plotPca->replot();

    ui->cmbxParametros->clear();
    ui->cmbxParametros->addItem("");
    estudios.clear();
    estudiosSenales.clear();
    graphNumber = 0;

    // Reajustar botones ----------------------------------------------------------------
    ui->btnGuardar->setEnabled(true);
    ui->btnGenerarReporte->setEnabled(false);

    // Establecer campo inicial ---------------------------------------------------------
    ui->editApellidos->setFocus();
}

void MainWindow::on_btnCargar_clicked()
{
    QString textFileName = QFileDialog::getOpenFileName(this, tr("Seleccione el archivo con los datos del sujeto"), resultsFolder, tr("Archivos de Texto (*.txt)"));
    if (textFileName != "")
    {
        // Cargar datos -----------------------------------------------------------------
        QFile file(textFileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QString lines = in.readAll();
        QStringList splitLines = lines.split(QRegExp("\t|\n"));

        ui->editApellidos->setText(splitLines.at(3));
        ui->editNombres->setText(splitLines.at(5));

        QStringList idText = QString(splitLines.at(7)).split(QRegExp(" "));
        ui->editId->setText(idText.at(1));
        ui->cmbxId->setCurrentText(idText.at(0));

        ui->dateNacimiento->setDate(QDate::fromString(splitLines.at(9), "dd/MM/yyyy"));

        ui->cmbxGenero->setCurrentText(splitLines.at(11));

        QStringList splitPeso = QString(splitLines.at(13)).split(QRegExp(" "));
        ui->spinPeso->setValue(QString(splitPeso.at(0)).toDouble());
        if (splitPeso.at(1) == "Kg") {
            ui->cmbxPeso->setCurrentIndex(0);
        } else {
            ui->cmbxPeso->setCurrentIndex(1);
        }

        QStringList splitAltura = QString(splitLines.at(15)).split(QRegExp(" "));
        ui->spinAltura->setValue(QString(splitAltura.at(0)).toDouble());
        if (splitAltura.at(1) == "m") {
            ui->cmbxAltura->setCurrentIndex(0);
        } else {
            ui->cmbxAltura->setCurrentIndex(1);
        }

        ui->lblBMI->setText(splitLines.at(17));

        ui->dateSesion->setDate(QDate::fromString(splitLines.at(19), "dd/MM/yyyy"));

        ui->editOcupacion->setText(splitLines.at(21));
        ui->editProyecto->setText(splitLines.at(23));
        ui->editEvaluador->setText(splitLines.at(25));

        // Habilitar cuadros ------------------------------------------------------------
        ui->editApellidos->setEnabled(true);
        ui->editNombres->setEnabled(true);
        ui->editId->setEnabled(true);
        ui->cmbxId->setEnabled(true);
        ui->dateNacimiento->setEnabled(true);
        ui->cmbxGenero->setEnabled(true);
        ui->spinPeso->setEnabled(true);
        ui->cmbxPeso->setEnabled(true);
        ui->spinAltura->setEnabled(true);
        ui->cmbxAltura->setEnabled(true);
        ui->dateSesion->setEnabled(true);
        ui->editOcupacion->setEnabled(true);
        ui->editProyecto->setEnabled(true);
        ui->editEvaluador->setEnabled(true);

        // Habilitar botón --------------------------------------------------------------
        ui->btnGuardar->setEnabled(true);

        ui->statusBar->showMessage(tr("Datos del sujeto cargados"));
    }
}

void MainWindow::on_btnGuardar_clicked()
{
    if (ui->editApellidos->text() == "" || ui->editNombres->text() == "" || ui->cmbxId->currentIndex() == -1 || ui->editId->text() == "" ||
            ui->cmbxGenero->currentIndex() == -1 || ui->cmbxPeso->currentIndex() == -1 || ui->cmbxAltura->currentIndex() == -1 ||
            ui->editOcupacion->text() == "" || ui->editProyecto->text() == "" || ui->editEvaluador->text() == "") {
        QMessageBox *MsgBox = new QMessageBox(QMessageBox::Critical, "Error", tr("Faltan campos de datos por completar"), QMessageBox::Close);
        MsgBox->exec();
    } else {
        QString pesoUnidades;
        if (ui->cmbxPeso->currentIndex() == 0) {
            pesoUnidades = "Kg";
        } else {
            pesoUnidades = "Lb";
        }

        QString alturaUnidades;
        if (ui->cmbxAltura->currentIndex() == 0) {
            alturaUnidades = "m";
        } else {
            alturaUnidades = "ft.in";
        }

        QString textFileName = QFileDialog::getSaveFileName(this, tr("Guardar los datos del sujeto en..."), resultsFolder, tr("Archivos de Texto (*.txt)"));
        if (textFileName != "") {
            QFile file(textFileName);
            file.open(QIODevice::WriteOnly);
            QTextStream out(&file);
            out << QString("DATOS\tSUJETO\r\n"
                           "APELLIDOS:\t%1\r\n"
                           "NOMBRES:\t%2\r\n"
                           "IDENTIFICACIÓN:\t%3 %4\r\n"
                           "FECHA DE NACIMIENTO:\t%5\r\n"
                           "GÉNERO:\t%6\r\n"
                           "PESO:\t%7 %8\r\n"
                           "ALTURA:\t%9 %10\r\n"
                           "IMC:\t%11\r\n"
                           "FECHA DE SESIÓN:\t%12\r\n"
                           "OCUPACIÓN:\t%13\r\n"
                           "PROYECTO:\t%14\r\n"
                           "EVALUADO POR:\t%15")
                   .arg(ui->editApellidos->text())
                   .arg(ui->editNombres->text())
                   .arg(ui->cmbxId->currentText()).arg(ui->editId->text())
                   .arg(ui->dateNacimiento->text())
                   .arg(ui->cmbxGenero->currentText())
                   .arg(ui->spinPeso->value()).arg(pesoUnidades)
                   .arg(ui->spinAltura->value()).arg(alturaUnidades)
                   .arg(ui->lblBMI->text())
                   .arg(ui->dateSesion->text())
                   .arg(ui->editOcupacion->text())
                   .arg(ui->editProyecto->text())
                   .arg(ui->editEvaluador->text());

            ui->statusBar->showMessage(tr("Datos del sujeto guardados"));
        }
    }
}

void MainWindow::on_editApellidos_textChanged(const QString &arg1)
{
    ui->editApellidos->setText(arg1.toUpper());
}

void MainWindow::on_editNombres_textChanged(const QString &arg1)
{
    ui->editNombres->setText(arg1.toUpper());
}

void MainWindow::on_spinPeso_valueChanged(double arg1)
{
    if ((ui->cmbxPeso->currentIndex() != -1) && (ui->cmbxAltura->currentIndex() != -1)) {
        double pesoKg = 0;
        if (ui->cmbxPeso->currentIndex() == 0) {
            pesoKg = arg1;
        } else {
            pesoKg = arg1 * 0.454;
        }

        double alturaM = 0;
        if (ui->cmbxAltura->currentIndex() == 0) {
            alturaM = ui->spinAltura->value();
        } else {
            double alturaFt = qFloor(ui->spinAltura->value());
            double alturaIn = (ui->spinAltura->value() - alturaFt) * 100;
            alturaM = ((alturaFt * 12) + alturaIn) * 2.54 / 100;
        }
        ui->lblBMI->setText(QString::number(pesoKg / (alturaM * alturaM),'F',1));
    }
}

void MainWindow::on_spinAltura_valueChanged(double arg1)
{
    if ((ui->cmbxPeso->currentIndex() != -1) && (ui->cmbxAltura->currentIndex() != -1)) {
        double pesoKg = 0;
        if (ui->cmbxPeso->currentIndex() == 0) {
            pesoKg = ui->spinPeso->value();
        } else {
            pesoKg = ui->spinPeso->value() * 0.454;
        }

        double alturaM = 0;
        if (ui->cmbxAltura->currentIndex() == 0) {
            alturaM = arg1;
        } else {
            double alturaFt = qFloor(arg1);
            double alturaIn = (arg1 - alturaFt) * 100;
            alturaM = ((alturaFt * 12) + alturaIn) * 2.54 / 100;
        }
        ui->lblBMI->setText(QString::number(pesoKg / (alturaM * alturaM),'F',1));
    }
}

void MainWindow::on_cmbxPeso_currentIndexChanged(int index)
{
    if ((ui->cmbxPeso->currentIndex() != -1) && (ui->cmbxAltura->currentIndex() != -1)) {
        double pesoKg = 0;
        if (index == 0) {
            pesoKg = ui->spinPeso->value();
        } else {
            pesoKg = ui->spinPeso->value() * 0.454;
        }

        double alturaM = 0;
        if (ui->cmbxAltura->currentIndex() == 0) {
            alturaM = ui->spinAltura->value();
        } else {
            double alturaFt = qFloor(ui->spinAltura->value());
            double alturaIn = (ui->spinAltura->value() - alturaFt) * 100;
            alturaM = ((alturaFt * 12) + alturaIn) * 2.54 / 100;
        }
        ui->lblBMI->setText(QString::number(pesoKg / (alturaM * alturaM),'F',1));
    }
}

void MainWindow::on_cmbxAltura_currentIndexChanged(int index)
{
    if ((ui->cmbxPeso->currentIndex() != -1) && (ui->cmbxAltura->currentIndex() != -1)) {
        double pesoKg = 0;
        if (ui->cmbxPeso->currentIndex() == 0) {
            pesoKg = ui->spinPeso->value();
        } else {
            pesoKg = ui->spinPeso->value() * 0.454;
        }

        double alturaM = 0;
        if (index == 0) {
            alturaM = ui->spinAltura->value();
        } else {
            double alturaFt = qFloor(ui->spinAltura->value());
            double alturaIn = (ui->spinAltura->value() - alturaFt) * 100;
            alturaM = ((alturaFt * 12) + alturaIn) * 2.54 / 100;
        }
        ui->lblBMI->setText(QString::number(pesoKg / (alturaM * alturaM),'F',1));
    }
}

void MainWindow::on_editOcupacion_textChanged(const QString &arg1)
{
    ui->editOcupacion->setText(arg1.toUpper());
}

void MainWindow::on_editProyecto_textChanged(const QString &arg1)
{
    ui->editProyecto->setText(arg1.toUpper());
}

void MainWindow::on_editEvaluador_textChanged(const QString &arg1)
{
    ui->editEvaluador->setText(arg1.toUpper());
}

// ----------------------------------------------------------------------------------------
// CARGAR ÁREAS
// ----------------------------------------------------------------------------------------

void MainWindow::on_cmbxParametros_currentIndexChanged(int index)
{
    if (index > 0) {
        QVector<QVector<double>> dataSenales = estudiosSenales.at(index - 1);

        // Gráfica Oscilación - Elipse --------------------------------------------------
        QVector<QVector<double>> elipse = ellipseStandard(dataSenales);

        ui->plotElipse->clearPlottables();

        ui->plotElipse->addGraph();
        ui->plotElipse->graph(0)->setData(dataSenales[0],dataSenales[1]);
        ui->plotElipse->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->plotElipse->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
        ui->plotElipse->graph(0)->setPen(QPen(QColor(0, 114, 189), 2));

        QCPCurve *elipseCurve = new QCPCurve(ui->plotElipse->xAxis, ui->plotElipse->yAxis);
        ui->plotElipse->addPlottable(elipseCurve);
        elipseCurve->setData(elipse[0], elipse[1]);
        elipseCurve->setPen(QPen(QColor(217, 83, 25), 2));

        auto x_max = std::max_element( dataSenales[0].begin() , dataSenales[0].end() );
        auto x_min = std::min_element( dataSenales[0].begin() , dataSenales[0].end() );
        auto y_max = std::max_element( dataSenales[1].begin() , dataSenales[1].end() );
        auto y_min = std::min_element( dataSenales[1].begin() , dataSenales[1].end() );
        double chartRange;
        if ( *x_max - *x_min > *y_max - *y_min ) {
            chartRange = 1.2 * (*x_max - *x_min);
        } else {
            chartRange = 1.2 * (*y_max - *y_min);
        }
        double positionX = *x_min - ((chartRange - *x_max + *x_min) / 2);
        ui->plotElipse->xAxis->setRange(positionX, chartRange, Qt::AlignLeft);
        double positionY = *y_min - ((chartRange - *y_max + *y_min) / 2);
        ui->plotElipse->yAxis->setRange(positionY, chartRange, Qt::AlignLeft);

        ui->plotElipse->replot();

        // Gráfica Oscilación - Envolvente Convexa --------------------------------------
        QVector<QVector<double>> envolvente = convexHull(dataSenales);

        ui->plotConvex->clearPlottables();

        ui->plotConvex->addGraph();
        ui->plotConvex->graph(0)->setData(dataSenales[0],dataSenales[1]);
        ui->plotConvex->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->plotConvex->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
        ui->plotConvex->graph(0)->setPen(QPen(QColor(0, 114, 189), 2));

        QCPCurve *convexHullCurve = new QCPCurve(ui->plotConvex->xAxis, ui->plotConvex->yAxis);
        ui->plotConvex->addPlottable(convexHullCurve);
        convexHullCurve->setData(envolvente[0], envolvente[1]);
        convexHullCurve->setPen(QPen(QColor(217, 83, 25), 2));

        ui->plotConvex->xAxis->setRange(positionX, chartRange, Qt::AlignLeft);
        ui->plotConvex->yAxis->setRange(positionY, chartRange, Qt::AlignLeft);

        ui->plotConvex->replot();

        // Gráfica Oscilación - Elipse Orientada ----------------------------------------
        QVector<QVector<double>> pca = ellipsePCA(dataSenales);

        ui->plotPca->clearPlottables();

        ui->plotPca->addGraph();
        ui->plotPca->graph(0)->setData(dataSenales[0],dataSenales[1]);
        ui->plotPca->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->plotPca->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
        ui->plotPca->graph(0)->setPen(QPen(QColor(0, 114, 189), 2));

        QCPCurve *pcaCurve = new QCPCurve(ui->plotPca->xAxis, ui->plotPca->yAxis);
        ui->plotPca->addPlottable(pcaCurve);
        pcaCurve->setData(pca[0], pca[1]);
        pcaCurve->setPen(QPen(QColor(217, 83, 25), 2));

        ui->plotPca->xAxis->setRange(positionX, chartRange, Qt::AlignLeft);
        ui->plotPca->yAxis->setRange(positionY, chartRange, Qt::AlignLeft);

        ui->plotPca->replot();

        QVector<double> resultados = Parametros(dataSenales);
        ui->lblValueParam1->setText(QString::number(resultados.at(0), 'F' , 2 ));
        ui->lblValueParam2->setText(QString::number(resultados.at(1), 'F' , 2 ));
        ui->lblValueParam3->setText(QString::number(resultados.at(2), 'F' , 2 ));
        ui->lblValueParam4->setText(QString::number(resultados.at(3), 'F' , 2 ));
        ui->lblValueParam5->setText(QString::number(resultados.at(4), 'F' , 2 ));
        ui->lblValueParam6->setText(QString::number(resultados.at(5), 'F' , 2 ));
        ui->lblValueParam7->setText(QString::number(resultados.at(6), 'F' , 2 ));
        ui->lblValueParam8->setText(QString::number(resultados.at(7), 'F' , 2 ));
        ui->lblValueParam9->setText(QString::number(resultados.at(8), 'F' , 2 ));
        ui->lblValueParam10->setText(QString::number(elipse[2][0], 'F' , 2 ));
        ui->lblValueParam11->setText(QString::number(envolvente[2][0], 'F' , 2 ));
        ui->lblValueParam12->setText(QString::number(pca[2][0], 'F' , 2 ));
    } else {
        ui->lblValueParam1->setText("");
        ui->lblValueParam2->setText("");
        ui->lblValueParam3->setText("");
        ui->lblValueParam4->setText("");
        ui->lblValueParam5->setText("");
        ui->lblValueParam6->setText("");
        ui->lblValueParam7->setText("");
        ui->lblValueParam8->setText("");
        ui->lblValueParam9->setText("");
        ui->lblValueParam10->setText("");
        ui->lblValueParam11->setText("");
        ui->lblValueParam12->setText("");

        // Reajustar gráficas ---------------------------------------------------------------
        ui->plotElipse->clearPlottables();
        ui->plotElipse->replot();
        ui->plotConvex->clearPlottables();
        ui->plotConvex->replot();
        ui->plotPca->clearPlottables();
        ui->plotPca->replot();
    }
}

// ----------------------------------------------------------------------------------------
// FUNCIONES
// ----------------------------------------------------------------------------------------

QVector<QVector<double>> MainWindow::ellipseStandard(QVector<QVector<double>> dataSenales)
{
    auto x_max = std::max_element(dataSenales[0].begin(), dataSenales[0].end());
    auto x_min = std::min_element(dataSenales[0].begin(), dataSenales[0].end());
    auto y_max = std::max_element(dataSenales[1].begin(), dataSenales[1].end());
    auto y_min = std::min_element(dataSenales[1].begin(), dataSenales[1].end());

    double a = (*x_max - *x_min) / 2;
    double b = (*y_max - *y_min) / 2;
    double x0 = *x_max - a;
    double y0 = *y_max - b;

    QVector<double> theta, x, y, area;
    for (int k = 0; k < 1000; k++) {
        theta.push_back(k * 2 * M_PI / 1000);
        x.push_back(x0 + a * qCos(theta[k]));
        y.push_back(y0 + b * qSin(theta[k]));
    }
    area.push_back(M_PI * a * b);

    QVector<QVector<double>> dataOut;
    dataOut.push_back(x);
    dataOut.push_back(y);
    dataOut.push_back(area);

    return dataOut;
}

QVector<QVector<double>> MainWindow::convexHull(QVector<QVector<double>> dataSenales)
{
    std::vector<cv::Point2f> puntosSenal, hull, contour;
    for (int i = 0 ; i < dataSenales[0].size() ; i++) {
        puntosSenal.push_back(cv::Point2f(static_cast<float>(dataSenales[0].at(i)) , static_cast<float>(dataSenales[1].at(i))));
    }
    cv::convexHull(puntosSenal, hull);

    QVector<double> hullX, hullY, area;
    for (size_t j = 0 ; j < hull.size() ; j++) {
        cv::Point2f pt = hull.at(j);
        hullX.push_back(static_cast<double>(pt.x));
        hullY.push_back(static_cast<double>(pt.y));
    }
    cv::Point2f pt = hull.at(0);
    hullX.push_back(static_cast<double>(pt.x));
    hullY.push_back(static_cast<double>(pt.y));

    cv::approxPolyDP(hull, contour, 0.001, false);
    area.push_back(cv::contourArea(hull));

    QVector<QVector<double>> dataOut;
    dataOut.push_back(hullX);
    dataOut.push_back(hullY);
    dataOut.push_back(area);

    return dataOut;
}

QVector<QVector<double>> MainWindow::ellipsePCA(QVector<QVector<double>> dataSenales)
{
    double sumX = std::accumulate(dataSenales[0].begin(), dataSenales[0].end(), 0.0);
    double sumY = std::accumulate(dataSenales[1].begin(), dataSenales[1].end(), 0.0);
    QVector<double> cen = { sumX / dataSenales[0].length() , sumY / dataSenales[1].length()};

    QVector<double> JX, JY, theta, rho, thetarot, rotX, rotY, covXX, covXY, covYY;
    for(int i = 0; i < dataSenales[0].length(); i++)
    {
        covXX.push_back( (dataSenales[0].at(i) - cen[0]) * (dataSenales[0].at(i) - cen[0]) );
        covXY.push_back( (dataSenales[0].at(i) - cen[0]) * (dataSenales[1].at(i) - cen[1]) );
        covYY.push_back( (dataSenales[1].at(i) - cen[1]) * (dataSenales[1].at(i) - cen[1]) );
        JX.push_back( dataSenales[0].at(i) - cen[0] );
        JY.push_back( dataSenales[1].at(i) - cen[1] );
        theta.push_back( qAtan2( JY.at(i) , JX.at(i) ) );
        rho.push_back( qSqrt(JX.at(i) * JX.at(i) + JY.at(i) * JY.at(i)) );
    }
    double a = std::accumulate(covXX.begin(), covXX.end(), 0.0) / covXX.length();
    double b = std::accumulate(covXY.begin(), covXY.end(), 0.0) / covXY.length();
    double d = std::accumulate(covYY.begin(), covYY.end(), 0.0) / covYY.length();

    double B = a + d;
    double C = a * d - b * b;
    double L1 = (B / 2) + qSqrt(B * B - 4 * C) / 2;
    QVector<double> eigvec = { L1 - d , b };

    double rot = qAtan( (L1 - d) / b );

    for(int i = 0; i < dataSenales[0].length(); i++)
    {
        thetarot.push_back(theta.at(i) + rot);
        rotX.push_back(rho.at(i) * qCos(thetarot.at(i)));
        rotY.push_back(rho.at(i) * qSin(thetarot.at(i)));
    }

    auto maxX = std::max_element(rotX.begin(), rotX.end());
    auto minX = std::min_element(rotX.begin(), rotX.end());
    auto maxY = std::max_element(rotY.begin(), rotY.end());
    auto minY = std::min_element(rotY.begin(), rotY.end());

    double aa = (*maxX - *minX) / 2;
    double bb = (*maxY - *minY) / 2;
    double x0 = *maxX - aa;
    double y0 = *maxY - bb;

    QVector<double> phi, newX, newY, thetaellipse, rhoellipse, thetarotellipse, Xellipse, Yellipse, XellipseFinal, YellipseFinal, area;
    for (int k = 0; k < 1000; k++)
    {
        phi.push_back( k * 2 * M_PI / 1000 );
        newX.push_back( x0 + aa * qCos(phi.at(k)) );
        newY.push_back( y0 + bb * qSin(phi.at(k)) );
        thetaellipse.push_back( qAtan2(newY.at(k), newX.at(k)) );
        rhoellipse.push_back( qSqrt(newX.at(k) * newX.at(k) + newY.at(k) * newY.at(k)) );
        thetarotellipse.push_back( thetaellipse.at(k) - rot );
        Xellipse.push_back( rhoellipse.at(k) * qCos(thetarotellipse.at(k)) );
        Yellipse.push_back( rhoellipse.at(k) * qSin(thetarotellipse.at(k)) );
        XellipseFinal.push_back( Xellipse.at(k) + cen[0] );
        YellipseFinal.push_back( Yellipse.at(k) + cen[1] );
    }
    area.push_back(M_PI * aa * bb);

    QVector<QVector<double>> dataOut;
    dataOut.push_back(XellipseFinal);
    dataOut.push_back(YellipseFinal);
    dataOut.push_back(area);

    return dataOut;
}

void MainWindow::generarImagenes(QString selectedFolder)
{
    QString lateralName = QString("%1/Imagenes/SenalLateral.png").arg(selectedFolder);
    ui->plotLateral->savePng(lateralName, 0, 0, 4, -1);
    QString anteroposteriorName = QString("%1/Imagenes/SenalAnteroPosterior.png").arg(selectedFolder);
    ui->plotAnteroPosterior->savePng(anteroposteriorName, 0, 0, 4, -1);

    ui->cmbxParametros->setCurrentIndex(0);
    for (int i = 1; i <= estudios.length(); i++) {
        ui->cmbxParametros->setCurrentIndex(i);

        QString elipseName = QString("%1/Imagenes/Elipse%2.png").arg(selectedFolder).arg(estudios.at(i-1));
        ui->plotElipse->savePng(elipseName, 331, 326, 4, -1);

        QString convexName = QString("%1/Imagenes/Convex%2.png").arg(selectedFolder).arg(estudios.at(i-1));
        ui->plotConvex->savePng(convexName, 331, 326, 4, -1);

        QString pcaName = QString("%1/Imagenes/ElipsePca%2.png").arg(selectedFolder).arg(estudios.at(i-1));
        ui->plotPca->savePng(pcaName, 331, 326, 4, -1);
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->frame->setGeometry(0,0,this->width(),90);
    // Separadores ----------------------------------------------------------------------
    ui->line_8->setGeometry(210, 125, 20, this->height() - 150);
    ui->line_9->setGeometry(this->width() - 285, 125, 20, static_cast<int>(this->height() - 150));
    // Sección Gráficas -----------------------------------------------------------------
    ui->plotLateral->setGeometry(230, 125, ui->line_9->x() - 230, static_cast<int>(this->height() / 5));
    ui->plotAnteroPosterior->setGeometry(230, 130 + ui->plotLateral->height(), ui->line_9->x() - 230, static_cast<int>(this->height() / 5));
    ui->cmbxParametros->setGeometry(static_cast<int>(ui->plotLateral->width() / 2) + 110, 95, 240, 25);

    int h = this->height() - (2 * ui->plotAnteroPosterior->height()) - 190;
    int w = static_cast<int>((ui->plotLateral->width() - 10) / 3);
    int y = ui->plotAnteroPosterior->y() + ui->plotAnteroPosterior->height() + 5;

    ui->plotElipse->setGeometry(230, y, w, h);
    ui->plotConvex->setGeometry(235 + w, y, w , h);
    ui->plotPca->setGeometry(240 + (2*w), y, w, h);

    ui->lblElipseTItle->setGeometry(230, this->height() - 50, w, 25);
    ui->lblConvexTitle->setGeometry(235 + w, this->height() - 50, w, 25);
    ui->lblPCATitle->setGeometry(240 + (2*w), this->height() - 50, w, 25);

    // Sección Parámetros ---------------------------------------------------------------
    ui->parametrosTitle->setGeometry(this->width() - 265, 155, 245, 25);

    ui->lblParam1->setGeometry(this->width() - 265,190,160,25);
    ui->lblParam2->setGeometry(this->width() - 265,225,160,25);
    ui->lblParam3->setGeometry(this->width() - 265,260,160,25);
    ui->lblParam4->setGeometry(this->width() - 265,295,160,25);
    ui->lblParam5->setGeometry(this->width() - 265,330,160,25);
    ui->lblParam6->setGeometry(this->width() - 265,365,160,25);
    ui->lblParam7->setGeometry(this->width() - 265,400,160,25);
    ui->lblParam8->setGeometry(this->width() - 265,435,160,25);
    ui->lblParam9->setGeometry(this->width() - 265,470,160,25);
    ui->lblParam10->setGeometry(this->width() - 265,505,160,25);
    ui->lblParam11->setGeometry(this->width() - 265,540,160,25);
    ui->lblParam12->setGeometry(this->width() - 265,575,160,25);

    ui->lblValueParam1->setGeometry(this->width() - 105,190,50,25);
    ui->lblValueParam2->setGeometry(this->width() - 105,225,50,25);
    ui->lblValueParam3->setGeometry(this->width() - 105,260,50,25);
    ui->lblValueParam4->setGeometry(this->width() - 105,295,50,25);
    ui->lblValueParam5->setGeometry(this->width() - 105,330,50,25);
    ui->lblValueParam6->setGeometry(this->width() - 105,365,50,25);
    ui->lblValueParam7->setGeometry(this->width() - 105,400,50,25);
    ui->lblValueParam8->setGeometry(this->width() - 105,435,50,25);
    ui->lblValueParam9->setGeometry(this->width() - 105,470,50,25);
    ui->lblValueParam10->setGeometry(this->width() - 105,505,50,25);
    ui->lblValueParam11->setGeometry(this->width() - 105,540,50,25);
    ui->lblValueParam12->setGeometry(this->width() - 105,575,50,25);

    ui->lblUnitsParam->setGeometry(this->width() - 50,190,30,25);
    ui->lblUnitsParam_2->setGeometry(this->width() - 50,225,30,25);
    ui->lblUnitsParam_3->setGeometry(this->width() - 50,260,30,25);
    ui->lblUnitsParam_4->setGeometry(this->width() - 50,295,30,25);
    ui->lblUnitsParam_5->setGeometry(this->width() - 50,330,30,25);
    ui->lblUnitsParam_6->setGeometry(this->width() - 50,365,30,25);
    ui->lblUnitsParam_7->setGeometry(this->width() - 50,400,30,25);
    ui->lblUnitsParam_8->setGeometry(this->width() - 50,435,30,25);
    ui->lblUnitsParam_9->setGeometry(this->width() - 50,470,30,25);
    ui->lblUnitsParam_10->setGeometry(this->width() - 50,505,30,25);
    ui->lblUnitsParam_11->setGeometry(this->width() - 50,540,30,25);
    ui->lblUnitsParam_12->setGeometry(this->width() - 50,575,30,25);
}
