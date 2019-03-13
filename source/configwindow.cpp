#include "configwindow.h"
#include "ui_configwindow.h"
#include <QDebug>

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    ui->lineForSources->setValidator(new QIntValidator(1, 100000, this));
    ui->lineForRequests->setValidator(new QIntValidator(1, 100000, this));
    ui->lineForDevices->setValidator(new QIntValidator(1, 100000, this));
    ui->lineForBufer->setValidator(new QIntValidator(1, 100000, this));
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::on_backToMenuButton_clicked()
{
    this->close();
    emit back();
}

int ConfigWindow::getNumberOfSources()
{
    return ui->lineForSources->text().toInt();
}

int ConfigWindow::getNumberOfDevices()
{
    return ui->lineForDevices->text().toInt();
}

int ConfigWindow::getSizeOfBuffer()
{
    return ui->lineForBufer->text().toInt();
}

int ConfigWindow::getNumberOfRequests()
{
    return ui->lineForRequests->text().toInt();
}

float ConfigWindow::getAlpha()
{
    return ui->lineForAlpha->text().toFloat();
}

float ConfigWindow::getBeta()
{
    return ui->lineForBeta->text().toFloat();
}

float ConfigWindow::getLambda()
{
    return ui->lineForLambda->text().toFloat();
}

void ConfigWindow::on_okButton_clicked()
{
    int numberOfSources = getNumberOfSources();
    int numberOfDevices = getNumberOfDevices();
    int numberOfRequests = getNumberOfRequests();
    int sizeOfBuffer = getSizeOfBuffer();
    float lambda = getLambda();
    float alpha = getAlpha();;
    float beta = getBeta();;
    qDebug() << lambda;

    if (numberOfSources > 0 && sizeOfBuffer > 0 && numberOfDevices > 0
        && numberOfRequests > 0 && alpha >= 0 && beta >= 0 && lambda > 0) {
        ui->lineForBeta->setStyleSheet("border: 2px solid green");
        ui->lineForAlpha->setStyleSheet("border: 2px solid green");
        ui->lineForLambda->setStyleSheet("border: 2px solid green");
        ui->lineForSources->setStyleSheet("border: 2px solid green");
        ui->lineForDevices->setStyleSheet("border: 2px solid green");
        ui->lineForRequests->setStyleSheet("border: 2px solid green");
        ui->lineForBufer->setStyleSheet("border: 2px solid green");
        QMessageBox msgSuccess;
        msgSuccess.setText("Система сконфигурирована успешно");
        msgSuccess.setIcon(QMessageBox::Information);
        msgSuccess.setDefaultButton(QMessageBox::Ok);
        msgSuccess.exec();

        emit configured();
        this->close();
        emit back();
    }
    else
    {
        if (numberOfSources > 0)
        {
            ui->lineForSources->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForSources->setStyleSheet("border: 2px solid red");
        }

        if (numberOfDevices > 0)
        {
            ui->lineForDevices->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForDevices->setStyleSheet("border: 2px solid red");
        }

        if (numberOfRequests > 0)
        {
            ui->lineForRequests->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForRequests->setStyleSheet("border: 2px solid red");
        }

        if (sizeOfBuffer > 0)
        {
            ui->lineForBufer->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForBufer->setStyleSheet("border: 2px solid red");
        }

        if (lambda > 0)
        {
            ui->lineForLambda->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForLambda->setStyleSheet("border: 2px solid red");
        }

        if (alpha > 0)
        {
            ui->lineForAlpha->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForAlpha->setStyleSheet("border: 2px solid red");
        }

        if (beta > 0)
        {
            ui->lineForBeta->setStyleSheet("border: 2px solid green");
        }
        else
        {
            ui->lineForBeta->setStyleSheet("border: 2px solid red");
        }

        QMessageBox msgCritical;
        msgCritical.setText("Конфигурация системы с такими параметрами невозможна");
        msgCritical.setInformativeText("Измените параметры и сконфигурируйте заново");
        msgCritical.setIcon(QMessageBox::Critical);
        msgCritical.setDefaultButton(QMessageBox::Ok);
        msgCritical.exec();
        emit configureError();
    }
}

void ConfigWindow::on_optimalButton_clicked()
{
    ui->lineForSources->setText("10");
    ui->lineForDevices->setText("14");
    ui->lineForBufer->setText("20");
    ui->lineForRequests->setText("10000");
    ui->lineForLambda->setText("2.05");
    ui->lineForAlpha->setText("1.0");
    ui->lineForBeta->setText("0.4");
}
