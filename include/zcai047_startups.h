#ifndef ZCAI047_TITLE_H
#define ZCAI047_TITLE_H

#include "spiAVR.h"
#include "helper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
#include "zcai047_graphics.h"

void Make_Black_Screen() {
    cmd(0x2A);
    data(0,130);
    cmd(0x2B);
    data(0,130);
    cmd(0x2C);
    draw(0x00,0x00,0x00,130,130);
}

#endif 