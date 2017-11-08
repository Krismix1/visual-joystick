#include "joystick.h"

Joystick::Joystick(uint vendorId, uint productId)
{
    deviceId = pHidApi->open(vendorId, productId);
}

const uint8_t Joystick::buttonsCount(){
    return (const uint8_t)12;
}

const uint8_t Joystick::axisCount(){
    return (const uint8_t)3;
}
