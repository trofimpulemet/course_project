#include "automodewindow.h"
#include "ui_automodewindow.h"

AutoModeWindow::AutoModeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoModeWindow)
{
    ui->setupUi(this);
    ui->lineEditForTime->setReadOnly(true);
    ui->lineEditForStep->setReadOnly(true);
    ui->lineEditForAllSteps->setReadOnly(true);
    step = 0;
}

AutoModeWindow::~AutoModeWindow()
{
    delete ui;
}

void AutoModeWindow::setEvents(std::vector<event_t> events)
{
    events_ = events;
}

void AutoModeWindow::setSources(std::vector<Source *> sources)
{
    sources_ = sources;
}

void AutoModeWindow::setBufferSize(int size)
{
    sizeOfBuffer = size;
}

void AutoModeWindow::setDevices(std::multimap<int, Device *> devices)
{
    devices_ = devices;
}

void AutoModeWindow::on_backButton_clicked()
{
    this->close();
    emit back();
}

void AutoModeWindow::prepareAutoModeWindow()
{
    ui->lineEditForOtherStep->setValidator(new QIntValidator(0, events_.size() - 1, this));

    ui->lineEditForAllSteps->setText(QString::number(events_.size() - 1));
    ui->backStepButton->setEnabled(false);

    modelForDevices = new QStandardItemModel;
    QStringList horizontalHeadersForResults;
    horizontalHeadersForResults.append("Состояние");
    horizontalHeadersForResults.append("Источник");
    horizontalHeadersForResults.append("Заявка");
    horizontalHeadersForResults.append("Время освобождения");
    modelForDevices->setHorizontalHeaderLabels(horizontalHeadersForResults);

    QStringList verticalHeadersForResults;
    for (int i = 0; i < devices_.size(); i++)
    {
        QString index = QString::number(i + 1);
        verticalHeadersForResults.append("Прибор " + index);
    }
    modelForDevices->setVerticalHeaderLabels(verticalHeadersForResults);
    ui->deviceTable->setModel(modelForDevices);
    ui->deviceTable->resizeRowsToContents();
    ui->deviceTable->resizeColumnsToContents();

    modelForSources = new QStandardItemModel;
    QStringList horizontalHeadersForSources;
    horizontalHeadersForSources.append("Число заявок");
    horizontalHeadersForSources.append("Число отказов");
    modelForSources->setHorizontalHeaderLabels(horizontalHeadersForSources);

    QStringList verticalHeadersForSources;
    for (int i = 0; i < sources_.size(); i++)
    {
        QString index = QString::number(i + 1);
        verticalHeadersForSources.append("Источник " + index);
    }
    modelForSources->setVerticalHeaderLabels(verticalHeadersForSources);
    ui->sourceTable->setModel(modelForSources);
    ui->sourceTable->resizeRowsToContents();
    ui->sourceTable->resizeColumnsToContents();

    modelForBuffer = new QStandardItemModel;
    QStringList horizontalHeadersForBuffer;
    horizontalHeadersForBuffer.append("Источник");
    horizontalHeadersForBuffer.append("Заявка");
    horizontalHeadersForBuffer.append("Время загрузки в буфер");
    modelForBuffer->setHorizontalHeaderLabels(horizontalHeadersForBuffer);

    QStringList verticalHeadersForBuffer;
    for (int i = 0; i < sizeOfBuffer; i++)
    {
        QString index = QString::number(i + 1);
        verticalHeadersForBuffer.append("Позиция " + index);
    }
    modelForBuffer->setVerticalHeaderLabels(verticalHeadersForBuffer);
    ui->bufferTable->setModel(modelForBuffer);
    ui->bufferTable->resizeRowsToContents();
    ui->bufferTable->resizeColumnsToContents();
    showCurrentEvent();
}

void AutoModeWindow::showCurrentEvent()
{
    if (step == 0)
    {
        ui->backStepButton->setEnabled(false);
    }
    else
    {
        ui->backStepButton->setEnabled(true);
    }
    if (step == events_.size() - 1)
    {
        ui->nextStepButton->setEnabled(false);
    }
    else
    {
        ui->nextStepButton->setEnabled(true);
    }

    QStandardItem *item;
    event_t event = events_[step];

    ui->lineEditForTime->setText(QString::number(event.time));
    ui->lineEditForStep->setText(QString::number(step + 1));
    ui->textBrowserForEvent->setText(event.info);

    std::vector<device_state_t> deviceStates = event.devices_state;
    for (int i = 0; i < deviceStates.size(); i++)
    {
        if (deviceStates[i].is_free == true)
        {
            item = new QStandardItem("Ожидает");
            modelForDevices->setItem(i, 0, item);
            item = new QStandardItem("");
            modelForDevices->setItem(i, 1, item);
            item = new QStandardItem("");
            modelForDevices->setItem(i, 2, item);
            item = new QStandardItem(QString::number(deviceStates[i].releaseTime));
            modelForDevices->setItem(i, 3, item);
        }
        else
        {
            item = new QStandardItem("Занят");
            modelForDevices->setItem(i, 0, item);
            item = new QStandardItem(QString::number(deviceStates[i].request.numberOfSource));
            modelForDevices->setItem(i, 1, item);
            item = new QStandardItem(QString::number(deviceStates[i].request.number));
            modelForDevices->setItem(i, 2, item);
            item = new QStandardItem(QString::number(deviceStates[i].releaseTime));
            modelForDevices->setItem(i, 3, item);
        }
    }
    ui->deviceTable->setModel(modelForDevices);
    ui->deviceTable->resizeRowsToContents();
    ui->deviceTable->resizeColumnsToContents();

    std::vector<std::pair<unsigned long,
                          unsigned long>> source_states = event.sources_state;
    for (int i = 0; i < source_states.size(); i++)
    {
        item = new QStandardItem(QString::number(source_states[i].first));
        modelForSources->setItem(i, 0, item);
        item = new QStandardItem(QString::number(source_states[i].second));
        modelForSources->setItem(i, 1, item);
    }
    ui->sourceTable->setModel(modelForSources);
    ui->sourceTable->resizeRowsToContents();
    ui->sourceTable->resizeColumnsToContents();

    std::vector<buffer_state_t> buffer_state = event.buffer_state;
    for (int i = 0; i < buffer_state.size(); i++)
    {
        if (buffer_state[i].is_free)
        {
            item = new QStandardItem(QString::number(0));
            modelForBuffer->setItem(i, 0, item);
            item = new QStandardItem(QString::number(0));
            modelForBuffer->setItem(i, 1, item);
            item = new QStandardItem("");
            modelForBuffer->setItem(i, 2, item);
        }
        else
        {
            item = new QStandardItem(QString::number(buffer_state[i].request.numberOfSource));
            modelForBuffer->setItem(i, 0, item);
            item = new QStandardItem(QString::number(buffer_state[i].request.number));
            modelForBuffer->setItem(i, 1, item);
            item = new QStandardItem(QString::number(buffer_state[i].request.time));
            modelForBuffer->setItem(i, 2, item);
        }
    }
    ui->bufferTable->setModel(modelForBuffer);
    ui->bufferTable->resizeRowsToContents();
    ui->bufferTable->resizeColumnsToContents();
}

void AutoModeWindow::on_backStepButton_clicked()
{
    step--;
    showCurrentEvent();
}

void AutoModeWindow::on_nextStepButton_clicked()
{
    step++;
    showCurrentEvent();
}

void AutoModeWindow::on_buttonForOtherStep_clicked()
{
    int stepToGo = ui->lineEditForOtherStep->text().toInt();
    if (stepToGo >= 0 && stepToGo <= events_.size() - 1)
    {
        step = stepToGo;
        showCurrentEvent();
        ui->lineEditForOtherStep->clear();
    }
    else
    {
        QMessageBox msgCritical;
        msgCritical.setText("Переход на этот шаг невозможен");
        msgCritical.setInformativeText("Введите номер шага от 0 до " + QString::number(events_.size() - 1));
        msgCritical.setIcon(QMessageBox::Critical);
        msgCritical.setDefaultButton(QMessageBox::Ok);
        msgCritical.exec();
    }
}
