#ifndef ZCAI047_GRAPHICS_H
#define ZCAI047_GRAPHICS_H

#include "spiAVR.h"
#include "helper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>

// CS - pin 10
// SCK - pin 13
// SDA - pin 11
// A0 - C5 (Analog pin A5), 
// RESET - B0 (digital pin 8),  
// AO (0 is for command, 1 is for data)

void HARDWARE_RESET() {
    PORTB = SetBit(PORTB,0,0);
    _delay_ms(200);
    PORTB = SetBit(PORTB,0,1);
    _delay_ms(200);
}

void ST7735_INIT() {
    HARDWARE_RESET();
    PORTB = SetBit(PORTB,2,0); // CS pin to 0
    PORTC = SetBit(PORTC,5,0); // A0 pin to 0 
    SPI_SEND(0x01); // SWRESET
    _delay_ms(150);
    SPI_SEND(0x11); // SLPOUT
    _delay_ms(200);
    SPI_SEND(0x3A); // COLMOD
    PORTC = SetBit(PORTC,5,1);
    SPI_SEND(0x06); // 18 bit color pixel
    _delay_ms(10);
    PORTC = SetBit(PORTC,5,0);
    SPI_SEND(0x29); // dispon
    _delay_ms(200);
    PORTB = SetBit(PORTB,2,1);
}

void cmd(unsigned char cmd) {
    PORTB = SetBit(PORTB,2,0);
    PORTC = SetBit(PORTC,5,0); // A0
    SPI_SEND(cmd);
    PORTC = SetBit(PORTC,5,1);
}

void data(unsigned char start,unsigned char end) {
    SPI_SEND(0x00);
    SPI_SEND(start);
    SPI_SEND(0x00);
    SPI_SEND(end);
    PORTB = SetBit(PORTB,2,1); //CS
}

void draw(unsigned char color1, unsigned char color2, unsigned char color3, int DX, int DY) {    
    for (int i = 0; i < (DX+1)*(DY+1); i++) { // Area of box: (rowEnd - rowStart) * (columnEnd - columnStart)
        SPI_SEND(color1);
        SPI_SEND(color2);
        SPI_SEND(color3);
    }
    PORTB = SetBit(PORTB,2,1);
}

#endif