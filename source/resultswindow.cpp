#include "resultswindow.h"
#include "ui_resultswindow.h"

ResultsWindow::ResultsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultsWindow),
    autoModeWindow(new AutoModeWindow())
{
    ui->setupUi(this);
    connect(autoModeWindow, &AutoModeWindow::back, this, &ResultsWindow::show);
    connect(this, &ResultsWindow::statisticsCollected, autoModeWindow,
            &AutoModeWindow::prepareAutoModeWindow);
}

ResultsWindow::~ResultsWindow()
{
    delete ui;
    delete autoModeWindow;
}

void ResultsWindow::on_backToMenuButton_clicked()
{
    this->close();
    emit back();
}

void ResultsWindow::setSources(std::vector<Source*> sources)
{
    sources_ = sources;
}

void ResultsWindow::setDevices(std::multimap<int, Device *> devices)
{
    devices_ = devices;
}

std::vector<event_t> ResultsWindow::moveEvents()
{
    return events_;
}

void ResultsWindow::setEvents(std::vector<event_t> events)
{
    events_ = events;
}

void ResultsWindow::setBufferSize(int size)
{
    sizeOfBuffer = size;
}

std::vector<Source*> ResultsWindow::getSources()
{
    return sources_;
}

std::multimap<int, Device *> ResultsWindow::getDevices()
{
    return devices_;
}

int ResultsWindow::getBufferSize()
{
    return sizeOfBuffer;
}

void ResultsWindow::getStatistics()
{
    QStandardItemModel *modelForResults = new QStandardItemModel;
    QStringList horizontalHeadersForResults;
    horizontalHeadersForResults.append("Число обработанных заявок");
    horizontalHeadersForResults.append("Число отказов");
    horizontalHeadersForResults.append("СВвС");
    horizontalHeadersForResults.append("СВОж");
    horizontalHeadersForResults.append("СВОбр");
    horizontalHeadersForResults.append("ДСВОж");
    horizontalHeadersForResults.append("ДСВОбр");
    horizontalHeadersForResults.append("Вероятность отказа");
    modelForResults->setHorizontalHeaderLabels(horizontalHeadersForResults);

    QStringList verticalHeadersForResults;
    for (int i = 0; i < sources_.size(); i++)
    {
        QString index = QString::number(i + 1);
        verticalHeadersForResults.append("Источник " + index);
    }
    modelForResults->setVerticalHeaderLabels(verticalHeadersForResults);

    QStandardItem *item;
    for (int i = 0; i < sources_.size(); i++)
    {
        item = new QStandardItem(QString::number(sources_[i]->getRequestsProcessed()));
        modelForResults->setItem(i, 0, item);

        item = new QStandardItem(QString::number(sources_[i]->getRequestsFailed()));
        modelForResults->setItem(i, 1, item);

        item = new QStandardItem(QString::number(sources_[i]->getAvgTimeInSystem()));
        modelForResults->setItem(i, 2, item);

        item = new QStandardItem(QString::number(sources_[i]->getAvgWaitTimeInBuffer()));
        modelForResults->setItem(i, 3, item);

        item = new QStandardItem(QString::number(sources_[i]->getAvgProcessingTime()));
        modelForResults->setItem(i, 4, item);

        item = new QStandardItem(QString::number(sources_[i]->getDispersionOfTimeInBuffer()));
        modelForResults->setItem(i, 5, item);

        item = new QStandardItem(QString::number(sources_[i]->getDispersionOfTimeOnDevice()));
        modelForResults->setItem(i, 6, item);

        item = new QStandardItem(QString::number(sources_[i]->getProbabilityOfFailure()) + " %");
        modelForResults->setItem(i, 7, item);
    }

    ui->resultsTable->setModel(modelForResults);
    ui->resultsTable->resizeRowsToContents();
    ui->resultsTable->resizeColumnsToContents();

    QStandardItemModel *modelForDevices = new QStandardItemModel;
    QStringList horizontalsHeadersForDevices;
    horizontalsHeadersForDevices.append("Коэффициент занятости");
    modelForDevices->setHorizontalHeaderLabels(horizontalsHeadersForDevices);

    QStringList verticalHeadersForDevices;
    for (int i = 0; i < devices_.size(); i++)
    {
        QString index = QString::number(i + 1);
        verticalHeadersForDevices.append("Прибор " + index);
    }
    modelForDevices->setVerticalHeaderLabels(verticalHeadersForDevices);

    int i = 0;
    for (std::multimap<int, Device *>::iterator it = devices_.begin();
         it != devices_.end();
         it++, i++)
    {
        QString employment_rate = QString::number(it->second->getEmploymentRate());
        item = new QStandardItem(QString(employment_rate + " %"));
        modelForDevices->setItem(i, 0, item);
    }
    ui->deviceTable->setModel(modelForDevices);
    ui->deviceTable->resizeRowsToContents();
    ui->deviceTable->resizeColumnsToContents();
    autoModeWindow->setEvents(moveEvents());
    autoModeWindow->setSources(getSources());
    autoModeWindow->setDevices(getDevices());
    autoModeWindow->setBufferSize(getBufferSize());
    emit statisticsCollected();
}

void ResultsWindow::on_showAutoModeWindowButton_clicked()
{
    this->close();
    autoModeWindow->show();
}

void ResultsWindow::on_helpButton_clicked()
{
    QMessageBox msgSuccess;
    msgSuccess.setText("СВвС - среднее время пребывания в системе\n"
                       "СВОж - среднее время ожидания\n"
                       "СВОбр - среднее время обработки\n"
                       "ДСВОж - дисперсия среднего времени ожидания\n"
                       "ДСВОбр - дисперсия среднего времени обработки");
    msgSuccess.setIcon(QMessageBox::Information);
    msgSuccess.setDefaultButton(QMessageBox::Ok);
    msgSuccess.exec();
}
