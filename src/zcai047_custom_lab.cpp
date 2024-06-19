#define A4_8 2272
#define B4_8 2024
#define E5_8 1517
#define A5_8 1136
#define F5_8 1432
#define C6_8 956
#define F6_8 716
#define G4_8 2551
#define C4_8 3816
#define D4_8 3401
#define E4_8 3030
#define G5_8 1275
#define C5_8 1912
#define F4_8 2865
#define D4Sharp 3215
#define C4Sharp 3610
#define B3_8 4048
#define A3Sharp 4292
#define A3_8 4545
#define G3Sharp 4830
#define G3_8 5102
#define F7_8 357

#include "serialATmega.h"
#include "timerISR.h"
#include "spiAVR.h"
#include "periph.h"
#include "helper.h"
#include "LCD.h"
#include "irAVR.h"
#include "zcai047_graphics.h"
#include "zcai047_startups.h"
#include "zcai047_characters.h"
#include "zcai047_score.h"

bool right = 0; 
bool left = 0;
bool reset = 0;
bool shoot = 0;
bool bullet_shot = 0;
bool aliens_coming = 0;
bool game_over = 0;
bool start = 0;
bool on = 0;
bool title = 0;
bool off = 0;
bool win = 0;
unsigned char i = 0;
unsigned char j = 0;
unsigned char k = 0;

int alienCnt;
int menu_cnt;
int start_cnt;
int congrats_cnt;
int loser_cnt;
int songCnt;
int pointCnt;
int firstPlace;
int secondPlace;
unsigned long address;
decode_results results;

int menuICR[32] = {7633,7633,7633,7633,6802,6802,6802,6802,7633,7633,7633,7633,6802,6802,6802,6802,6802,6802,7633,7633,7633,7633,6802,6802,6802,6802,6060,6060,7633,7633,5102,5102};
int menuVal[32] = {C4_8,7633,C4_8,7633,D4_8,6802,D4_8,6802,C4_8,7633,C4_8,7633,D4_8,6802,D4_8,6802,D4_8,6802,C4_8,7633,C4_8,7633,D4_8,6802,D4_8,6802,E4_8,6060,C4_8,7633,G4_8,5102};
int menuTime[32] = {2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,1,1,1,1,1};

int startUpICR[7] = {4545,4048,3034,2272,3034,2864,1912};
int startUpVal[7] = {A4_8, B4_8, E5_8, A5_8, E5_8,F5_8,C6_8};
int startUpTime[7] = {2,2,2,2,2,2,2};

int congratsICR[6] = {5102,3823,3034,2550,3034,2550};
int congratsVal[6] = {G4_8,C5_8,E5_8,G5_8,E5_8,G5_8};
int congratsTime [6] = {2,2,1,2,1,2};

int loseICR[8] = {5731,6430,7220,8096,8584,9090,9661,10203};
int loseVal[8] = {F4_8,D4Sharp,C4Sharp,B3_8,A3Sharp,A3_8,G3Sharp,G3_8}; 
int loseTime[8] = {1,1,1,1,1,1,1,1};

#define NUM_TASKS 10 //TODO: Change to the number of tasks being used

//Task struct for concurrent synchSMs implmentations
typedef struct _task{
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

//TODO: Define Periods for each task
// e.g. const unsined long TASK1_PERIOD = <PERIOD>
const unsigned long TASK1_PERIOD = 50;
const unsigned long TASK2_PERIOD = 1;
const unsigned long TASK3_PERIOD = 1;
const unsigned long TASK4_PERIOD = 1;
const unsigned long TASK5_PERIOD = 1;
const unsigned long TASK6_PERIOD = 5;
const unsigned long TASK7_PERIOD = 1;
const unsigned long TASK8_PERIOD = 1;
const unsigned long TASK9_PERIOD = 5;
const unsigned long TASK10_PERIOD = 10;
const unsigned long GCD_PERIOD = findGCD(TASK7_PERIOD,TASK9_PERIOD);//TODO:Set the GCD Period

task tasks[NUM_TASKS]; // declared task array with 5 tasks

// needed to change this to allow the remote to work better
void TimerISR() {
	// for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
	// 	if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
	// 		tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
	// 		tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
	// 	}
	// 	tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
	// }
    TimerFlag = 0;
}

// added to Github
enum REMOTE_STATES {REMOTE_INIT, WAIT_REMOTE, DISPLAY_TITLE, OFF, RESTART};
int TickFct_REMOTE(int state) {
    switch(state) {
        case REMOTE_INIT:
            state = WAIT_REMOTE;
            break;
        case WAIT_REMOTE:
            if (IRdecode(&results)) {
                address = results.value;
                if (address == 16712445) {
                    IRresume();
                    state = DISPLAY_TITLE;
                }
                else if (address == 16753245) {
                    IRresume();
                    state = OFF;
                }
                else {
                    IRresume();
                    state = WAIT_REMOTE;
                }
            }
            break;
        case DISPLAY_TITLE:
            title = 1;
            off = 0;
            state = WAIT_REMOTE;
            break;
        case OFF:
            off = 1;
            title = 0;
            state = WAIT_REMOTE;
            break;
        default:
            break;
    }
    switch(state) {
        case WAIT_REMOTE:
            break;
                case OFF:
            break;
        case DISPLAY_TITLE:
            break;
        default:
            break;
    }
    return state;
}

enum JOYSTICK_STATES {JOYSTICK_INIT, MAKE_MOVE, RIGHT, LEFT};
int TickFct_JOYSTICK(int state) {
    switch(state) {
        case JOYSTICK_INIT:
            state = MAKE_MOVE;
            break;
        case MAKE_MOVE:
            if (ADC_read(0) > 700) {
                right = 1;
                state = RIGHT;
            }
            else if (ADC_read(0) < 500) {
                left = 1;
                state = LEFT;
            }
            break;
        case RIGHT:
            if (ADC_read(0) > 500 && ADC_read(0) < 700) {
                right = 0;
                state = MAKE_MOVE;
            }
            break;
        case LEFT:
            if ((ADC_read(0) > 500 && ADC_read(0) < 700)) {
                left = 0;
                state = MAKE_MOVE;
            }
            break;
        default:
            state = JOYSTICK_INIT;
            break;
    }
    switch(state) {
        default:
            break;
    }
    return state;
}

enum SHOOT_STATES {SHOOT_INIT, SHOOT_PRESS, SHOOT_RELEASE};
int TickFct_SHOOT(int state) {
    switch(state) {
        case SHOOT_INIT:
            state = SHOOT_RELEASE;
            break;
        case SHOOT_RELEASE:
            state = GetBit(PINC,2) ? SHOOT_PRESS : SHOOT_RELEASE;
            break;
        case SHOOT_PRESS:
            state = !GetBit(PINC,2) ? SHOOT_RELEASE : SHOOT_PRESS;
            break;
        default:
            state = SHOOT_INIT;
            break;
    }
    switch(state) {
        case SHOOT_RELEASE:
            shoot = 0;
            break;
        case SHOOT_PRESS:
            shoot = 1;
            break;
        default:
            break;
    }
    return state;
}

enum START_STATES {START_INIT, START_RELEASE, ON_MUSIC, ON_NOW, START_PRESS};
int TickFct_START(int state) {
    switch(state) {
        case START_INIT:
            state = START_RELEASE;
            break;
        case START_RELEASE:
            state = GetBit(PINC,3) ? ON_MUSIC : START_RELEASE;
            break;
        case ON_MUSIC:
            state = !GetBit(PINC,3) ? ON_NOW : ON_MUSIC;
            break;
        case ON_NOW:
            state = GetBit(PINC,3) ? START_PRESS : ON_NOW;
            break;
        case START_PRESS:
            state = !GetBit(PINC,3) ? START_RELEASE : START_PRESS;
            break;
        default:
            state = START_INIT;
            break;
    }
    switch(state) {
        case START_RELEASE:
            on = 0;
            start = 0;
            break;
        case ON_MUSIC:
            on = 1;
            start = 0;
            break;
        case START_PRESS:
            on = 0;
            start = 1;
            break;
        default:
            break;
    }
    return state;
}

enum RESET_STATES {RESET_INIT, RESET_RELEASE, RESET_PRESS};
int TickFct_RESET(int state) {
    switch(state) {
        case RESET_INIT:
            state = RESET_RELEASE;
            break;
        case RESET_RELEASE:
            state = !((PINC >> 1) & 0x01) ? RESET_PRESS : RESET_RELEASE;
            break;
        case RESET_PRESS:
            state = ((PINC >> 1) & 0x01) ? RESET_RELEASE : RESET_PRESS;
            break;
        default:
            state = RESET_INIT;
            break;
    }
    switch(state) {
        case RESET_RELEASE:
            reset = 0;
            break;
        case RESET_PRESS:
            reset = 1;
            break;
        default:
            break;
    }
    return state;
}

enum MUSIC_STATES {MUSIC_INIT, WAIT, STARTUP, CONGRATS, LOSER, SHOOT, MENU};
int TickFct_MUSIC(int state) {
    switch(state) {
        case MUSIC_INIT:
            state = WAIT;
            break;
        case WAIT:  
            if (start) {
                songCnt = 0;
                start_cnt = 0;
                state = STARTUP;
            }
            else if (alienPosY[7]==130) { 
                songCnt = 0;
                loser_cnt = 0;
                state = LOSER;
            }
            else if (on) {
                songCnt = 0;
                menu_cnt = 0;
                state = MENU;
            }
            else if (win) {
                songCnt = 0;
                congrats_cnt = 0;
                state = CONGRATS;
            }
            else if (shoot) {
                songCnt = 0;
                state = SHOOT;
            }
            else {
                state = WAIT;
            }
            break;
        case MENU:
            if (on) {
                if (songCnt <= menuTime[menu_cnt]) {
                    state = MENU;
                }
                else if (songCnt > menuTime[menu_cnt]) {
                    if (menu_cnt <= 31) {
                        menu_cnt++;
                        songCnt = 0;
                        state = MENU;
                    }
                    else {
                        state = WAIT; // allows it to cycle back if wanting to play continously 
                    }
                }
            }
            else {
                state = WAIT;
            }
            break;
        case STARTUP:
            if (songCnt <= startUpTime[start_cnt]) {
                state = STARTUP;
            }
            else if (songCnt > startUpTime[start_cnt]) {
                if (start_cnt <= 6) {
                    start_cnt++;
                    songCnt = 0;
                    state = STARTUP;
                }
                else {
                    state = WAIT;
                }        
            }    
            break;
        case CONGRATS:
            if (songCnt <= congratsTime[congrats_cnt]) {
                state = CONGRATS;
            }
            else if (songCnt > congratsTime[congrats_cnt]) {
                if (congrats_cnt <= 5) {
                    congrats_cnt++;
                    songCnt = 0;
                    state = CONGRATS;
                }
                else {
                    win = 0;
                    state = WAIT; // allows it to cycle back if wanting to play continously 
                }
            }
            break;
        case LOSER:
            if (songCnt <= loseTime[loser_cnt]) {
                state = LOSER;
            }
            else if (songCnt > loseTime[loser_cnt]) {
                if (loser_cnt <= 7) {
                    loser_cnt++;
                    songCnt = 0;
                    state = LOSER;
                }
                else {
                    state = WAIT; // allows it to cycle back if wanting to play continously
                }
            } 
            break;
        case SHOOT:
            if (songCnt <= 2) {
                state = SHOOT;
            }
            else if (songCnt > 2) {
                state = WAIT;
            }
            break;
        default:
            state = MUSIC_INIT;
            break;
    }
    switch(state) {
        case WAIT:
            OCR1A = ICR1;
            break;
        case MENU:
            songCnt++;
            ICR1 = menuICR[menu_cnt];
            OCR1A = menuVal[menu_cnt];
            break;
        case STARTUP:
            songCnt++;
            ICR1 = startUpICR[start_cnt];
            OCR1A = startUpVal[start_cnt];
            break;
        case CONGRATS:
            songCnt++;
            ICR1 = congratsICR[congrats_cnt];
            OCR1A = congratsVal[congrats_cnt];
            break;
        case LOSER:
            songCnt++;
            ICR1 = loseICR[loser_cnt];
            OCR1A = loseVal[loser_cnt];
            break;
        case SHOOT:
            songCnt++;
            ICR1 = F7_8 * 2;
            OCR1A = F7_8;
            break;
        default:
            break;
    }
    return state;
}

enum MOVE_PLAYER_STATES {MOVE_INIT,WAIT_MOVE, MOVE_LEFT, MOVE_RIGHT};
int TickFct_MOVE(int state) {
    switch(state) {
        case MOVE_INIT:
            state = WAIT_MOVE;
            break;
        case WAIT_MOVE:
            if (left && coor[2] > 5) {
                Clear_Player();
                state = MOVE_LEFT;
            }
            else if (right && coor[5] < 125) {
                Clear_Player();
                state = MOVE_RIGHT;
            }
            break;
        case MOVE_LEFT:
            if (left && coor[2] > 5) {
                Clear_Player();
                state = MOVE_LEFT;
            }
            else if (coor[2] == 5) {
                state = WAIT_MOVE;
            }
            else {
                state = WAIT_MOVE;
            }
            break;
        case MOVE_RIGHT:
            if (right && coor[5] < 125) {
                Clear_Player();
                state = MOVE_RIGHT;
            }
            else if (coor[5] == 125) {
                state = WAIT_MOVE;
            }
            else {
                state = WAIT_MOVE;
            }
            break;
        default:
            state = WAIT_MOVE;
            break;
    }
    switch(state) {
        case WAIT_MOVE:
            break;
        case MOVE_LEFT:
            Move_Player_Left();
            break;
        case MOVE_RIGHT:
            Move_Player_Right();
            break;
        default:
            break;
    }
    return state;
}

enum SHOOT_BULLET_STATES {SHOOT_BULLET_INIT, WAIT_PRESS, PRINT_BULLET};
int TickFct_SHOOT_BULLET(int state) {
    switch(state) {
        case SHOOT_BULLET_INIT:
            state = WAIT_PRESS;
            break;
        case WAIT_PRESS:
            if (shoot) {
                j = 0;
                bulletPos[0] = newPos;
                state = PRINT_BULLET;
            }
            break;
        case PRINT_BULLET:
            if (j <= 100) {
                if (aliens_coming && ALIEN_HIT(alienCnt)) {
                    Clear_Bullet();
                    j = 0;
                    state = WAIT_PRESS;
                }
                else {
                    Clear_Bullet();
                    Move_Bullet();
                    state = PRINT_BULLET;
                }  
            }
            else if (j > 100) {
                Clear_Bullet();
                state = WAIT_PRESS;
            }
            break;
        default:
            state = SHOOT_BULLET_INIT;
            break;
    }
    switch(state) {
        case WAIT_PRESS:
            bullet_shot = 0;
            Reset_Bullet();
            break;
        case PRINT_BULLET:
            j++;
            bullet_shot = 1;
            Print_Bullet();
            break;
        default:
            break;
    }
    return state;
}

enum GAME_STATES {GAME_INIT,GAME_OFF,BUTTON_WAIT,SHOW_TITLE,START_GAME,SHOW_GAME,PLAY_GAME,CHECK,RESTART_GAME,CLEAR_GAME};
int TickFct_GAME(int state) {
    switch(state) {
        case GAME_INIT:
            state = BUTTON_WAIT;
            break;
        case BUTTON_WAIT:
            if (off) {
                state = GAME_OFF;
            }
            else if (title) {
                state = SHOW_TITLE;
            }
            break;
        case GAME_OFF:
            state = BUTTON_WAIT;
            break;
        case SHOW_TITLE:
            if (on) {
                state = SHOW_GAME;
            }
            else if (off) {
                state = GAME_OFF;
            }
            break;
        case SHOW_GAME:
            if (start) {
                state = START_GAME;
            }
            else if (reset) {
                state = RESTART_GAME;
            }
            else if (off) {
                state = GAME_OFF;
            }
            break;
        case START_GAME:     
            k = 0;
            alienCnt = 0;
            pointCnt = 0;
            Display_Score(pointCnt,0);
            state = PLAY_GAME;
            break;
        case RESTART_GAME:
            k = 0;
            alienCnt = 0;
            pointCnt = 0;
            Clear_Entire_Score();
            Display_Score(pointCnt,0);
            state = PLAY_GAME;
            break;
        case PLAY_GAME:
            if (off) {
                state = GAME_OFF;
            }
            else if (pointCnt == 10) {
                win = 1;
                Clear_ENEMY(alienCnt);
                state = CLEAR_GAME;
            }
            else if (k <= 110 && !reset) { 
                if (bullet_shot && ALIEN_HIT(alienCnt)) {
                    Clear_ENEMY(alienCnt);
                    state = CHECK;
                }
                else {   
                    Clear_ENEMY(alienCnt);
                    state = PLAY_GAME;
                }
            }
            else if (k > 110 && !reset) {
                Clear_ENEMY(alienCnt);
                game_over = 1;
                state = CLEAR_GAME;
            }
            else if (reset) {
                state = RESTART_GAME;
            }
            break;
        case CHECK:
            if (k <= 110 && alienPosY[7] != 130) {
                if (alienCnt <= 4) {
                    Clear_ENEMY(alienCnt);
                    Reset_Alien();
                    k = 0;
                    alienCnt++;
                    pointCnt++;
                    state = PLAY_GAME;
                }
                else {
                    Clear_ENEMY(alienCnt);
                    Reset_Alien();
                    k = 0;
                    alienCnt = 0;
                    pointCnt++;
                    state = PLAY_GAME;
                }
            }
            break;
        case CLEAR_GAME:
            state = SHOW_GAME;
            break;
        default: 
            state = GAME_INIT;
            break;
    }
    switch(state) {
        case BUTTON_WAIT:
            left = 0;
            right = 0;
            Clear_Player();
            break;
        case GAME_OFF:
            left = 0;
            right = 0;
            pointCnt = 0;
            game_over = 0;
            win = 0;
            aliens_coming = 0;
            Make_Black_Screen();
            Reset_Alien();
            Reset_Bullet();
            break;
        case SHOW_TITLE:
            left = 0;
            right = 0;
            Clear_Player();
            break;
        case SHOW_GAME:
            left = 0;
            right = 0;
            pointCnt = 0;
            aliens_coming = 0;
            Draw_PLAYER();
            Write_Score(0xFF);
            Clear_Entire_Score();
            break;
        case START_GAME:
            game_over = 0;
            aliens_coming = 0;
            pointCnt = 0;
            win = 0;
            Draw_PLAYER();
            Write_Score(0xFF);
            Display_Score(pointCnt,0);
            Clear_Entire_Score();
            break;
        case RESTART_GAME:
            game_over = 0;
            pointCnt = 0;
            aliens_coming = 0;
            win = 0;
            Clear_Player();
            Clear_ENEMY(alienCnt);
            Reset_Alien();
            Clear_Bullet();
            Reset_Bullet();
            Clear_Entire_Score();
            break;
        case PLAY_GAME:
            k++;
            win = 0;
            game_over = 0;
            aliens_coming = 1;
            Move_Alien();
            Draw_ENEMY(alienCnt);
            firstPlace = (pointCnt % 100) % 10; 
            secondPlace = (pointCnt / 10) % 10; 
            if (firstPlace > 0 && secondPlace == 0) {
                Display_Score(firstPlace,0);
            }
            else if (secondPlace > 0) {
                Display_Score(firstPlace,5);
                Display_Score(secondPlace,0);
            } 
            break;
        case CLEAR_GAME:
            Clear_Player();
            Clear_ENEMY(alienCnt);  
            Reset_Alien(); 
            Clear_Bullet();
            Reset_Bullet();
            break;
        default:
            break;
    }
    return state;
}

enum LCD_STATES {LCD_INIT, WAIT_ON,LCD_GAME_NAME,YOU_LOSE,YOU_WIN,LCD_OFF};
int TickFct_LCD(int state) {
    switch(state) {
        case LCD_INIT:
            state = WAIT_ON;
            break;
        case WAIT_ON:
            if (title) {
                lcd_clear();
                state = LCD_GAME_NAME;
            }
            else if (off) {
                lcd_clear();
                state = LCD_OFF;
            }
            break;
        case LCD_GAME_NAME:
            if (game_over) {
                lcd_clear();
                state = YOU_LOSE;
            }
            else if (win) {
                lcd_clear();
                state = YOU_WIN;
            }
            else if (off) {
                lcd_clear();
                state = LCD_OFF;
            }
            break;
        case YOU_LOSE:
            if (off) {
                lcd_clear();
                state = LCD_OFF;
            }
            else if (reset) {
                lcd_clear();
                state = LCD_GAME_NAME;
            }
            break;
        case YOU_WIN:
            if (off) {
                lcd_clear();
                state = LCD_OFF;
            }
            else if (reset) {
                lcd_clear();
                state = LCD_GAME_NAME;
            }
            break;
        case LCD_OFF:
            state = WAIT_ON;
            break;
        default:
            state = LCD_INIT;
            break;
    }
    switch(state) {
        case LCD_GAME_NAME:
            lcd_goto_xy(0,0);
            lcd_write_str("SPACE WAR");
            break;
        case YOU_LOSE:
            lcd_goto_xy(0,0);
            lcd_write_str("You Suck!!");
            break;
        case YOU_WIN:
            lcd_goto_xy(0,0);
            lcd_write_str("Good Job!!");
            break;
        default:
            break;
    }
    return state;
}

int main(void) {
    //TODO: initialize all your inputs and ouputs
    unsigned char i = 0;
    serial_init(9600);
    ADC_init();   // initializes ADC
    lcd_init();
    IRinit(&PORTC, &PINC, 4);
    
    DDRC = 0xE0; PORTC = 0x1F;
    DDRB = 0xFF; PORTB = 0x00; 
    DDRD = 0xFF; PORTD = 0x00;

    //TODO: Initialize the buzzer timer/pwm(timer1)
    TCCR1A |= (1 << WGM11) | (1 << COM1A1); //COM1A1 sets it to channel A
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);  // sets prescalar to 8

    SPI_INIT();
    ST7735_INIT();
    Make_Black_Screen();

    //TODO: Initialize tasks here
    // e.g. 
    // tasks[0].period = ;
    // tasks[0].state = ;
    // tasks[0].elapsedTime = ;
    // tasks[0].TickFct = ;
    tasks[i].period = TASK1_PERIOD;
    tasks[i].state = REMOTE_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_REMOTE;
    i++;
    tasks[i].period = TASK2_PERIOD;
    tasks[i].state = JOYSTICK_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_JOYSTICK;
    i++;
    tasks[i].period = TASK3_PERIOD;
    tasks[i].state = SHOOT_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_SHOOT;
    i++;
    tasks[i].period = TASK4_PERIOD;
    tasks[i].state = START_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_START;
    i++;
    tasks[i].period = TASK5_PERIOD;
    tasks[i].state = RESET_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_RESET;
    i++;
    tasks[i].period = TASK6_PERIOD;
    tasks[i].state = MUSIC_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_MUSIC;
    i++;
    tasks[i].period = TASK7_PERIOD;
    tasks[i].state = MOVE_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_MOVE;
    i++;
    tasks[i].period = TASK8_PERIOD;
    tasks[i].state = SHOOT_BULLET_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_SHOOT_BULLET;
    i++;
    tasks[i].period = TASK9_PERIOD;
    tasks[i].state = GAME_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_GAME;
    i++;
    tasks[i].period = TASK10_PERIOD;
    tasks[i].state = LCD_INIT;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_LCD;

    TimerSet(GCD_PERIOD);
    TimerOn();

    // need to change this for the remote to work 
    while (1) {
        for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
            if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
                tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
                tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
            }
            tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
            while(TimerFlag) {}
            TimerFlag = 1;
        }
    }

    return 0;
}