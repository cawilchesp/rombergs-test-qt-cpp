#include "loadwindow.h"
#include "ui_loadwindow.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>

LoadWindow::LoadWindow(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::LoadWindow)
{
    ui->setupUi(this);

    //---------------------------- Valores de Configuración -----------------------------
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);                                  // Archivo de configuración
    QString dataFolder = testRombergSettings.value("Data_Folder","C:\\").toString();                             // Carpeta de datos
    temaDefault = testRombergSettings.value("Theme",0).toInt();

    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(dataFolder);
    dirmodel->setReadOnly(true);

    QStringList filters;
    filters << "*.txt" << "*.emt";

    dirmodel->setNameFilters(filters);
    dirmodel->setNameFilterDisables(false);
    ui->treeView->setModel(dirmodel);

    for (int i = 1; i < dirmodel->columnCount(); i++) {
        ui->treeView->hideColumn(i);
    }
    QModelIndex idx = dirmodel->index(dataFolder);
    ui->treeView->setRootIndex(idx);
    ui->treeView->resizeColumnToContents(0);

    // Tema -----------------------------------------------------------------------------
    if (temaDefault == 0) {
        // Tema Claro
        this->setStyleSheet("QDialog#LoadWindow {background-color: rgb(240,240,240);}");
        ui->lblNombreEstudio->setStyleSheet("QLabel {color: rgb(0,0,0)}");
        ui->editNombreEstudio->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: white; padding: 0 12 0 12}");
        ui->btnAgregar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid;	border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnAdd->setStyleSheet("QToolButton {border: 0px solid; 	border-radius: 12;	background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; 	border-radius: 12;	background-color: rgb(178,178,178)}");
        ui->btnCancelar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid;	border-radius: 12; background-color: rgb(178,178,178)}");
        ui->btnRemove->setStyleSheet("QToolButton {border: 0px solid; 	border-radius: 12;	background-color: rgb(240,240,240)} QToolButton:hover {border: 0px solid; 	border-radius: 12;	background-color: rgb(178,178,178)}");
        ui->listWidget->setStyleSheet("background-color: white");
        ui->treeView->setStyleSheet("background-color: white");
    } else if (temaDefault == 1) {
        // Tema Oscuro
        this->setStyleSheet("QDialog#LoadWindow {background-color: rgb(68,68,68);}");
        ui->lblNombreEstudio->setStyleSheet("QLabel {color: rgb(178,178,178)}");
        ui->editNombreEstudio->setStyleSheet("QLineEdit {border: 0px solid; border-radius: 12; background-color: rgb(240,240,240); padding: 0 12 0 12}");
        ui->btnAgregar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid;	border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnAdd->setStyleSheet("QToolButton {border: 0px solid; 	border-radius: 12;	background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; 	border-radius: 12;	background-color: rgb(240,240,240)}");
        ui->btnCancelar->setStyleSheet("QToolButton {border: 0px solid; border-radius: 12; background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid;	border-radius: 12; background-color: rgb(240,240,240)}");
        ui->btnRemove->setStyleSheet("QToolButton {border: 0px solid; 	border-radius: 12;	background-color: rgb(178,178,178)} QToolButton:hover {border: 0px solid; 	border-radius: 12;	background-color: rgb(240,240,240)}");
        ui->listWidget->setStyleSheet("background-color: rgb(178,178,178)");
        ui->treeView->setStyleSheet("background-color: rgb(178,178,178)");
    }
}

LoadWindow::~LoadWindow()
{
    delete ui;
}

void LoadWindow::receiveLoadEstudios(QStringList &estudios)
{
    mainEstudios = estudios;
}

void LoadWindow::on_editNombreEstudio_textChanged()
{
    if ((ui->listWidget->count() == 0) || (ui->editNombreEstudio->text() == "")) {
        ui->btnAgregar->setEnabled(false);
    } else {
        ui->btnAgregar->setEnabled(true);
    }
}

void LoadWindow::on_btnAdd_clicked()
{
    QModelIndexList treeSelectedList = ui->treeView->selectionModel()->selectedRows();
    for (int i = 0; i < treeSelectedList.size(); i++) {
        textFileName.push_back(dirmodel->filePath(treeSelectedList.at(i)));
        fileNames.push_back(dirmodel->fileName(treeSelectedList.at(i)));
    }
    ui->listWidget->clear();
    ui->listWidget->addItems(fileNames);

    if ((ui->listWidget->count() == 0) || (ui->editNombreEstudio->text() == "")) {
        ui->btnAgregar->setEnabled(false);
    } else {
        ui->btnAgregar->setEnabled(true);
    }
}

void LoadWindow::on_btnRemove_clicked()
{
    QModelIndexList indexes = ui->listWidget->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexes) {
        textFileName.replace(index.row(),"");
        fileNames.replace(index.row(),"");
    }
    textFileName.removeAll("");
    fileNames.removeAll("");

    ui->listWidget->clear();
    ui->listWidget->addItems(fileNames);

    if ((ui->listWidget->count() == 0) || (ui->editNombreEstudio->text() == "")) {
        ui->btnAgregar->setEnabled(false);
    } else {
        ui->btnAgregar->setEnabled(true);
    }
}

void LoadWindow::on_btnAgregar_clicked()
{
    if (mainEstudios.contains( ui->editNombreEstudio->text() ) == false) {
        QVector<QStringList> dataEstudios;
        mainEstudios.push_back(ui->editNombreEstudio->text());
        dataEstudios.push_back(mainEstudios);
        dataEstudios.push_back(textFileName);
        emit sendLoadEstudios(dataEstudios);
        this->close();
    } else {
        QMessageBox::information(this, tr("Advertencia"), tr("Ya existe un estudio con este nombre"));
    }
}

void LoadWindow::on_btnCancelar_clicked()
{
    textFileName.clear();
    this->close();
}

void LoadWindow::on_treeView_expanded()
{
    ui->treeView->resizeColumnToContents(0);
}
