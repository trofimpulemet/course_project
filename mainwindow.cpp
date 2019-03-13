#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configWindow(new ConfigWindow()),
    resultsWindow(new ResultsWindow())
{
    ui->setupUi(this);
    this->setWindowTitle("СМО");
    movie = new QMovie(":/images/loading.gif");
    ui->labelForLoading->setMovie(movie);
    ui->labelForLoading->setAlignment(Qt::AlignCenter);
    ui->labelForLoading->hide();
    ui->modelingButton->setEnabled(false);
    ui->resultsButton->setEnabled(false);
    connect(configWindow, &ConfigWindow::back, this, &MainWindow::show);
    connect(configWindow, &ConfigWindow::configured, this, &MainWindow::setModelingButtonEnabled);
    connect(configWindow, &ConfigWindow::configured, this, &MainWindow::setResultsButtonDisabled);
    connect(configWindow, &ConfigWindow::configureError, this, &MainWindow::setModelingButtonDisabled);
    connect(resultsWindow, &ResultsWindow::back, this, &MainWindow::show);
    connect(resultsWindow, &ResultsWindow::statisticsCollected, this, &MainWindow::setResultsButtonEnabled);
    connect(this, &MainWindow::handled, resultsWindow, &ResultsWindow::getStatistics);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
    delete resultsWindow;
    delete configWindow;
}

void MainWindow::on_exitButton_clicked()
{
    resultsWindow->close();
    configWindow->close();
    this->close();
}

void MainWindow::on_configButton_clicked()
{
    this->close();
    configWindow->show();
}

void MainWindow::on_resultsButton_clicked()
{
    this->close();
    resultsWindow->show();
}

void MainWindow::on_modelingButton_clicked()
{
    ui->resultsButton->setEnabled(false);
    ui->configButton->setEnabled(false);
    ui->modelingButton->setEnabled(false);
    //ui->labelForLoading->show();
    //movie->start();
    int numberOfSources = configWindow->getNumberOfSources();
    int numberOfDevices = configWindow->getNumberOfDevices();
    int sizeOfBuffer = configWindow->getSizeOfBuffer();
    int numberOfRequests = configWindow->getNumberOfRequests();
    float alpha = configWindow->getAlpha();
    float beta = configWindow->getBeta();
    float lambda = configWindow->getLambda();

    std::vector<int> priorities;
    for (int i = 0; i < numberOfDevices; i++)
    {
        priorities.push_back(i);
    }
    controller = new Controller(numberOfSources, numberOfDevices, sizeOfBuffer,
                                                numberOfRequests, alpha, beta, lambda, priorities);
    controller->modeling();
    handleStatistics();
    setConfigButtonEnabled();
    setModelingButtonEnabled();
}

void MainWindow::setModelingButtonEnabled()
{
    ui->modelingButton->setEnabled(true);
}

void MainWindow::setModelingButtonDisabled()
{
    ui->modelingButton->setEnabled(false);
}

void MainWindow::setResultsButtonEnabled()
{
    ui->resultsButton->setEnabled(true);
}

void MainWindow::setConfigButtonEnabled()
{
    ui->configButton->setEnabled(true);
}

void MainWindow::setResultsButtonDisabled()
{
    ui->resultsButton->setEnabled(false);
}

void MainWindow::handleStatistics()
{
    resultsWindow->setSources(controller->getSources());
    resultsWindow->setDevices(controller->getDevices());
    resultsWindow->setEvents(controller->getEvents());
    resultsWindow->setBufferSize(controller->getSizeOfBuffer());
    emit handled();
}
