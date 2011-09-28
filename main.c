/* 
 * File:    main.c
 * Author:  Jayson Rhynas, 20299211
 * Created: January 13, 2011
 *
 * A simple tower defense game.
 *
 * Some code from lecture material by Edward Lank
 */

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "enemies.h"
#include "defenses.h"
#include "list.h"

/* X Vars */
Display* display;
Window window;
Pixmap buffer;

/* GC Styles */
GC gc;  // default (black on white)
GC floorGC;
GC enemyGC;
GC lineGC;  // assumed to always be 1px black - if changed, must change back

/* System Vars */
long long old_time;
int mouseX, mouseY;
XFontStruct* font_info[3];
int showSplash;

/* Game Vars */
int money, points;
Castle castle;
List *eList;
List *nextWave;
List *bList;
Defense *turretArray[3][8];
Defense *trapArray[3][8];

long long nextWaveTime;

Enemy *thumbEnemies[NUM_ENEMY_TYPES];

List *buttonList;                   // all buttons
List *systemButtonList;
List *shopButtonList;
List *turretButtonList;
List *trapButtonList;
List *castleButtonList;
Button *pushedButtons[NUM_GROUPS];  // useed to simulate radio buttons
Button *pauseButton;                // used to check if game is paused
Button *speedButton;
Button *deleteButton;
Button *nextWaveButton;

Defense *selection = NULL;

/* Color vars */
Colormap screen_colormap;
XColor red, brown, blue, yellow, green, grey, dark_grey, grey_10, grey_25, grey_50, brick_red, black, white;
  
int main(){
    printFonts();
    windowInit();
    createGraphicsContext();
    registerForEvents();
    initVars();
    splashScreen();
    eventLoop();
    gameOver();
}

/* System Initialization */
void initVars() {
    int i, j, id;
    Enemy *e;
    Defense *d;
    Castle *c;
    Button *b;
    
    srandom(getpid());
    
    showSplash = 1;
    
    old_time = getTime();
    
    money = 1000;
    points = 0;
    
    castle.type = WOOD;
    castle.curHealth = 1000;
    castle.maxHealth = 1000;
    castle.showHealth = 1;
    
    for (i = 0; i < NUM_ENEMY_TYPES; i++) {
        thumbEnemies[i] = createEnemy(i, 0, i);
    }
    
    bList = listCreate(BULLET);
    
    eList = listCreate(ENEMY);
    
    createRandomWave();
    
    for (i = 0; i < NUM_GROUPS; i++) {
        pushedButtons[i] = NULL;
    }
    
    buttonList = listCreate(BUTTON);
    
    systemButtonList = listCreate(BUTTON);
    b = createButton(WIN_W - 30, 380, 20, 20, "X", NULL, 1, 0, -1, &quit);
    listAppend(systemButtonList, b);
    listAppend(buttonList, b);
    
    b = createButton(WIN_W - 60, 380, 20, 20, "||", NULL, 1, 1, -1, NULL);
    pauseButton = b;
    listAppend(systemButtonList, b);
    listAppend(buttonList, b);
    
    b = createButton(WIN_W - 90, 380, 20, 20, ">", NULL, 1, 2, -1, NULL);
    speedButton = b;
    listAppend(systemButtonList, b);
    listAppend(buttonList, b);
    
    b = createButton(390, 380, 100, 20, "Delete", NULL, 1, 3, -1, deleteSelection);
    deleteButton = b;
    listAppend(systemButtonList, b);
    listAppend(buttonList, b);
    
    b = createButton(510, 380, 100, 20, "Send Next Wave", NULL, 1, 4, -1, sendAndCreateWave);
    nextWaveButton = b;
    listAppend(systemButtonList, b);
    listAppend(buttonList, b);

    shopButtonList = listCreate(BUTTON);
    b = createButton(10, 410, 100, 30, "Turrets", NULL, 1, TURRET, SHOP, NULL);
    b->pushed = 1;
    pushedButtons[SHOP] = b;
    listAppend(shopButtonList, b);
    listAppend(buttonList, b);
    b = createButton(10, 445, 100, 30, "Traps", NULL, 1, TRAP, SHOP, NULL);
    listAppend(shopButtonList, b);
    listAppend(buttonList, b);
    b = createButton(10, 480, 100, 30, "Door", NULL, 1, CASTLE, SHOP, NULL);
    listAppend(shopButtonList, b);
    listAppend(buttonList, b);
    
    trapButtonList = listCreate(BUTTON);
    for (i = 0; i < NUM_TRAPS; i++) {
        d = createDefense(i, -1, i, i);
        b = createButton(i*110 + 120, 410, 100, 100, NULL, d, 0, i, TRAP, NULL);
        listAppend(trapButtonList, b);
        listAppend(buttonList, b);
    }
    turretButtonList = listCreate(BUTTON);
    for (i = 0; i < NUM_TURRETS_PLUS_GUN - GUN; i++) {
        d = createDefense(i + GUN, -1, i, i);
        b = createButton(i*110 + 120, 410, 100, 100, NULL, d, 0, i, TURRET, NULL);
        listAppend(turretButtonList, b);
        listAppend(buttonList, b);
    }
    
    castleButtonList = listCreate(BUTTON);
    for (i = 0; i < NUM_CASTLE; i++) {
        c = (Castle*)malloc(sizeof(Castle));
        c->type = i;
        c->maxHealth = c->curHealth = 1;
        c->showHealth = 0;
        b = createButton(i*110 + 120, 410, 100, 100, NULL, c, 0, i, CASTLE, NULL);
        listAppend(castleButtonList, b);
        listAppend(buttonList, b);
    }
}

/* X Init Functions */

void colorInit() {

  screen_colormap = DefaultColormap(display, DefaultScreen(display));
  int rc;
  rc = XAllocNamedColor(display, screen_colormap, "red", &red, &red);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'red' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "brown", &brown, &brown);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'brown' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "blue", &blue, &blue);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'blue' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "yellow", &yellow, &yellow);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'yellow' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "green", &green, &green);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'green' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "grey", &grey, &grey);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'grey' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "black", &black, &black);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'black' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "white", &white, &white);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'white' color.\n");
    exit(1);
  }
  rc = XAllocNamedColor(display, screen_colormap, "dark grey", &dark_grey, &dark_grey);
  if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - failed to allocated 'dark_grey' color.\n");
    exit(1);
  }
  grey_10.red = 6553;
  grey_10.green = 6553;
  grey_10.blue = 6553;
  rc = XAllocColor(display, screen_colormap, &grey_10);
  if (rc == 0) {
    fprintf(stderr, "XAllocColor - failed to allocated 'grey_10' (6553, 6553, 6553) color.\n");
    exit(1);
  }
  grey_25.red = 16384;
  grey_25.green = 16384;
  grey_25.blue = 16384;
  rc = XAllocColor(display, screen_colormap, &grey_25);
  if (rc == 0) {
    fprintf(stderr, "XAllocColor - failed to allocated 'grey_25' (16384, 16384, 16384) color.\n");
    exit(1);
  }
  grey_50.red = 32768;
  grey_50.green = 32768;
  grey_50.blue = 32768;
  rc = XAllocColor(display, screen_colormap, &grey_50);
  if (rc == 0) {
    fprintf(stderr, "XAllocColor - failed to allocated 'grey_50' (32768, 32768, 32768) color.\n");
    exit(1);
  }
  brick_red.red = 38293;
  brick_red.green = 0;
  brick_red.blue = 5140;
  rc = XAllocColor(display, screen_colormap, &brick_red);
  if (rc == 0) {
    fprintf(stderr, "XAllocColor - failed to allocated 'brick_red' (38293, 0, 5140) color.\n");
    exit(1);
  }
}

void windowInit(){
    int depth;
    display = XOpenDisplay(":0");
    if (display == NULL) {
        printf("Cannot connect\n");
        exit (-1);
    }
    colorInit();
    int screennum = DefaultScreen(display);
    long background = grey.pixel; //WhitePixel(display, screennum);
    long foreground = BlackPixel(display, screennum); 
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 10, 10, WIN_W, WIN_H, 2, foreground, background);
    XMapRaised(display, window);
    XSync(display, False);
    
    depth = DefaultDepth(display, DefaultScreen(display));
    buffer = XCreatePixmap(display, DefaultRootWindow(display), WIN_W, WIN_H, depth);
}

void createGraphicsContext(){
    XGCValues values;
    unsigned long value_mask;
    
    gc = XCreateGC(display, buffer, 0, 0);
    char* font_name = "*-helvetica-medium-r-*-12-*";
    font_info[0] = XLoadQueryFont(display, font_name);
    if (!font_info[0]) {
        fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
    }
    font_name = "*-helvetica-bold-r-*-12-*";
    font_info[1] = XLoadQueryFont(display, font_name);
    if (!font_info[1]) {
        fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
    }
    font_name = "*-helvetica-bold-r-*-34-*";
    font_info[2] = XLoadQueryFont(display, font_name);
    if (!font_info[2]) {
        fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
    }
    XSetFont(display, gc, font_info[0]->fid);
    
    enemyGC = XCreateGC(display, buffer, 0, 0);
    lineGC = XCreateGC(display, buffer, 0, 0);
    XSetForeground(display, lineGC, black.pixel);
    
    floorGC = XCreateGC(display, buffer, 0, 0);
    XSetLineAttributes(display, floorGC, 20, LineSolid, CapRound, JoinRound);
}

void registerForEvents(){
  XSelectInput(display, window, 
	           ButtonPressMask
	          |ButtonReleaseMask
	          |KeyPressMask
              |ExposureMask);
}


void splashScreen(){
    long long cur_time;
    while(showSplash){
        if (XPending(display)) {
            handleEvent();
        }
        long long cur_time;
        cur_time = getTime();
        if (abs(cur_time - old_time) > 40) {
            old_time = cur_time;
            redrawScreen();
        }
        usleep(1000);
    }
}

void eventLoop(){
    while(castle.curHealth > 0){
        if (XPending(display)) {
            handleEvent();
        }
        programAnimation();
        usleep(1000);
    }
}

void gameOver() {
    XEvent an_event;
    
    XSetForeground(display, gc, grey.pixel);
    XFillRectangle(display, buffer, gc, 50, 50, WIN_W - 100, 260);
    XDrawRectangle(display, buffer, lineGC, 50, 50, WIN_W - 100, 260);
    XSetForeground(display, gc, black.pixel);
    drawText("GAME OVER", WIN_W / 2, 360/2, 2, CENTER, CENTER);
    drawText("Press any key to exit", WIN_W/2, 290, 0, CENTER, BOTTOM);
    // draw buffer to screen
    XCopyArea(display, buffer, window, gc, 0, 0, WIN_W, WIN_H, 0, 0);
 
    while(1) {
        if (XPending(display)) {
            XNextEvent(display, &an_event);
            if (an_event.type == KeyPress) {
                quit();
            }
        }
        usleep(1000);
    }
}

        
void handleEvent(){
    XEvent an_event;
    KeySym key_symbol;
    XNextEvent(display, &an_event);
    switch (an_event.type) {
        case KeyPress:
            key_symbol = XKeycodeToKeysym(display, an_event.xkey.keycode, 0);
            switch (key_symbol) {
                case XK_q:
                    quit();
                case XK_p:
                    if (showSplash == 0) {
                        if (pauseButton->pushed) {
                            pauseButton->pushed = 0;
                        } else {
                            pauseButton->pushed = 1;
                        }
                    }
                    break;
                case XK_s:
                    if (showSplash == 0) {
                        if (speedButton->pushed) {
                            speedButton->pushed = 0;
                        } else {
                            speedButton->pushed = 1;
                        }
                    }
                    break;
                case XK_n:
                    if (showSplash == 0) {
                        sendAndCreateWave();
                    }
                    break;
                case XK_d:
                    if (showSplash == 0) {
                        deleteSelection();
                    }
                    break;
                case XK_m:
                    if (showSplash == 0) {
                        money += 1000;
                    }
                    break;                    
                case XK_space:
                    if (showSplash) {
                        showSplash = 0;
                    } else {
                        if ((an_event.xkey.x >= 0 && an_event.xkey.x <= WIN_W) && (an_event.xkey.y >= 0 && an_event.xkey.y <= 360)) { // click was on stage
                            if (pushedButtons[SHOP] != NULL && pushedButtons[SHOP]->id == TURRET && pushedButtons[TURRET] != NULL) {
                                placeDefense(an_event.xkey.x, an_event.xkey.y, ((Defense*)(pushedButtons[TURRET]->item))->type);
                            } else if (pushedButtons[SHOP] != NULL && pushedButtons[SHOP]->id == TRAP && pushedButtons[TRAP] != NULL) {
                                placeDefense(an_event.xkey.x, an_event.xkey.y, ((Defense*)(pushedButtons[TRAP]->item))->type);
                            }
                        }
                    }
                    break;
            }
            break;
        case Expose:
            redrawScreen();
            break;
        case ButtonPress:
            if (showSplash == 0) {
                if ((an_event.xbutton.x >= 0 && an_event.xbutton.x <= WIN_W) && (an_event.xbutton.y >= 0 && an_event.xbutton.y <= 360)) { // click was on stage
                    if ((an_event.xbutton.x >= 20 && an_event.xbutton.x <= 80) && (an_event.xbutton.y >= 20 && an_event.xbutton.y <= 110)) { // click was on castle/door
                        if (pushedButtons[SHOP] != NULL && pushedButtons[SHOP]->id == CASTLE && pushedButtons[CASTLE] != NULL) {
                            upgradeCastle(pushedButtons[CASTLE]->id, &castle);
                        }
                    }
                    selectDefense(an_event.xbutton.x, an_event.xbutton.y);
                    if (selection == NULL) {
                        // there was nothing there, try to place a defense
                        if (pushedButtons[SHOP] != NULL && pushedButtons[SHOP]->id == TURRET && pushedButtons[TURRET] != NULL) {
                            placeDefense(an_event.xbutton.x, an_event.xbutton.y, ((Defense*)(pushedButtons[TURRET]->item))->type);
                        } else if (pushedButtons[SHOP] != NULL && pushedButtons[SHOP]->id == TRAP && pushedButtons[TRAP] != NULL) {
                            placeDefense(an_event.xbutton.x, an_event.xbutton.y, ((Defense*)(pushedButtons[TRAP]->item))->type);
                        }
                    }
                } else { // click was on controls
                    clickUI(an_event.xbutton.x, an_event.xbutton.y);
                }
            }
            break;
        case ButtonRelease:
            deleteButton->pushed = 0;
            nextWaveButton->pushed = 0;
            break;
    }
}

void quit() {
    pauseButton->pushed = 1;
    XFreePixmap(display, buffer);
    XCloseDisplay(display);
    listDelete(eList);
    if (nextWave != NULL) {
        listDelete(nextWave);
    }
    listDelete(bList);
    listForEach(buttonList, freeButtonPointers);
    listDelete(buttonList);
    free(systemButtonList);
    free(shopButtonList);
    free(trapButtonList);
    free(turretButtonList);
    free(castleButtonList);
    exit(0);
}

void drawSplash() {
    int i;
    Info info;
    XSetForeground(display, gc, black.pixel);
    drawText("Corridor Defense", WIN_W/2, 20, 2, CENTER, TOP);
    drawText("Jayson Rhynas, 20299211", WIN_W/2, 55, 0, CENTER, TOP);

    drawText("Instructions:", 50, 100, 1, LEFT, TOP);
    char *instructions[] = {" - click on an item in the store to see a description",
                            " - place turrets and traps by selecting an item in the store",
                            "   and clicking a grid spot in the playing field. Each spot can",
                            "   hold at most 1 turret and 1 trap. Items can also be placed",
                            "   by holding space and moving the mouse over the playing field",
                            "   (be careful - this can be expensive!)",
                            " - delete items by selecting the desired item on the field and",
                            "   clicking the Delete button or pressing d",
                            " - upgrade your door by selecting an upgrade from the Door menu",
                            "   and then clicking on the door",
                            " - the game can be paused indefinitely by clicking the || button",
                            "   or by pressing p. Items can still be placed while paused,",
                            "   however the Wave Timer will also continue to increase",
                            " - click the > button or press s to speed up the game",
                            " - flying enemies are not affected by traps",
                            " - one point is awarded for every enemy you destroy before it",
                            "   reaches the final door",
                            " - play to get the highest score possible!"};
    drawMultilineText(instructions, 18, 20, 50, 120, 0, LEFT, TOP);
    
    drawText("Enemies:", 470, 100, 1, LEFT, TOP);
    drawText("Note: flying enemies are not affected by traps", 470, 120, 0, LEFT, TOP);
    for (i = 0; i < NUM_ENEMY_TYPES; i++) {
        drawThumbEnemy(i, 480, i*50 + 155);
        info = defenseInfo[ROCKET + NUM_CASTLE + 1 + i];
        drawText(info.name, 510, i*50 + 140, 1, LEFT, TOP);
        drawText(info.desc[0], 510, i*50 + 160, 0, LEFT, TOP);
    }
    
    drawText("Keyboard Shortcuts:", 750, 100, 1, LEFT, TOP); 
    char *keys[] = {"q",
                    "p",
                    "s",
                    "n",
                    "d",
                    "space",
                    "m"};
    char *keyDesc[] = {"quit game (X button)",
                       "pause game (|| button)",
                       "speed up game (x4) (> button)",
                       "send next wave",
                       "delete defense",
                       "place defense",
                       "add $1000 (cheat)"};
    drawMultilineText(keys, 7, 20, 750, 120, 0, LEFT, TOP);
    drawMultilineText(keyDesc, 7, 20, 800, 120, 0, LEFT, TOP);
    
    drawText("Press space to start", WIN_W - 100, WIN_H - 100, 2, RIGHT, BOTTOM);
}

void redrawScreen(){
    // clear the buffer
    XSetForeground(display, gc, grey.pixel);
    XFillRectangle(display, buffer, gc, 0, 0, WIN_W, WIN_H);
    // draw elements to buffer
    if (showSplash) {
        drawSplash();
    } else {
        drawStage();
        drawCastle(&castle, 20, 20);
        listForEach(eList, drawEnemy);
        listForEach(bList, drawBullet);
        drawTurrets();
        drawTraps();
        drawCovers();
        drawUI();
        if (pauseButton->pushed) {
            XSetForeground(display, gc, red.pixel);
            drawText("Paused", WIN_W/2, 360/2, 2, CENTER, CENTER);
        }
    }
    // draw buffer to screen
    XCopyArea(display, buffer, window, gc, 0, 0, WIN_W, WIN_H, 0, 0);
}

void programAnimation(){
    int waitTime = 40;
    long long cur_time;
    cur_time = getTime();
    if (speedButton->pushed) {
        waitTime = 10;
    }
    if (abs(cur_time - old_time) > waitTime) {
        old_time = cur_time;
        if (pauseButton->pushed == 0) {
            if (cur_time >= nextWaveTime) {
                sendAndCreateWave();
            }
            listForEach(eList, moveEnemy);
            targetDefenses();
            moveTraps();
            listForEach(bList, moveBullet);
            checkCollisions();
            checkTraps();
            cleanupEnemies();
            cleanupBullets();
        }
        redrawScreen();
    }
}


void drawStage() {
    int r, c;
    XSetForeground(display, gc, dark_grey.pixel);
    XFillRectangle(display, buffer, gc, 0, 60, WIN_W, 50);
    XFillRectangle(display, buffer, gc, 0, 180, WIN_W, 50);
    XFillRectangle(display, buffer, gc, 0, 300, WIN_W, 50);
    // Roof
    XDrawLine(display, buffer, floorGC, 0, 0, WIN_W, 0);
    // 3rd Floor
    XDrawLine(display, buffer, floorGC, 0, 120, WIN_W, 120);
    // 2nd Floor
    XDrawLine(display, buffer, floorGC, 0, 240, WIN_W, 240);
    // 1st Floor
    XDrawLine(display, buffer, floorGC, 0, 360, WIN_W, 360);

    XSetForeground(display, gc, grey_10.pixel);
    // 2-3 Stairs
    XFillRectangle(display, buffer, gc, WIN_W - 80, 20, 60, 90);
    XFillRectangle(display, buffer, gc, WIN_W - 80, 140, 60, 90);
    // 1-2 Stairs
    XFillRectangle(display, buffer, gc, 20, 140, 60, 90);
    XFillRectangle(display, buffer, gc, 20, 260, 60, 90);

    XSetForeground(display, gc, black.pixel);
    // 2-3 Stairs
    XDrawRectangle(display, buffer, gc, WIN_W - 80, 20, 60, 90);
    XDrawRectangle(display, buffer, gc, WIN_W - 80, 140, 60, 90);
    // 1-2 Stairs
    XDrawRectangle(display, buffer, gc, 20, 140, 60, 90);
    XDrawRectangle(display, buffer, gc, 20, 260, 60, 90);
        
    // Grid
    XSetForeground(display, gc, white.pixel);
    for (r = 0; r < 3; r++) {
        for(c = 1; c < (WIN_W / 100) - 1; c++) {
            XDrawRectangle(display, buffer, gc, c*100 + 1, r * 120, 98, 9);
            XDrawRectangle(display, buffer, gc, c*100 + 1, r * 120 + 110, 98, 9);
        }
    }
}

void deleteSelection() {
    if (selection != NULL) {
        if (selection->type < GUN) {
            if (trapArray[selection->floor][selection->col] == selection) {
                trapArray[selection->floor][selection->col] = NULL;
                free(selection);
            } else {
                fprintf(stderr, "trapArray - selection mismatch\n");
            }
        } else {
            if (turretArray[selection->floor][selection->col] == selection) {
                turretArray[selection->floor][selection->col] = NULL;
                free(selection);
            } else {
                fprintf(stderr, "turretArray - selection mismatch\n");
            }
        }
        selection = NULL;
    }
}

long long getTime() {
    struct timeval tv;
    long long result;
    gettimeofday(&tv, 0);
    result = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return result;
}

void drawCovers() {
    XSetForeground(display, gc, grey.pixel);
    // 2-3 Stairs
    XFillRectangle(display, buffer, gc, WIN_W - 20, 20, 20, 40);
    XFillRectangle(display, buffer, gc, WIN_W - 20, 140, 20, 40);
    // 1-2 Stairs
    XFillRectangle(display, buffer, gc, 0, 140, 20, 40);
    XFillRectangle(display, buffer, gc, 0, 260, 20, 40);

    XSetForeground(display, gc, dark_grey.pixel);
    // 2-3 Stairs
    XFillRectangle(display, buffer, gc, WIN_W - 20, 60, 20, 50);
    XFillRectangle(display, buffer, gc, WIN_W - 20, 180, 20, 50);
    // 1-2 Stairs
    XFillRectangle(display, buffer, gc, 0, 180, 20, 50);
    XFillRectangle(display, buffer, gc, 0, 300, 20, 50);
    
    XSetForeground(display, gc, black.pixel);
    XDrawLine(display, buffer, gc, WIN_W - 20, 20, WIN_W - 20, 109);
    XDrawLine(display, buffer, gc, WIN_W - 20, 140, WIN_W - 20, 229);
    XDrawLine(display, buffer, gc, 20, 140, 20, 229);
    XDrawLine(display, buffer, gc, 20, 260, 20, 349);
}

void drawUI() {
    int i;
    int nextWave_interval;
    int nextWave_min;
    int nextWave_sec;
    char moneyStr[20];
    char pointsStr[20];
    char nextWaveStr[40];
    // UI Background
    XSetForeground(display, gc, grey_25.pixel);
    XFillRectangle(display, buffer, gc, 0, 370, WIN_W, WIN_H - 370);
    // info panel
    XSetForeground(display, gc, grey.pixel);
    XFillRectangle(display, buffer, gc, 670, 410, WIN_W - 680, 100);
    XDrawRectangle(display, buffer, lineGC, 670, 410, WIN_W - 680, 100);
    
    XSetForeground(display, gc, white.pixel);
    // money
    sprintf(moneyStr, "Money: $%d", money);
    drawText(moneyStr, 10, 390, 1, LEFT, CENTER);
    // points
    sprintf(pointsStr, "Points: %d", points);
    drawText(pointsStr, 120, 390, 1, LEFT, CENTER);
    
    // next wave timer
    nextWave_interval = (nextWaveTime - getTime())/1000;
    nextWave_min = nextWave_interval / 60;
    nextWave_sec = nextWave_interval - 60*nextWave_min;
    sprintf(nextWaveStr, "Next Wave in %d:%02d           x%d", nextWave_min, nextWave_sec, nextWave->size);
    drawText(nextWaveStr, 670, 390, 1, LEFT, CENTER);
    
    drawThumbEnemy(((Enemy*)(nextWave->first->item))->type, 805, 390);
    
    // show/hide buttons
    if (pushedButtons[SHOP] != NULL) {
        if (pushedButtons[SHOP]->id == TURRET) {
            listForEach(turretButtonList, makeVisible);
            if (pushedButtons[TURRET] != NULL) {
                drawInfo(((Defense*)(pushedButtons[TURRET]->item))->type);
            }
            // hide and deselect other buttons
            if (pushedButtons[TRAP] != NULL) {
                pushedButtons[TRAP]->pushed = 0;
            }
            pushedButtons[TRAP] = NULL;
            if (pushedButtons[CASTLE] != NULL) {
                pushedButtons[CASTLE]->pushed = 0;
            }
            pushedButtons[CASTLE] = NULL;
            listForEach(trapButtonList, makeInvisible);
            listForEach(castleButtonList, makeInvisible);
        } else if (pushedButtons[SHOP]->id == TRAP) {
            listForEach(trapButtonList, makeVisible);
            if (pushedButtons[TRAP] != NULL) {
                drawInfo(((Defense*)(pushedButtons[TRAP]->item))->type);
            }
            // hide and deselect other buttons
            if (pushedButtons[TURRET] != NULL) {
                pushedButtons[TURRET]->pushed = 0;
            }
            pushedButtons[TURRET] = NULL;
            if (pushedButtons[CASTLE] != NULL) {
                pushedButtons[CASTLE]->pushed = 0;
            }
            pushedButtons[CASTLE] = NULL;
            listForEach(turretButtonList, makeInvisible);
            listForEach(castleButtonList, makeInvisible);
        } else if (pushedButtons[SHOP]->id == CASTLE) {
            listForEach(castleButtonList, makeVisible);
            if (pushedButtons[CASTLE] != NULL) {
                drawInfo(((Castle*)(pushedButtons[CASTLE]->item))->type + ROCKET + 1);
            }
            // hide and deselect other buttons
            if (pushedButtons[TRAP] != NULL) {
                pushedButtons[TRAP]->pushed = 0;
            }
            pushedButtons[TRAP] = NULL;
            if (pushedButtons[TURRET] != NULL) {
                pushedButtons[TURRET]->pushed = 0;
            }
            pushedButtons[TURRET] = NULL;
            listForEach(trapButtonList, makeInvisible);
            listForEach(turretButtonList, makeInvisible);
        }
    } else {
        // hide and deselect all buttons
        if (pushedButtons[TRAP] != NULL) {
            pushedButtons[TRAP]->pushed = 0;
        }
        pushedButtons[TRAP] = NULL;
        if (pushedButtons[TURRET] != NULL) {
            pushedButtons[TURRET]->pushed = 0;
        }
        pushedButtons[TURRET] = NULL;
        if (pushedButtons[CASTLE] != NULL) {
            pushedButtons[CASTLE]->pushed = 0;
        }
        pushedButtons[CASTLE] = NULL;
        listForEach(trapButtonList, makeInvisible);
        listForEach(turretButtonList, makeInvisible);
        listForEach(castleButtonList, makeInvisible);
    }
    // draw all buttons
    listForEach(buttonList, drawButton);
}

void drawInfo(int lookup) {
    char price[6];
    Info info;
    info = defenseInfo[lookup];
    sprintf(price, "$%d", info.price);
    XSetForeground(display, gc, black.pixel);
    drawText(info.name, 680, 420, 1, LEFT, TOP);
    drawText(price, 980, 420, 1, RIGHT, TOP);
    drawMultilineText(info.desc, 3, 20, 680, 440, 0, LEFT, TOP);
}

void clickUI(int x, int y) {
    Node *cur, *next;
    int i, j;
    Button *b, *p;
    cur = buttonList->first;
    while (cur != NULL) {
        next = cur->next;
        b = cur->item;
        if (b->visible && x > b->x && x < b->x + b->w && y > b->y && y < b->y + b->h) {
            break;
        }
        cur = next;
    }
    if (cur != NULL) {
        if (b->groupNum == -1) {
            if (b->pushed) {
                b->pushed = 0;
            } else {
                b->pushed = 1;
                if (b->callback != NULL) {
                    b->callback();
                }
            }
        } else {
            if (b->pushed) {
                b->pushed = 0;
                pushedButtons[b->groupNum] = NULL;
            } else {
                p = pushedButtons[b->groupNum];
                if (p != NULL) {
                    p->pushed = 0;
                }
                pushedButtons[b->groupNum] = b;
                b->pushed = 1;
                if (b->callback != NULL) {
                    b->callback();
                }
            }
        }
    }
}

Button* createButton(int x, int y, int w, int h, char* label, void *item, int visible, int id, ButtonGroup groupNum, void(*callback)()) {
    int str_len;
    Button *b;
    b = (Button*)malloc(sizeof(Button));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->pushed = 0;
    b->visible = visible;
    b->groupNum = groupNum;
    b->id = id;
    b->item = item;
    b->callback = callback;
    if (label != NULL) {
        str_len = strlen(label);
        b->label = (char*)malloc((str_len + 1) * sizeof(char));
        strcpy(b->label, label);
    } else {
        b->label = NULL;
    }
    return b;
}

void makeVisible(void *vb) {
    Button *b;
    b = vb;
    b->visible = 1;
}

void makeInvisible(void *vb) {
    Button *b;
    b = vb;
    b->visible = 0;
}

void drawButton(void *vb) {
    Button *b;
    if (vb == NULL) {
        return;
    }
    b = vb;
    if (b->visible == 0) {
        return;
    }
    if (b->item == NULL) {
        // text button
        if (b->pushed) {
            XSetForeground(display, gc, grey_50.pixel);
        } else {
            XSetForeground(display, gc, grey.pixel);
        }
        XFillRectangle(display, buffer, gc, b->x, b->y, b->w, b->h);
        XDrawRectangle(display, buffer, lineGC, b->x, b->y, b->w, b->h);
        XSetForeground(display, gc, black.pixel);
        if (b->label != NULL) {
            drawText(b->label, b->x + (b->w / 2), b->y + (b->h / 2), 0, CENTER, CENTER);
        }
    } else {
        // image BUTTON
        XSetForeground(display, gc, grey.pixel);
        XFillRectangle(display, buffer, gc, b->x, b->y, b->w, b->h/2);
        XSetForeground(display, gc, dark_grey.pixel);
        XFillRectangle(display, buffer, gc, b->x, b->y + b->h/2, b->w, b->h/2);
        if (b->groupNum == CASTLE) {
            drawCastle((Castle*)b->item, b->x + 20, b->y + 10);
        } else {
            drawDefense(b->item);
        }
        if (b->pushed) {
            XSetForeground(display, lineGC, blue.pixel);
            XSetLineAttributes(display, lineGC, 3, LineSolid, CapButt, JoinMiter);
        }
        XDrawRectangle(display, buffer, lineGC, b->x, b->y, b->w, b->h);
        XSetForeground(display, lineGC, black.pixel);
        XSetLineAttributes(display, lineGC, 1, LineSolid, CapButt, JoinMiter);
    }   
}
    
void freeButtonPointers(void *vb) {
    Button *b;
    b = vb;
    if (b->item != NULL) {
        free(b->item);
    }
    if (b->label != NULL) {
        free(b->label);
    }
}

void drawText(char* string, int x, int y, int font_num, Alignment hAlign, Alignment vAlign) {
    int string_width = XTextWidth(font_info[font_num], string, strlen(string));
    int actualX, actualY;
    int font_height = font_info[font_num]->ascent;
    if (hAlign == CENTER) {
        actualX = x - (string_width / 2);
    } else if (hAlign == RIGHT) {
        actualX = x - string_width;
    } else {
        actualX = x;
    }
    if (vAlign == CENTER) {
        actualY = y + (font_height / 2);
    } else if (vAlign == TOP) {
        actualY = y + font_height;
    } else {
        actualY = y;
    }
    XSetFont(display, gc, font_info[font_num]->fid);
    XDrawString(display, buffer, gc, actualX, actualY, string, strlen(string));
}

void drawMultilineText(char* strings[], int nlines, int line_height, int x, int y, int font_num, Alignment hAlign, Alignment vAlign) {
    int i;
    int string_width, font_height, total_height;
    int actualX, actualY;
    char *cur_string;
    
    font_height = font_info[font_num]->ascent;
    total_height = (nlines - 1) * line_height + font_height;

    if (vAlign == CENTER) {
        actualY = y - (total_height / 2) + font_height;
    } else if (vAlign == TOP) {
        actualY = y + font_height;
    } else {
        actualY = y - total_height + font_height;
    }
    XSetFont(display, gc, font_info[font_num]->fid);
    for (i = 0; i < nlines; i++) {
        cur_string = strings[i];
        string_width = XTextWidth(font_info[font_num], cur_string, strlen(cur_string));
        if (hAlign == CENTER) {
            actualX = x - (string_width / 2);
        } else if (hAlign == RIGHT) {
            actualX = x - string_width;
        } else {
            actualX = x;
        }
        XDrawString(display, buffer, gc, actualX, actualY + i * line_height, cur_string, strlen(cur_string));
    }
}
void printFonts() {
    int numFonts, i;
    char ** fonts;
    fonts = XListFonts(display, "*-helvetica-*", 500, &numFonts);
    for (i = 0; i < numFonts; i++) {
        fprintf(stderr, "%s\n", fonts[i]);
    }
}

void rotatePoints(XPoint *points, int npoints, int rx, int ry, float a) {
    int i;
    for (i=0; i<npoints; i++) {
        float x, y;
        x = points[i].x - rx;
        y = points[i].y - ry;
        points[i].x =  rx + x * cos(a) - y * sin(a);
        points[i].y =  ry + x * sin(a) + y * cos(a);
    }
}

void flipPoints(XPoint *points, int npoints, int fx, int fy, int flipHorizontal, int flipVertical) {
    int i;
    for (i=0; i<npoints; i++) {
        int x, y;
        x = points[i].x - fx;
        y = points[i].y - fy;
        points[i].x =  fx + x * flipHorizontal;
        points[i].y =  fy + y * flipVertical;
    }
}