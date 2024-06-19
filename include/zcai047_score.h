#ifndef ZCAI047_SCORE_H
#define ZCAI047_SCORE_H

#include "spiAVR.h"
#include "helper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
#include "zcai047_graphics.h"

// high score numbers from 116 to 128

void Write_Score(unsigned char color) {
    // S
    cmd(0x2A);
    data(4,8);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(3,3);
    cmd(0x2B);
    data(3,3);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(2,2);
    cmd(0x2B);
    data(4,4);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(3,7);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(8,8);
    cmd(0x2B);
    data(6,6);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(7,7);
    cmd(0x2B);
    data(7,7);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(2,6);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,4,0);

    // C
    cmd(0x2A);
    data(11,14);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,3,0);

    cmd(0x2A);
    data(10,10);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,0,4);

    cmd(0x2A);
    data(11,14);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,3,0);

    // O
    cmd(0x2A);
    data(17,20);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,3,0);

    cmd(0x2A);
    data(16,16);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,0,4);

    cmd(0x2A);
    data(17,20);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,3,0);

    cmd(0x2A);
    data(21,21);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,4,0);
    // R
    cmd(0x2A);
    data(23,23);
    cmd(0x2B);
    data(2,8);
    cmd(0x2C);
    draw(color,color,color,6,0);

    cmd(0x2A);
    data(23,26);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,3,0);

    cmd(0x2A);
    data(24,27);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,3,0);

    for(int i = 0; i < 2; i++) {
        cmd(0x2A);
        data(27+i,27+i);
        cmd(0x2B);
        data(3+i,3+i);
        cmd(0x2C);
        draw(color,color,color,0,0);
    }

    for (int i = 0; i < 3; i++) {
        cmd(0x2A);
        data(26+i,26+i);
        cmd(0x2B);
        data(6+i,6+i);
        cmd(0x2C);
        draw(color,color,color,0,0);
    }
    // E
    cmd(0x2A);
    data(30,34);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(30,34);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(30,34);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(30,30);
    cmd(0x2B);
    data(2,8);
    cmd(0x2C);
    draw(color,color,color,6,0);

    // :
    cmd(0x2A);
    data(36,36);
    cmd(0x2B);
    data(4,4);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(36,36);
    cmd(0x2B);
    data(7,7);
    cmd(0x2C);
    draw(color,color,color,0,0);
}

void one(unsigned char color, int digitCheck) {
    for (int i = 0; i < 3; i++) {
        cmd(0x2A);
        data(39+i+digitCheck,39+i+digitCheck);
        cmd(0x2B);
        data(4-i,4-i);
        cmd(0x2C);
        draw(color,color,color,0,0);
    }
    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(2,8);
    cmd(0x2C);
    draw(color,color,color,0,6);
}

void two(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,3);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,0,1);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(41+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(40+digitCheck,40+digitCheck);
    cmd(0x2B);
    data(6,6);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(7,8);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,3,0);
}

void three(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,0,2);

    cmd(0x2A);
    data(41+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,2,0);

    cmd(0x2A);
    data(41+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(6,7);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,2,0);
}

void four(unsigned char color, int digitCheck) {
    for (int i = 0; i < 3; i++) {    
        cmd(0x2A);
        data(39+i+digitCheck,39+i+digitCheck);
        cmd(0x2B);
        data(5-i,5-i);
        cmd(0x2C);
        draw(color,color,color,0,0);
    }

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(2,8);
    cmd(0x2C);
    draw(color,color,color,6,0);

    cmd(0x2A);
    data(39+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(6,6);
    cmd(0x2C);
    draw(color,color,color,3,0);
}

void five(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,3,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(4,4);
    cmd(0x2C);
    draw(color,color,color,2,0);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(5,7);
    cmd(0x2C);
    draw(color,color,color,2,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(7,7);
    cmd(0x2C);
    draw(color,color,color,0,0);
}

void six(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(3,3);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,1,0);
    
    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(6,7);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,1,0);
}

void seven(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,3,0);
    
    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(2,4);
    cmd(0x2C);
    draw(color,color,color,2,0);

    cmd(0x2A);
    data(41+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,0,0);

    cmd(0x2A);
    data(40+digitCheck,40+digitCheck);
    cmd(0x2B);
    data(6,8);
    cmd(0x2C);
    draw(color,color,color,2,0);
}

void eight(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(6,7);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(6,7);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,1,0);
}

void nine(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,4);
    cmd(0x2C);
    draw(color,color,color,1,0);  

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(5,5);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(7,7);
    cmd(0x2C);
    draw(color,color,color,0,0);
}

void zero(unsigned char color, int digitCheck) {
    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(2,2);
    cmd(0x2C);
    draw(color,color,color,1,0);

    cmd(0x2A);
    data(39+digitCheck,39+digitCheck);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(42+digitCheck,42+digitCheck);
    cmd(0x2B);
    data(3,7);
    cmd(0x2C);
    draw(color,color,color,4,0);

    cmd(0x2A);
    data(40+digitCheck,41+digitCheck);
    cmd(0x2B);
    data(8,8);
    cmd(0x2C);
    draw(color,color,color,1,0);
}

void Display_Score(int points,int digitMove) {
    if (points == 0) {
        nine(0x00,digitMove);
        zero(0xFF,digitMove);
    }
    else if (points == 1) {
        nine(0x00,digitMove);
        zero(0x00,digitMove);
        one(0xFF,digitMove);
    }
    else if (points == 2)  {     
        nine(0x00,digitMove);
        one(0x00,digitMove);
        two(0xFF,digitMove);
    }
    else if (points == 3) {
        nine(0x00,digitMove);
        two(0x00,digitMove);
        three(0xFF,digitMove);
    }
    else if (points == 4) {
        nine(0x00,digitMove);
        three(0x00,digitMove);
        four(0xFF,digitMove);
    }
    else if (points == 5) {
        nine(0x00,digitMove);
        four(0x00,digitMove);
        five(0xFF,digitMove);
    }
    else if (points == 6) {
        nine(0x00,digitMove);
        five(0x00,digitMove);
        six(0xFF,digitMove);
    }
    else if (points == 7) {
        nine(0x00,digitMove);
        six(0x00,digitMove);
        seven(0xFF,digitMove);
    }
    else if (points == 8) {
        nine(0x00,digitMove);
        seven(0x00,digitMove);
        eight(0xFF,digitMove);
    }
    else if (points == 9) {
        nine(0x00,digitMove);
        eight(0x00,digitMove);
        nine(0xFF,digitMove);
    }
}

void Clear_Entire_Score() {
    cmd(0x2A);
    data(39,52);
    cmd(0x2B);
    data(2,8);
    cmd(0x2C);
    draw(0x00,0x00,0x00,13,8);
}

#endif 