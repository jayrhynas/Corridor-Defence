/* 
 * File:    enemies.c
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Implements enemy functions.
 */

#include "enemies.h"
#include "main.h"

Enemy* createEnemy(EType type, int x, int id) {
    Enemy *e;
    e = (Enemy*)malloc(sizeof(Enemy));
    e->type = type;
    e->container = NULL;
    e->id = id;
    e->x = x;
    e->floor = 0;
    e->dy = 0;
    e->size = 20;
    e->flying = 0;
    e->slow = 0;
    e->sTime = 0;
    e->burn = 0;
    e->bTime = 0;
    e->lastBurn = 0;
    e->burnInterval = 1000;
    e->maxEffectTime = 10000;
    e->dTime = 0;
    e->damageInterval = 750;
    e->showHealth = 1;
    
    // specific inits
    if (e->type == SLOW || e->type == FLYING_SLOW) {
        e->dx = -2;
        e->y = 308 + e->size;
        if (e->type == FLYING_SLOW) {
            e->y = 290 + e->size;
            e->flying = 1;
        }
        e->maxHealth = 100;
        if (e->type == SLOW) {
            e->damage = 50;
            e->dollarValue = 10;
        } else {
            e->damage = 20;
            e->dollarValue = 20;
        }
    } else if (e->type == FAST || e->type == FLYING_FAST) {
        e->dx = -4;
        e->y = 308 + e->size;
        if (e->type == FLYING_FAST) {
            e->y = 290 + e->size;
            e->flying = 1;
        }
        e->maxHealth = 50;
        if (e->type == FAST) {
            e->damage = 40;
            e->dollarValue = 20;
        } else {
            e->damage = 10;
            e->dollarValue = 50;
        }
    }
    e->health = e->maxHealth;
    return e;
}

void moveEnemy(void *ve) {
    long long cur_time;
    Enemy *e;
    e = ve;
    // if an enemy goes off screen, reverse its movement and move it up a level
    if ((e->x < -50 && e->dx < 0) || (e->x > WIN_W + 50 && e->dx > 0)) {
        e->dx = e->dx * -1;
        if (e->x < 0) {
            e->x = -50;
        } else {
            e->x = WIN_W + 50;
        }
        e->y -= 120;
        e->floor++;
    }
    // if an enemy runs into the door, destory the enemy (no money awarded) and damage the door
    if (e->floor == 2 && e->x < 70) {
        e->health = 0;
        e->dollarValue = 0;
        castle.curHealth -= e->damage;
        return;
    }
    
    // disable any effects if they have expired
    cur_time = getTime();
    if (abs(cur_time - e->sTime) > e->maxEffectTime) {
        e->slow = 0;
    }
    if (abs(cur_time - e->bTime) > e->maxEffectTime) {
        e->burn = 0;
    }
    
    // apply burn damage
    if (e->burn && (abs(cur_time - e->lastBurn) > e->burnInterval)) {
        e->lastBurn = cur_time;
        e->health -= 2;
    }
    
    // move the enemy
    if (e->slow) {
        e->x += e->dx / 2;
        e->y += e->dy / 2;
    } else {
        e->x += e->dx;
        e->y += e->dy;
    }
}

void drawEnemy(void *ve) {
    Enemy *e;
    e = (Enemy*)ve;
    XSetLineAttributes(display, enemyGC, 2, LineSolid, CapRound, JoinRound);
    if (e->type == SLOW) {
        XSetForeground(display, enemyGC, green.pixel);
        XFillArc(display, buffer, enemyGC, e->x - e->size, e->y - e->size, e->size*2, e->size*3, 0, 360*32);
        XFillArc(display, buffer, enemyGC, e->x - e->size/2 - e->size/4, e->y + e->size/2, e->size/2, e->size/2, 0, 360*64);
        XFillArc(display, buffer, enemyGC, e->x + e->size/2 - e->size/4, e->y + e->size/2, e->size/2, e->size/2, 0, 360*64);
        if (e->burn) {
            XSetForeground(display, enemyGC, red.pixel);
        } else {
            XSetForeground(display, enemyGC, black.pixel);
        }
        XDrawArc(display, buffer, enemyGC, e->x - e->size, e->y - e->size, e->size*2, e->size*3, 0, 360*32);
        XDrawLine(display, buffer, enemyGC, e->x - e->size, e->y + e->size/2, e->x + e->size, e->y + e->size/2);
        XDrawArc(display, buffer, enemyGC, e->x - e->size/2 - e->size/4, e->y + e->size/2, e->size/2, e->size/2, 0, 360*64);
        XDrawArc(display, buffer, enemyGC, e->x + e->size/2 - e->size/4, e->y + e->size/2, e->size/2, e->size/2, 0, 360*64);
    } else if (e->type == FLYING_SLOW) {
        XSetForeground(display, enemyGC, brown.pixel);
        XFillArc(display, buffer, enemyGC, e->x - e->size, e->y - e->size, e->size*2, e->size*2, 0, 360*64);
        if (e->burn) {
            XSetForeground(display, enemyGC, red.pixel);
        } else {
            XSetForeground(display, enemyGC, black.pixel);
        }
        XDrawArc(display, buffer, enemyGC, e->x - e->size, e->y - e->size, e->size*2, e->size*2, 0, 360*64);
    } else if (e->type == FAST) {
        int npoints = 5;
        XPoint points[] = {{e->x - e->size, e->y + e->size},
                           {e->x + e->size, e->y - e->size},
                           {e->x + e->size/4, e->y + e->size/2},
                           {e->x + e->size, e->y + e->size},
                           {e->x - e->size, e->y + e->size}};
        if (e->floor == 1) {
            flipPoints(points, npoints, e->x, e->y, -1, 1);
        }
        XSetForeground(display, enemyGC, blue.pixel);
        XFillPolygon(display, buffer, enemyGC, points, npoints, Nonconvex, CoordModeOrigin);
        if (e->burn) {
            XSetForeground(display, enemyGC, red.pixel);
        } else {
            XSetForeground(display, enemyGC, black.pixel);
        }
        XDrawLines(display, buffer, enemyGC, points, npoints, CoordModeOrigin);
    } else if (e->type == FLYING_FAST) {
        int npoints = 7;
        XPoint points[] = {{e->x - e->size, e->y},
                           {e->x, e->y - e->size},
                           {e->x + e->size, e->y - e->size},
                           {e->x, e->y},
                           {e->x + e->size, e->y + e->size},
                           {e->x, e->y + e->size},
                           {e->x - e->size, e->y}};
        if (e->floor == 1) {
            flipPoints(points, npoints, e->x, e->y, -1, 1);
        }
        XSetForeground(display, enemyGC, yellow.pixel);
        XFillPolygon(display, buffer, enemyGC, points, npoints, Nonconvex, CoordModeOrigin);
        if (e->burn) {
            XSetForeground(display, enemyGC, red.pixel);
        } else {
            XSetForeground(display, enemyGC, black.pixel);
        }
        XDrawLines(display, buffer, enemyGC, points, npoints, CoordModeOrigin);
    }
    if (e->showHealth) {
        // draw health bar
        XSetForeground(display, enemyGC, white.pixel);
        XFillRectangle(display, buffer, enemyGC, e->x - e->size, e->y - e->size - 10, e->size * 2, 5);
        XSetForeground(display, enemyGC, red.pixel);
        XFillRectangle(display, buffer, enemyGC, e->x - e->size, e->y - e->size - 10, e->size * 2 * ((float)e->health/(float)e->maxHealth), 5);
        XDrawRectangle(display, buffer, lineGC, e->x - e->size, e->y - e->size - 10, e->size * 2, 5);
    }
}

void drawThumbEnemy(EType type, int x, int y) {
    Enemy *e;
    e = thumbEnemies[type];
    e->x = x;
    e->y = y;
    e->size = 10;
    e->showHealth = 0;
    drawEnemy(thumbEnemies[type]);
}

void cleanupEnemies() {
    Node *cur, *next;
    Enemy *e;
    cur = eList->first;
    while (cur != NULL) {
        next = cur->next;
        e = cur->item;
        if (e->floor > 2 || e->health <= 0) {
            money += e->dollarValue;
            if (e->dollarValue > 0) {   // enemies who reach the end have a dollar value of 0 and don't count
                points++;
            }
            listRemove(eList, cur);
        }
        cur = next;
    }
}

void createWave(EType type, int num) {
    int i;
    Enemy *e;
    if (nextWave != NULL) {
        listDelete(nextWave);
        fprintf(stderr, "called createWave when nextWave != NULL, deleting nextWave");
    }
    nextWave = listCreate(ENEMY);
    for (i = 0; i < num; i++) {
        e = createEnemy(type, WIN_W + 100 + i*50, i);
        listAppend(nextWave, e);
    }
}

void createRandomWave() {
    EType type;
    int num;
    type = ((double)random()/(double)RAND_MAX) * NUM_ENEMY_TYPES;
    num = ((double)random()/(double)RAND_MAX) * 3 + 4;
    createWave(type, num);
    nextWaveTime = getTime() + (((double)random()/(double)RAND_MAX) * 30 + 45) * 1000;
}

void sendWave() {
    Node *cur, *next;
    if (nextWave != NULL) {
        cur = nextWave->first;
        // copy enemies to main eList
        while (cur != NULL) {
            listAppend(eList, cur->item);
            cur = cur->next;
        }
        // free the nodes and the list, but not the enemies
        cur = nextWave->first;
        while (cur != NULL) {
            next = cur->next;
            free(cur);
            cur = next;
        }
        free(nextWave);
        nextWave = NULL;
    }
}

void sendAndCreateWave() {
    sendWave();
    createRandomWave();
}