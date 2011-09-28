/* 
 * File:    defenses.c
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Implements defense functions.
 */

#include "defenses.h"
#include "main.h"

void upgradeCastle(CType type, Castle *c) {
    if (c->type < type && defenseInfo[type + ROCKET + 1].price <= money) {
        if (type == WOOD) {
            c->type = WOOD;
            c->maxHealth = 1000;
            c->curHealth = c->maxHealth;
        } else if (type == BRICK) {
            c->type = BRICK;
            c->maxHealth = 2000;
            c->curHealth = c->maxHealth;
        } else if (type == IRON) {
            c->type = IRON;
            c->maxHealth = 3000;
            c->curHealth = c->maxHealth;
        } else if (type == STEEL) {
            c->type = STEEL;
            c->maxHealth = 4000;
            c->curHealth = c->maxHealth;
        } else if (type == HEALTH) {
            c->curHealth += 500;
            if (c->curHealth > c->maxHealth) {
                c->curHealth = c->maxHealth;
            }
        }
        money -= defenseInfo[type + ROCKET + 1].price;
    }
}

void drawCastle(Castle *c, int x, int y) {
    if (c->type == WOOD) {
        int i;
        XSetForeground(display, gc, brown.pixel);
        XFillRectangle(display, buffer, gc, x, y, 60, 90);
        for (i = 1; i <= 5; i++) {
            XDrawLine(display, buffer, lineGC, x + i*10, y, x + i*10, y + 90);
        }
        XDrawRectangle(display, buffer, lineGC, x, y, 60, 90);
    } else if (c->type == BRICK) {
        int i;
        XSetForeground(display, gc, brick_red.pixel);
        XFillRectangle(display, buffer, gc, x, y, 60, 90);
        for (i = 1; i <= 8; i++) {
            XDrawLine(display, buffer, lineGC, x, y + i*10, x + 60, y + i*10);
        }
        for (i = 0; i <= 8; i++) {
            if (i%2) {
                XDrawLine(display, buffer, lineGC, x + 20, y + i*10, x + 20, y + i*10 + 10);
                XDrawLine(display, buffer, lineGC, x + 40, y + i*10, x + 40, y + i*10 + 10);
            } else {
                XDrawLine(display, buffer, lineGC, x + 10, y + i*10, x + 10, y + i*10 + 10);
                XDrawLine(display, buffer, lineGC, x + 30, y + i*10, x + 30, y + i*10 + 10);
                XDrawLine(display, buffer, lineGC, x + 50, y + i*10, x + 50, y + i*10 + 10);
            }
        }
        XDrawRectangle(display, buffer, lineGC, x, y, 60, 90);
    } else if (c->type == IRON) {
        int i;
        // door
        XSetForeground(display, gc, dark_grey.pixel);
        XFillRectangle(display, buffer, gc, x, y, 60, 90);
        // rivets
        // top row
        for (i = 0; i <= 5; i++) {
            XDrawArc(display, buffer, lineGC, x + i*10 + 3, y + 3, 4, 4, 0, 360*64);
        }
        // sides
        for (i = 1; i <= 8; i++) {
            XDrawArc(display, buffer, lineGC, x + 3, y + i*10 + 3, 4, 4, 0, 360*64);
            XDrawArc(display, buffer, lineGC, x + 53, y + i*10 + 3, 4, 4, 0, 360*64);
        }
        // bottom row
        for (i = 0; i <= 5; i++) {
            XDrawArc(display, buffer, lineGC, x + i*10 + 3, y + 83, 4, 4, 0, 360*64);
        }
        XDrawRectangle(display, buffer, lineGC, x, y, 60, 90);
    } else if (c->type == STEEL) {
        int i;
        // door
        XSetForeground(display, gc, grey_50.pixel);
        XFillRectangle(display, buffer, gc, x, y, 60, 90);

        // bands
        XSetForeground(display, gc, dark_grey.pixel);
        XFillRectangle(display, buffer, gc, x + 20, y, 20, 90);
        XDrawRectangle(display, buffer, lineGC, x + 20, y, 20, 90);

        XFillRectangle(display, buffer, gc, x, y + 20, 60, 10);
        XFillRectangle(display, buffer, gc, x, y + 40, 60, 10);
        XFillRectangle(display, buffer, gc, x, y + 60, 60, 10);
        
        XDrawRectangle(display, buffer, lineGC, x, y + 20, 60, 10);
        XDrawRectangle(display, buffer, lineGC, x, y + 40, 60, 10);
        XDrawRectangle(display, buffer, lineGC, x, y + 60, 60, 10);
        
        // rivets
        // top row
        for (i = 0; i <= 5; i++) {
            XDrawArc(display, buffer, lineGC, x + i*10 + 3, y + 3, 4, 4, 0, 360*64);
        }
        // sides
        for (i = 1; i <= 8; i++) {
            XDrawArc(display, buffer, lineGC, x + 3, y + i*10 + 3, 4, 4, 0, 360*64);
            XDrawArc(display, buffer, lineGC, x + 53, y + i*10 + 3, 4, 4, 0, 360*64);
        }
        // bottom row
        for (i = 0; i <= 5; i++) {
            XDrawArc(display, buffer, lineGC, x + i*10 + 3, y + 83, 4, 4, 0, 360*64);
        }
        XDrawRectangle(display, buffer, lineGC, x, y, 60, 90);
    } else if (c->type == HEALTH) {
        XSetForeground(display, gc, red.pixel);
        XDrawRectangle(display, buffer, lineGC, x+20, y+15, 20, 60);
        XDrawRectangle(display, buffer, lineGC, x, y+35, 60, 20);
        XFillRectangle(display, buffer, gc, x+21, y+16, 19, 59);
        XFillRectangle(display, buffer, gc, x+1, y+36, 59, 19);

    }
    if (c->showHealth) {
        // draw health bar
        XSetForeground(display, gc, white.pixel);
        XFillRectangle(display, buffer, gc, x, y - 8, 60, 5);
        XSetForeground(display, gc, red.pixel);
        XFillRectangle(display, buffer, gc, x, y - 8, 60 * ((float)c->curHealth/(float)c->maxHealth), 5);
        XDrawRectangle(display, buffer, lineGC, x, y - 8, 60, 5);
    }
    if (c->curHealth < c->maxHealth) {
        // draw cracks
        int health_pct = (c->curHealth * 100) / c->maxHealth;
        int ncracks1 = 4;
        XSegment cracks1[] = {{x, y+18, x+15, y+26},
                              {x+15, y+26, x+25, y+27},
                              {x+60, y+53, x+42, y+67},
                              {x+42, y+67, x+38, y+78}};
        int ncracks2 = 4;
        XSegment cracks2[] = {{x, y+64, x+18, y+52},
                              {x+18, y+52, x+26, y+58},
                              {x+60, y+12, x+42, y+31},
                              {x+42, y+31, x+35, y+45}};
        int ncracks3 = 4;
        XSegment cracks3[] = {{x+5, y+90, x+21, y+67},
                              {x+21, y+67, x+31, y+65},
                              {x+35, y, x+23, y+12},
                              {x+23, y+12, x+31, y+24}};
        if (health_pct < 100) {
            XDrawSegments(display, buffer, lineGC, cracks1, ncracks1);
        }
        if (health_pct < 66) {
            XDrawSegments(display, buffer, lineGC, cracks2, ncracks2);
        }
        if (health_pct < 33) {
            XDrawSegments(display, buffer, lineGC, cracks3, ncracks3);
        }
    }
}

Defense* createDefense(DType type, int f, int c, int id) {
    Defense* d;
    
    d = (Defense*)malloc(sizeof(Defense));
    
    // common inits
    d->container = NULL;
    d->type = type;
    d->id = id;
    d->selected = 0;
    d->floor = f;
    d->col = c;
    if (d->floor >= 0) {
        d->x = (d->col + 1) * 100 + 50;
        d->y = (2 - d->floor) * 120 + 10;
        if (d->type < GUN) {
            // shift down if a trap
            d->y += 100;
        }
    } else {
        // repostion when in the store
        d->x = d->col * 110 + 170;
        d->y = 420;
        if (d->type < GUN) {
            // shift down if a trap
            d->y += 80;
        }
    }
    

    
    d->aim = M_PI_2;
    d->cur_aim = d->aim;
    d->aim_speed = 0.1;
    d->last_shot_time = getTime();
    d->triggered = 0;
    d->bulletSpeed = 0;
    d->damage = 0;
    d->reload_time = 0;
    // type-specific
    if (d->type == SPIKE) {
        d->damage = 2;
        d->reload_time = 0;
    } else if (d->type == BEAR_TRAP) {
        d->damage = 50;
        d->reload_time = 1000;  // used to delay deletion - doesn't actually reload
        d->cur_aim = d->aim = 0;
        d->aim_speed = 0.5;
    } else if (d->type == TRAP_DOOR) {
        d->damage = 10000;
        d->reload_time = 15000;
        d->cur_aim = d->aim = 0;
    } else if (d->type == GUN) {
        d->bulletSpeed = 12;
        d->damage = 10;
        d->reload_time = 1000;
    } else if (d->type == SHOTGUN) {
        d->bulletSpeed = 12;
        d->damage = 7;
        d->reload_time = 1500;
    } else if (d->type == MACHINE_GUN) {
        d->bulletSpeed = 15;
        d->damage = 2;
        d->reload_time = 250;
    } else if (d->type == ROCKET) {
        d->bulletSpeed = 8;
        d->damage = 20;
        d->reload_time = 3000;
    }
    return d;
}

void selectDefense(int x, int y) {
    int r, c;
    Defense *d;
    
    d = NULL;
    r = 2 - y / 120;
    c = x / 100 - 1;
    
    // deselect
    if (selection != NULL) {
        selection->selected = 0;
        selection = NULL;
    }
    
    // check turrets
    d = turretArray[r][c];
    if (d != NULL) {
        if (!((x > d->x - 50 && x < d->x + 50 && y > d->y - 10 && y <= d->y) ||
              (x > d->x - 15 && x < d->x + 15 && y > d->y && y < d->y + 15))) {
            d = NULL;
        }
    }
    if (d != NULL) {
        selection = d;
        d->selected = 1;
        return;
    }
    
    // check traps
    d = trapArray[r][c];
    if (d != NULL) {
        if (x > d->x - 50 && x < d->x + 50 && y > d->y - 10 && y <= d->y + 10) {
        } else {
            d = NULL;
        }
    }
    if (d != NULL) {
        selection = d;
        d->selected = 1;
        return;
    }
}

void placeDefense(int x, int y, DType type) {
    int r, c, price;
    price = defenseInfo[type].price;
    r = 2 - y / 120;
    c = x / 100 - 1;
    if (r < 0 || r > 2 || c < 0 || c > 7) {
        return;
    }
    if (price <= money) {
        if (type < GUN) {
            if (trapArray[r][c] == NULL) {
                trapArray[r][c] = createDefense(type, r, c, 0);
                money -= price;
            }
        } else {
            if (turretArray[r][c] == NULL) {
                turretArray[r][c] = createDefense(type, r, c, 0);
                money -= price;
            }
        }
    }
}

void aimDefense(void *vd, int targetX, int targetY, int targetSize) {
    Defense* d;
    float dx, dy;
    
    if (vd == NULL) {
        return;
    }
    d = vd;
    
    // calclulate where we should be aiming
    dx = targetX - d->x;
    dy = targetY - d->y;
    
    if (dy >= 0 && dy <= 100 && dx > -150 - targetSize && dx < 150 + targetSize) {
        if (dx == 0) {
            d->aim = M_PI / 2;
        } else if (dx > 0) {
            d->aim = atan(dy/dx);
        } else {
            d->aim = atan(dy/dx) + M_PI;
        }
    }
    
    moveDefense(d);
}

void moveDefense(void *vd) {
    Defense *d;
    if (vd == NULL) {
        return;
    }
    d = vd;
    // move the aim towards the target
    if (d->cur_aim > d->aim) {
        d->cur_aim -= d->aim_speed;
        if (d->cur_aim < d->aim) {
            d->cur_aim = d->aim;
        }
    } else if (d->cur_aim < d->aim) {
        d->cur_aim += d->aim_speed;
        if (d->cur_aim > d->aim) {
            d->cur_aim = d->aim;
        }
    }
}

void targetDefenses() {
    int i, j;
    Node *cur_e, *next_e;
    Defense *d;
    Enemy *e, *target;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            d = turretArray[i][j];
            if (d != NULL) {
                target = NULL;
                cur_e = eList->first;
                while(cur_e != NULL) {
                    next_e = cur_e->next;
                    e = cur_e->item;
                    if (e->floor == d->floor && ((e->x - e->size < d->x + 150) && (e->x + e->size > d->x - 150))) {
                        if (target == NULL) {
                            target = e;
                        } else {
                            if (e->floor == 1) {
                                if (e->x > target->x) {
                                    target = e;
                                }
                            } else {
                                if (e->x < target ->x) {
                                    target = e;
                                }
                            }
                        }
                    }
                    cur_e = next_e;
                }
                if (target != NULL) {
                    aimDefense(d, target->x, target->y, target->size);
                    if (fabs(d->cur_aim - d->aim) < 0.2) {
                        // don't fire unless we are almost aiming at our target
                        shootDefense(d);
                    }
                } else {
                    aimDefense(d, d->x, d->y + 10, 0);
                }
            }
        }
    }
}

void drawDefense(void *vd) {
    Defense *d;
    long long cur_time;
    int ax, ay;
    if (vd == NULL) {
        return;
    }
    d = vd;

    cur_time = getTime();
    //red, brown, blue, yellow, green, grey, dark_grey, grey_10, black, white;
    if (d->selected) {
        XSetForeground(display, lineGC, blue.pixel);
        XSetLineAttributes(display, lineGC, 2, LineSolid, CapButt, JoinMiter);
    }
    if (d->type < GUN) {
        if (d->type == MUD) {
            int i;
            XSetForeground(display, gc, brown.pixel);
            XFillRectangle(display, buffer, gc, d->x - 50, d->y, 101, 10);
            for (i = 0; i < 10; i++) {
                XFillArc(display, buffer, gc, d->x -50 + i*10, d->y - 5, 11, 11, 0, 360* 32);
            }
            for (i = 0; i < 10; i++) {
                XDrawArc(display, buffer, lineGC, d->x -50 + i*10, d->y - 5, 10, 10, 0, 360* 32);
            }
        } else if (d->type == ACID) {
            int i;
            XSetForeground(display, gc, green.pixel);
            XFillRectangle(display, buffer, gc, d->x - 50, d->y, 101, 10);
            for (i = 1; i < 5; i +=2) {
                XFillArc(display, buffer, gc, d->x -50 + i*20, d->y - 3, 21, 7, 0, 360* 32);
            }
            XSetForeground(display, gc, dark_grey.pixel);
            for (i = 0; i < 5; i += 2) {
                XFillArc(display, buffer, gc, d->x -50 + i*20, d->y - 4, 21, 7, 0, -360* 32);
            }
            for (i = 0; i < 5; i++) {
                if (i%2) {
                    XDrawArc(display, buffer, lineGC, d->x -50 + i*20, d->y - 3, 20, 6, 0, 360* 32);
                } else {
                    XDrawArc(display, buffer, lineGC, d->x -50 + i*20, d->y - 3, 20, 6, 0, -360* 32);
                }
            }
        } else if (d->type == SPIKE) {
            int i;
            int npoints = 21;
            XPoint points[npoints];
            for (i = 0; i < 21; i++) {
                points[i].x = d->x - 50 + i * 5;
                points[i].y = d->y - 10 * (i % 2);
            }
            XSetForeground(display, gc, grey.pixel);
            XFillRectangle(display, buffer, gc, d->x - 50, d->y, 101, 10);
            XFillPolygon(display, buffer, gc, points, npoints, Nonconvex, CoordModeOrigin);
            XDrawLines(display, buffer, lineGC, points, npoints, CoordModeOrigin);
        } else if (d->type == BEAR_TRAP) {
            if (d->triggered == 1 && abs(cur_time - d->last_shot_time) > d->reload_time) {
                trapArray[d->floor][d->col] = NULL;
                free(d);
                return;
            } else {
                int npoints = 9;
                XPoint leftPoints[] = {{d->x - 40, d->y + 5},
                                       {d->x - 40, d->y},
                                       {d->x - 35, d->y - 10},
                                       {d->x - 30, d->y},
                                       {d->x - 25, d->y},
                                       {d->x - 20, d->y - 5},
                                       {d->x - 15, d->y},
                                       {d->x - 5, d->y},
                                       {d->x - 5, d->y + 5}};
    
                XPoint rightPoints[] = {{d->x + 4, d->y + 5},
                                        {d->x + 5, d->y},
                                        {d->x + 15, d->y},
                                        {d->x + 20, d->y - 5},
                                        {d->x + 25, d->y},
                                        {d->x + 30, d->y},
                                        {d->x + 35, d->y - 10},
                                        {d->x + 40, d->y},
                                        {d->x + 40, d->y + 5}};
                rotatePoints(leftPoints, npoints, d->x - 5, d->y, d->cur_aim);
                rotatePoints(rightPoints, npoints, d->x + 5, d->y, -1 * d->cur_aim);
                XSetForeground(display, gc, black.pixel);
                XFillRectangle(display, buffer, gc, d->x - 50, d->y, 101, 10);
                XDrawLine(display, buffer, lineGC, d->x - 50, d->y, d->x + 50, d->y);
                XFillPolygon(display, buffer, lineGC, leftPoints, npoints, Nonconvex, CoordModeOrigin);
                XFillPolygon(display, buffer, lineGC, rightPoints, npoints, Nonconvex, CoordModeOrigin);
                XFillRectangle(display, buffer, lineGC, d->x - 5, d->y - 2, 10, 7);
    //            XDrawLines(display, buffer, lineGC, points, npoints, CoordModeOrigin);
            }
        } else if (d->type == TRAP_DOOR) {
            int npoints = 4;
            if (d->triggered == 1 && abs(cur_time - d->last_shot_time) > 1000) {
                d->triggered = 0;
                d->aim = 0;
            }
            XPoint leftPoints[] = {{d->x - 50, d->y},
                                   {d->x - 1, d->y},
                                   {d->x - 1, d->y + 5},
                                   {d->x - 50, d->y + 5}};
            XPoint rightPoints[] = {{d->x + 50, d->y},
                                    {d->x + 1, d->y},
                                    {d->x + 1, d->y + 5},
                                    {d->x + 50, d->y + 5}};
            XSetForeground(display, gc, dark_grey.pixel);
            XFillRectangle(display, buffer, gc, d->x - 50, d->y, 101, 10);
            
            rotatePoints(leftPoints, npoints, d->x - 50, d->y + 5, d->cur_aim);
            rotatePoints(rightPoints, npoints, d->x + 50, d->y + 5, -1 * d->cur_aim);
            
            XFillPolygon(display, buffer, lineGC, leftPoints, npoints, Nonconvex, CoordModeOrigin);
            XFillPolygon(display, buffer, lineGC, rightPoints, npoints, Nonconvex, CoordModeOrigin);
        }
        XDrawLine(display, buffer, lineGC, d->x - 50, d->y, d->x - 50, d->y + 10);
        XDrawLine(display, buffer, lineGC, d->x + 50, d->y, d->x + 50, d->y + 10);
    } else {
        int npoints = 5;
        XPoint barrelPoints[] = {{d->x - 2, d->y+5},
                                 {d->x + 2, d->y+5},
                                 {d->x + 2, d->y + 25},
                                 {d->x - 2, d->y + 25},
                                 {d->x - 2, d->y+5}};
        
        if (d->type == GUN) {
            XSetForeground(display, gc, blue.pixel);
        } else if (d->type == SHOTGUN) {
            XSetForeground(display, gc, yellow.pixel);
        } else if (d->type == MACHINE_GUN) {
            XSetForeground(display, gc, red.pixel);
        } else if (d->type == ROCKET) {
            XSetForeground(display, gc, black.pixel);
        }
        
        rotatePoints(barrelPoints, npoints, d->x, d->y + 5, d->cur_aim - M_PI_2);
        XFillPolygon(display, buffer, gc, barrelPoints, npoints, Convex, CoordModeOrigin);
        XDrawLines(display, buffer, lineGC, barrelPoints, npoints, CoordModeOrigin);

        XFillRectangle(display, buffer, gc, d->x - 50, d->y - 10, 101, 10);
        XDrawRectangle(display, buffer, lineGC, d->x - 50, d->y - 10, 100, 9);
        XFillArc(display, buffer, gc, d->x - 15, d->y - 15, 30, 30, 0, -360*32);
        XDrawArc(display, buffer, lineGC, d->x - 15, d->y - 15, 30, 30, 0, -360*32);
    }
    XSetForeground(display, lineGC, black.pixel);
    XSetLineAttributes(display, lineGC, 1, LineSolid, CapButt, JoinMiter);
}

void drawTurrets() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            drawDefense(turretArray[i][j]);
        }
    }
}

void moveTraps() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            moveDefense(trapArray[i][j]);
        }
    }
}

void drawTraps() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            drawDefense(trapArray[i][j]);
        }
    }
}

void shootDefense(void *vd) {
    Defense *d;
    Bullet *b;
    long long cur_time;
    if (vd == NULL) {
        return;
    }
    d = vd;
    
    cur_time = getTime();
//    fprintf(stderr, "cur: %d last: %d\n", cur_time, d->last_shot_time);
    if (abs(cur_time - d->last_shot_time) > d->reload_time) {
        d->last_shot_time = cur_time;
        if (d->type == GUN || d->type == MACHINE_GUN || d->type == ROCKET) {  
            b = createBullet(d->type, d->floor, d->x, d->y, d->cur_aim, d->bulletSpeed, d->damage);
            listAppend(bList, b);
        } else if (d->type == SHOTGUN) { 
            b = createBullet(d->type, d->floor, d->x, d->y, d->cur_aim, d->bulletSpeed, d->damage);
            listAppend(bList, b);
             
            b = createBullet(d->type, d->floor, d->x, d->y, d->cur_aim - 0.3, d->bulletSpeed, d->damage);
            listAppend(bList, b);
              
            b = createBullet(d->type, d->floor, d->x, d->y, d->cur_aim + 0.3, d->bulletSpeed, d->damage);
            listAppend(bList, b);
        }
    }
}

void checkTraps() {
    Node *cur, *next;
    Enemy *e;
    int r, c;
    cur = eList->first;
    while(cur != NULL) {
        next = cur->next;
        e = cur->item;
        if (e->flying == 0) {
            r = e->floor;
            c = e->x / 100 - 1;
            if (r >=0 && r < 3 && c >=0 && c < 8) {
                triggerTrap(trapArray[r][c], e);
            }
        }
        cur = next;
    }
}

void triggerTrap(void *vd, void *ve) {
    long long cur_time;
    Defense *d;
    Enemy *e;
    if (vd == NULL || ve == NULL) {
        return;
    }
    d = vd;
    e = ve;
    cur_time = getTime();
    if (d->type == MUD) {
        e->slow = 1;
        e->sTime = cur_time;
    } else if (d->type == ACID) {
        e->burn = 1;
        e->bTime = cur_time;
    } else if (d->type == SPIKE) {
        if (abs(cur_time - e->dTime) > e->damageInterval) {
            e->dTime = cur_time;
            e->health -= d->damage;
        }
    } else if (d->type == BEAR_TRAP) {
        if (d->triggered == 0) {
            if (abs(e->x - d->x) <10) {
                e->health -= d->damage;
                d->triggered = 1;
                d->aim = M_PI_2;
                d->last_shot_time = cur_time;
            }
        }
    } else if (d->type == TRAP_DOOR) {
        if (d->triggered == 0 && abs(e->x - d->x) < 10 && abs(cur_time - d->last_shot_time) > d->reload_time) {
            d->triggered = 1;
            d->aim = M_PI_4;
            d->last_shot_time = cur_time;
        } else if (d->triggered == 1) {
            e->health = 0;
        }
    }
}

Bullet* createBullet(DType type, int f, int x, int y, float a, int s, int d) {
    Bullet *b;
    b = (Bullet*)malloc(sizeof(Bullet));
    b->container = NULL;
    b->type = type;
    b->floor = f;
    b->x = x;
    b->y = y;
    b->a = a;
    b->s = s;
    b->r = 3;
    b->dmg = d;
    return b;
}

void moveBullet(void *vb) {
    int dx, dy;
    Bullet *b;
    b = vb;
    
    dx = b->s * cos(b->a);
    dy = b->s * sin(b->a);
    
    b->x += dx;
    b->y += dy;
}

void checkCollisions() {
    Node *cur_b, *next_b;
    Node *cur_e, *next_e;
    Bullet *b;
    Enemy *e;
    cur_b = bList->first;
    while (cur_b != NULL) {
        next_b = cur_b->next;
        b = cur_b->item;
        
        cur_e = eList->first;
        while(cur_e != NULL) {
            next_e = cur_e->next;
            e = cur_e->item;
            if (b->x > e->x - e->size && b->x < e->x + e->size && b->y > e->y - e->size && b->y < e->y + e->size) {
                e->health -= b->dmg;
                listRemove(bList, cur_b);
                break;
            }
            cur_e = next_e;
        }
        cur_b = next_b;
    }
}

void drawBullet(void *vb) {
    int i, npoints;
    Bullet *b;
    b = vb;
    if (b->type == GUN || b->type == SHOTGUN || b->type == MACHINE_GUN) {
        XSetForeground(display, gc, red.pixel);
        XFillArc(display, buffer, gc, b->x - b->r, b->y - b->r, b->r*2, b->r*2, 0, 360*64);
    } else if (b->type == ROCKET) {
        npoints = 7;
        XPoint points[] = {{b->x - 5, b->y - 10},
                          {b->x + 5, b->y - 10},
                          {b->x + 3, b->y - 8},
                          {b->x + 3, b->y - 3},
                          {b->x,     b->y},
                          {b->x - 3, b->y - 3},
                          {b->x - 3, b->y - 8}};
        // rotate
        rotatePoints(points, npoints, b->x, b->y, b->a - M_PI_2);
        
        XSetForeground(display, gc, black.pixel);
        XFillPolygon(display, buffer, gc, points, npoints, Nonconvex, CoordModeOrigin);
    }
}

void cleanupBullets() {
    Node *cur, *next;
    Bullet *b;
    int floorY, ceilY;
    cur = bList->first;
    while (cur != NULL) {
        next = cur->next;
        b = cur->item;
        ceilY = (2 - b->floor) * 120 + 10;
        floorY = (2 - b->floor) * 120 + 110;
        if (b->x < 0 || b->x > WIN_W || b->y < ceilY || b->y > floorY) {
            listRemove(bList, cur);
        }
        cur = next;
    }
}