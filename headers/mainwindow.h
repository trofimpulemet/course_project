#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configwindow.h"
#include "resultswindow.h"
#include "controller.h"

#include <QMainWindow>
#include <QMovie>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void handleStatistics();

    void setModelingButtonDisabled();

    void setConfigButtonEnabled();

public slots:
    void on_exitButton_clicked();

    void on_configButton_clicked();

    void on_resultsButton_clicked();

    void on_modelingButton_clicked();

    void setModelingButtonEnabled();


    void setResultsButtonEnabled();

    void setResultsButtonDisabled();


signals:
    void handled();


private:
    Ui::MainWindow *ui;
    ConfigWindow *configWindow;
    ResultsWindow *resultsWindow;
    Controller *controller;
    QMovie *movie;
};

#endif // MAINWINDOW_H
