#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QValidator>

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();
    int getNumberOfSources();
    int getNumberOfDevices();
    int getNumberOfRequests();
    int getSizeOfBuffer();
    float getLambda();
    float getAlpha();
    float getBeta();

signals:
    void configured();
    void back();
    void configureError();

private slots:
    void on_backToMenuButton_clicked();

    void on_okButton_clicked();

    void on_optimalButton_clicked();

private:
    Ui::ConfigWindow *ui;
};

#endif // CONFIGWINDOW_H
