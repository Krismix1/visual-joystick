#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <qhidapi.h>

class Joystick
{
public:
    Joystick(uint vendorId, uint productId);

    const uint8_t buttonsCount();
    const uint8_t axisCount();
    const quint32 getDeviceId(){
        return deviceId;
    }

signals:
    void dataReceived();
private:
    quint32 deviceId;
    QHidApi *pHidApi;
};

#endif // JOYSTICK_H
