#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qhidapi.h>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void scanConnectedHidDevices(ushort vendorId = 0, ushort productId=0);
    void resetUi();
private slots:
    void readData();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    quint32 mId;
    QHidApi *pHidApi;
    QList<QLabel *> btnStateLabels;
    QLabel* dPadLabels[9];
    QLabel* lastDPadLabel;

    QList<QHidDeviceInfo> connectedHidDevices;
};

#endif // MAINWINDOW_H
