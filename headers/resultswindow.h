#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include "source.h"
#include "device.h"
#include "event.h"
#include "automodewindow.h"

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <iostream>
#include <QtDebug>

namespace Ui {
class ResultsWindow;
}

class ResultsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWindow(QWidget *parent = nullptr);
    ~ResultsWindow();

    void setSources(std::vector<Source*> sources);
    void setDevices(std::multimap<int, Device *> devices);
    void setEvents(std::vector<event_t> events);

    std::vector<Source*> getSources();
    std::multimap<int, Device *> getDevices();
    std::vector<event_t> moveEvents();

    void setBufferSize(int size);
    int getBufferSize();
private slots:
    void on_backToMenuButton_clicked();

    void on_showAutoModeWindowButton_clicked();

    void on_helpButton_clicked();

public slots:
    void getStatistics();

signals:
    void back();
    void statisticsCollected();


private:
    Ui::ResultsWindow *ui;
    std::vector<Source*> sources_;
    std::multimap<int, Device *> devices_;
    std::vector<event_t> events_;
    AutoModeWindow *autoModeWindow;
    int sizeOfBuffer;
};

#endif // RESULTSWINDOW_H
