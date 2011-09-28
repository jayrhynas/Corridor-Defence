/* 
 * File:    list.h
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Declares various X init and system functions
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "enemies.h"
#include "defenses.h"
#include "list.h"
#include "info.h"

/* Key symbols */
#define XK_space 0x0020
#define XK_d     0x0064
#define XK_m     0x006d
#define XK_n     0x006e
#define XK_p     0x0070
#define XK_q     0x0071
#define XK_s     0x0073
            
/* Window Size */
#define WIN_W 1000
#define WIN_H 520

/* X Vars */
extern Display* display;
extern Window window;
extern Pixmap buffer;

/* GC Styles */
extern GC gc;  // default (black on white)
extern GC floorGC;
extern GC enemyGC;
extern GC lineGC;

/* System Vars */
extern long long old_time;
extern int mouseX, mouseY;

/* Game Vars */
#define NUM_FLOORS 3;
#define NUM_COLS 8;
extern int money, points;
extern Castle castle;
extern List *eList;
extern List *nextWave;
extern List *bList;

extern Enemy *thumbEnemies[NUM_ENEMY_TYPES];

extern Defense *turretArray[3][8];
extern Defense *trapArray[3][8];

extern long long lastWaveTime;
extern long long nextWaveTime;

extern Defense *selection;

/* Color vars */
extern Colormap screen_colormap;
extern XColor red, brown, blue, yellow, green, grey, dark_grey, grey_10, grey_25, grey_50, brick_red, black, white;

void initVars();
void colorInit();
void windowInit();
void createGraphicsContext();
void registerForEvents();
void splashScreen();
void eventLoop();
void gameOver();
void redrawScreen();
void handleEvent();
void programAnimation();
void quit();

void togglePause();

long long getTime();

void drawSplash();
void drawStage();
void drawCovers();
void drawUI();
void drawInfo(int lookup);

void deleteSelection();

void clickUI(int x, int y);

typedef enum {
    SHOP,
    TRAP,
    TURRET,
    CASTLE,
    NUM_GROUPS
} ButtonGroup;

typedef struct {
    Node *container;
    int id;
    int x;
    int y;
    int w;
    int h;
    int pushed;
    int visible;
    char *label;
    void *item;
    void (*callback)();
    ButtonGroup groupNum;
} Button;

Button* createButton(int x, int y, int w, int h, char* label, void* item, int visible, int id, ButtonGroup groupNum, void(*callback)());
void drawButton(void *vb);
void freeButtonPointers(void *vb);
void makeVisible(void *vb);
void makeInvisible(void *vb);

typedef enum {
    LEFT,
    CENTER,
    RIGHT,
    TOP,
    BOTTOM
} Alignment;

void printFonts();
void drawText(char* string, int x, int y, int font_num, Alignment hAlign, Alignment vAlign);
void drawMultilineText(char* strings[], int nlines, int line_height, int x, int y, int font_num, Alignment hAlign, Alignment vAlign);
char* intToStr(int num);

void rotatePoints(XPoint *points, int npoints, int rx, int ry, float a);
void flipPoints(XPoint *points, int npoints, int fx, int fy, int flipHorizontal, int flipVertical);

#endif /* _MAIN_H_ */