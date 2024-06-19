#ifndef ZCAI047_CHARACTERS_H
#define ZCAI047_CHARACTERS_H

#include "spiAVR.h"
#include "helper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
#include "zcai047_graphics.h"

int coor[10] = {60,70,56,59,71,74,62,68,64,66};
int bulletPos[3] = {65,108,110};
int newPos = 65;
int alienPosX[6] = {40,0,80,20,60,100};
int alienPosX2[6] = {0,80,20,60,100,40};
int alienPosY[8] = {12,17,14,16,14,16,18,20};
int cnt;
bool hit = 0;

void Subtract_Left() {
    for(int i = 0; i < 10; i++) {
        coor[i] = coor[i] - 1;
    }
}

void ADD_Right() {
    for(int i = 0; i < 10; i++) {
        coor[i] = coor[i] + 1;
    }
}

void Shoot_Left() {
    newPos-=1;
}

void Shoot_Right() {
    newPos+=1;
}

void Move_Bullet() {
    bulletPos[1]-=1;
    bulletPos[2]-=1;
}

void Move_Bullet_Right() {
    bulletPos[0]+=1;
}

void Move_Bullet_Left() {
    bulletPos[0]-=1;
}

void Reset_Bullet() {
    bulletPos[1] = 108;
    bulletPos[2] = 110;
}

void Move_Alien() {
    for (int i = 0; i < 8; i++) {
        alienPosY[i] = alienPosY[i] + 1;
    }
}

void Reset_Alien() {
    alienPosY[0] = 12;
    alienPosY[1] = 17;
    alienPosY[2] = 14;
    alienPosY[3] = 16;
    alienPosY[4] = 14;
    alienPosY[5] = 16;
    alienPosY[6] = 18;
    alienPosY[7] = 20;
}

void Reset_Player() {
    coor[0] = 60;
    coor[1] = 70;
    coor[2] = 56;
    coor[3] = 59;
    coor[4] = 71;
    coor[5] = 74;
    coor[6] = 62;
    coor[7] = 68;
    coor[8] = 64;
    coor[9] = 66;
}

bool ALIEN_HIT(int cnt) {
    if ((bulletPos[1] == alienPosY[7]) && (bulletPos[0] >= 6+alienPosX[cnt] && bulletPos[0] <= 24+alienPosX[cnt])) {
        return hit = 1;
    }
    else {
        return hit = 0;
    }
}
 
void Draw_ENEMY(int cnt) {
    // head
    cmd(0x2A);
    data(10+alienPosX[cnt],20+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[0],alienPosY[1]);
    cmd(0x2C);
    draw(0x00,0xFF,0x00,5,10);
    // eyes
    cmd(0x2A);
    data(12+alienPosX[cnt],14+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[2],alienPosY[3]);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,2,2);
    cmd(0x2A);
    data(16+alienPosX[cnt],18+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[4],alienPosY[5]);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,4,5);
    // bottom
    cmd(0x2A);
    data(6+alienPosX[cnt],24+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[6],alienPosY[7]);
    cmd(0x2C);
    draw(0x00,0xFF,0x00,18,2);
}

void Clear_ENEMY(int cnt) {
    // head
    cmd(0x2A);
    data(10+alienPosX[cnt],20+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[0],alienPosY[1]);
    cmd(0x2C);
    draw(0x00,0x00,0x00,5,10);
    // eyes
    cmd(0x2A);
    data(12+alienPosX[cnt],14+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[2],alienPosY[3]);
    cmd(0x2C);
    draw(0x00,0x00,0x00,2,2);
    cmd(0x2A);
    data(16+alienPosX[cnt],18+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[4],alienPosY[5]);
    cmd(0x2C);
    draw(0x00,0x00,0x00,4,5);
    // bottom
    cmd(0x2A);
    data(6+alienPosX[cnt],24+alienPosX[cnt]);
    cmd(0x2B);
    data(alienPosY[6],alienPosY[7]);
    cmd(0x2C);
    draw(0x00,0x00,0x00,18,2);
}

void Draw_PLAYER() {
    // body of spaceship
    cmd(0x2A);
    data(coor[0],coor[1]);
    cmd(0x2B);
    data(120,125);
    cmd(0x2C);
    draw(0xFF,0xFF,0xFF,10,5);
    // left wing
    cmd(0x2A);
    data(coor[2],coor[3]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,3,5);
    // right wing
    cmd(0x2A);
    data(coor[4],coor[5]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,5,3);
    // head 
    cmd(0x2A);
    data(coor[6],coor[7]);
    cmd(0x2B);
    data(117,119);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,6,3);
    // shooter
    cmd(0x2A);
    data(coor[8],coor[9]);
    cmd(0x2B);
    data(111,117);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,5,2); 
}

void Move_Player_Left() {
    Subtract_Left();
    Shoot_Left();
    // body of spaceship
    // body of spaceship
    cmd(0x2A);
    data(coor[0],coor[1]);
    cmd(0x2B);
    data(120,125);
    cmd(0x2C);
    draw(0xFF,0xFF,0xFF,10,5);
    // left wing
    cmd(0x2A);
    data(coor[2],coor[3]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,3,5);
    // right wing
    cmd(0x2A);
    data(coor[4],coor[5]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,5,3);
    // head 
    cmd(0x2A);
    data(coor[6],coor[7]);
    cmd(0x2B);
    data(117,119);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,6,3);
    // shooter
    cmd(0x2A);
    data(coor[8],coor[9]);
    cmd(0x2B);
    data(111,117);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,5,2); 
}

void Clear_Player() {
    // body of spaceship
    // body of spaceship
    cmd(0x2A);
    data(coor[0],coor[1]);
    cmd(0x2B);
    data(120,125);
    cmd(0x2C);
    draw(0x00,0x00,0x00,10,5);
    // left wing
    cmd(0x2A);
    data(coor[2],coor[3]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x00,0x00,0x00,3,5);
    // right wing
    cmd(0x2A);
    data(coor[4],coor[5]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x00,0x00,0x00,5,3);
    // head 
    cmd(0x2A);
    data(coor[6],coor[7]);
    cmd(0x2B);
    data(117,119);
    cmd(0x2C);
    draw(0x00,0x00,0x00,6,3);
    // shooter
    cmd(0x2A);
    data(coor[8],coor[9]);
    cmd(0x2B);
    data(111,117);
    cmd(0x2C);
    draw(0x00,0x00,0x00,5,2); 
}

void Move_Player_Right() {
    ADD_Right();
    Shoot_Right();
    // body of spaceship
    cmd(0x2A);
    data(coor[0],coor[1]);
    cmd(0x2B);
    data(120,125);
    cmd(0x2C);
    draw(0xFF,0xFF,0xFF,10,5);
    // left wing
    cmd(0x2A);
    data(coor[2],coor[3]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,3,5);
    // right wing
    cmd(0x2A);
    data(coor[4],coor[5]);
    cmd(0x2B);
    data(123,128);
    cmd(0x2C);
    draw(0x80,0x80,0x80,5,3);
    // head 
    cmd(0x2A);
    data(coor[6],coor[7]);
    cmd(0x2B);
    data(117,119);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,6,3);
    // shooter
    cmd(0x2A);
    data(coor[8],coor[9]);
    cmd(0x2B);
    data(111,117);
    cmd(0x2C);
    draw(0x00,0x00,0xFF,5,2);  
}

void Print_Bullet() {
    // moves to the left or right by incrementing or decrementing CASET
    cmd(0x2A);
    data(bulletPos[0],bulletPos[0]);
    cmd(0x2B);
    data(bulletPos[1],bulletPos[2]);
    cmd(0x2C);
    draw(0xFF,0xFF,0xFF,0,2);
}

void Clear_Bullet() {
    cmd(0x2A);
    data(bulletPos[0],bulletPos[0]);
    cmd(0x2B);
    data(bulletPos[1],bulletPos[2]);
    cmd(0x2C);
    draw(0x00,0x00,0x00,0,2);
}

#endif