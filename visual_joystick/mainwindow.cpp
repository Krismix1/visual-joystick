#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qhidapi.h>
#include <QByteArray>
#include <QThread>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    btnStateLabels.append(ui->btn1StateLabel);
    btnStateLabels.append(ui->btn2StateLabel);
    btnStateLabels.append(ui->btn3StateLabel);
    btnStateLabels.append(ui->btn4StateLabel);
    btnStateLabels.append(ui->btn5StateLabel);
    btnStateLabels.append(ui->btn6StateLabel);
    btnStateLabels.append(ui->btn7StateLabel);
    btnStateLabels.append(ui->btn8StateLabel);
    btnStateLabels.append(ui->btn9StateLabel);
    btnStateLabels.append(ui->btn10StateLabel);
    btnStateLabels.append(ui->btn11StateLabel);
    btnStateLabels.append(ui->btn12StateLabel);


    dPadLabels[0] = ui->dPad1;
    dPadLabels[1] = ui->dPad2;
    dPadLabels[2] = ui->dPad3;
    dPadLabels[3] = ui->dPad4;
    dPadLabels[4] = ui->dPad5;
    dPadLabels[5] = ui->dPad6;
    dPadLabels[6] = ui->dPad7;
    dPadLabels[7] = ui->dPad8;
    dPadLabels[8] = ui->dPad9;

    pHidApi = new QHidApi;
    scanConnectedHidDevices();
//    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <bitset>

void MainWindow::readData(){
    QByteArray mReadData;
    mReadData = pHidApi->read(mId);
    if(!mReadData.isEmpty()){
        uint16_t x_axis = (uint8_t) mReadData.at(0) | (uint16_t) (((uint8_t) (mReadData.at(1) & 0x0F)) << 8);
        uint8_t y_axis = ((mReadData.at(1) & 0xF0) >> 4) | ((mReadData.at(2) & 0x0F) << 4);
        uint8_t z_axis = mReadData.at(3);
        uint8_t throttle = mReadData.at(5);

        ui->progressBar->setValue(x_axis);
        ui->progressBar_2->setValue(y_axis);
        ui->progressBar_3->setValue(z_axis);
        ui->progressBar_4->setValue(throttle);

        uint8_t btnValue = mReadData.at(4);
        uint8_t btnValue2 = mReadData.at(6);
        uint8_t dPadValue = ((uint8_t)mReadData.at(2)) >> 4;

        for(uint8_t i = 0; i<8; i++){ // change to an iterator
            QLabel* currLabel = btnStateLabels.at(i);
            if(btnValue & (1 << i)){
                currLabel->setText("On");
                currLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
            }else{
                currLabel->setText("Off");
                currLabel->setStyleSheet("QLabel { background-color : red; color : black; }");
            }
        }
        for(uint8_t i = 0; i < 4; i++){
            QLabel* currLabel = btnStateLabels.at(i+8);
            if(btnValue2 & (1 << i)){
                currLabel->setText("On");
                currLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
            }else{
                currLabel->setText("Off");
                currLabel->setStyleSheet("QLabel { background-color : red; color : black; }");
            }
        }


        QLabel* currentLabel = dPadLabels[dPadValue];
        currentLabel->setStyleSheet("QLabel { background-image: url(:/images/joystick_thumb.png); background-position: center };");
        if(lastDPadLabel && currentLabel != lastDPadLabel){ // not null
            lastDPadLabel->setStyleSheet("QLabel { background-image : none; }");
        }
        lastDPadLabel = currentLabel;
    }else{
        qDebug("no data received");
    }
}


void MainWindow::on_pushButton_clicked()
{
    int selectedItemIndex = ui->listWidget->currentRow();
    QHidDeviceInfo selectedDevice = connectedHidDevices.at(selectedItemIndex);
    uint vendor = selectedDevice.vendorId/*0x046d*/;
    uint product = selectedDevice.productId/*0xc215*/;

    mId = pHidApi->open(vendor, product, NULL);
    if(mId){
        qDebug("Opened the device");
        pHidApi->setNonBlocking(mId); // set the read() to be non-blocking
    }else{
        qCritical("Couldn't open device");
    }

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    timer->start(1);

}

void MainWindow::scanConnectedHidDevices(ushort vendorId, ushort productId){

    connectedHidDevices = pHidApi->enumerate(vendorId, productId);
    ui->listWidget->clear();
    foreach(QHidDeviceInfo deviceInfo, connectedHidDevices){
        QString vendorNumber;
        vendorNumber.setNum(deviceInfo.vendorId, 16);
        vendorNumber.insert(0, "0x");
        QString productNumber;
        productNumber.setNum(deviceInfo.productId, 16);
        productNumber.insert(0, "0x");
        QString info = deviceInfo.manufacturerString + " " + deviceInfo.productString + " " + vendorNumber + " : " + productNumber;
        ui->listWidget->addItem(info);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    scanConnectedHidDevices();
}
