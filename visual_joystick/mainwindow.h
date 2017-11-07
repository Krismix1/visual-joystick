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

private slots:
    void readData();

private:
    Ui::MainWindow *ui;
    quint32 mId;
    QHidApi *pHidApi;
    QList<QLabel *> btnStateLabels;
    QLabel* dPadLabels[9];
    QLabel* lastDPadLabel;
};

#endif // MAINWINDOW_H
