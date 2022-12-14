#include "reporteditwindow.h"
#include "ui_reporteditwindow.h"

ReportEditWindow::ReportEditWindow(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::ReportEditWindow)
{
    ui->setupUi(this);

    //---------------------------- Valores de Configuración -----------------------------
    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);                                  // Archivo de configuración
    reportTexts = testRombergSettings.value("Report_Texts").toStringList();

    if (reportTexts.isEmpty()) {
        ui->lineEditTitulo->setText(tr("Título"));
        ui->editIntro->setPlainText(tr("Introducción"));
        ui->editSub1_1->setText(tr("Subtítulo 1_1"));
        ui->editSub1_2->setText(tr("Subtítulo 1_2"));
        ui->editParagraph1->setPlainText(tr("Párrafo 1"));
        ui->editSub1_3->setText(tr("Subtítulo 1_3"));
        ui->editSub2_1->setText(tr("Subtítulo 2_1"));
        ui->editSub2_2->setText(tr("Subtítulo 2_2"));
        ui->editSub2_3->setText(tr("Subtítulo 2_3"));
        ui->editSub1_4->setText(tr("Subtítulo 1_4"));
    } else {
        ui->lineEditTitulo->setText(reportTexts[0]);
        ui->editIntro->setPlainText(reportTexts[1]);
        ui->editSub1_1->setText(reportTexts[2]);
        ui->editSub1_2->setText(reportTexts[3]);
        ui->editParagraph1->setPlainText(reportTexts[4]);
        ui->editSub1_3->setText(reportTexts[5]);
        ui->editSub2_1->setText(reportTexts[6]);
        ui->editSub2_2->setText(reportTexts[7]);
        ui->editSub2_3->setText(reportTexts[8]);
        ui->editSub1_4->setText(reportTexts[9]);
    }
}

ReportEditWindow::~ReportEditWindow()
{
    delete ui;
}

void ReportEditWindow::on_btnAceptar_clicked()
{
    reportTexts.clear();
    reportTexts.push_back(ui->lineEditTitulo->text());
    reportTexts.push_back(ui->editIntro->toPlainText());
    reportTexts.push_back(ui->editSub1_1->text());
    reportTexts.push_back(ui->editSub1_2->text());
    reportTexts.push_back(ui->editParagraph1->toPlainText());
    reportTexts.push_back(ui->editSub1_3->text());
    reportTexts.push_back(ui->editSub2_1->text());
    reportTexts.push_back(ui->editSub2_2->text());
    reportTexts.push_back(ui->editSub2_3->text());
    reportTexts.push_back(ui->editSub1_4->text());

    QSettings testRombergSettings("config.ini" , QSettings::IniFormat);
    testRombergSettings.setValue("Report_Texts", reportTexts);

    this->close();
}

void ReportEditWindow::on_btnCancelar_clicked()
{
    this->close();
}
