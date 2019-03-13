#ifndef AUTOMODEWINDOW_H
#define AUTOMODEWINDOW_H

#include "event.h"
#include "device.h"
#include "source.h"

#include <QWidget>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>

namespace Ui {
class AutoModeWindow;
}

class AutoModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AutoModeWindow(QWidget *parent = nullptr);
    ~AutoModeWindow();
    void setEvents(std::vector<event_t> events);
    void setSources(std::vector<Source *> sources);
    void setDevices(std::multimap<int, Device *> devices);
    void setBufferSize(int size);
    void showCurrentEvent();

public slots:
    void prepareAutoModeWindow();

private slots:
    void on_backButton_clicked();

    void on_backStepButton_clicked();

    void on_nextStepButton_clicked();

    void on_buttonForOtherStep_clicked();

signals:
    void back();

private:
    int step;
    Ui::AutoModeWindow *ui;
    std::vector<event_t> events_;
    std::vector<Source *> sources_;
    std::multimap<int, Device *> devices_;
    QStandardItemModel *modelForDevices;
    QStandardItemModel *modelForSources;
    QStandardItemModel *modelForBuffer;
    int sizeOfBuffer;
};

#endif // AUTOMODEWINDOW_H
