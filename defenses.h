/* 
 * File:    defenses.h
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Declares defense types and functions.
 */

#ifndef _DEFENSES_H_
#define _DEFENSES_H_

#include "list.h"

typedef enum {
    WOOD,
    BRICK,
    IRON,
    STEEL,
    HEALTH,                // not actually a castle, just an upgrade
    NUM_CASTLE             // for counting
} CType;

typedef struct {
    CType type;
    int maxHealth;
    int curHealth;
    int showHealth;
} Castle;

void upgradeCastle(CType type, Castle *c);
void drawCastle(Castle *c, int x, int y);

typedef enum {
    /* TRAPS */
    MUD,
    ACID,
    SPIKE,
    BEAR_TRAP,
    TRAP_DOOR,
    NUM_TRAPS,              // for counting
    /* TURRETS */
    GUN,
    SHOTGUN,
    MACHINE_GUN,
    ROCKET,
    //TASER,
    //LASER,
    //FLAMETHROWER,
    NUM_TURRETS_PLUS_GUN   // for counting
} DType;

typedef struct {
    // Type
    DType type;
    Node *container;
    int id;
    // Position
    int floor;
    int col;
    int x;
    int y;
    // misc paramaters
    int selected;
    int triggered;
    float aim;  // angle in radians
    float cur_aim;
    float aim_speed;
    int bulletSpeed;
    int damage;
    long long reload_time;    // in milliseconds
    long long last_shot_time; // in milliseconds
} Defense;

Defense* createDefense(DType type, int f, int c, int id);
void selectDefense(int x, int y);
void placeDefense(int x, int y, DType type);
void aimDefense(void *vd, int targetX, int targetY, int targetSize);
void moveDefense(void *vd);
void targetDefenses();
void drawDefense(void *vd);
void drawTurrets();
void moveTraps();
void drawTraps();
void checkTraps();
void triggerTrap(void *vd, void *ve);
void shootDefense(void *vd);

typedef struct {
    DType type;
    Node *container;
    int floor;
    int x;
    int y;
    float a;
    int s;
    int r;
    int dmg;
} Bullet;

Bullet* createBullet(DType type, int f, int x, int y, float a, int s, int d);
void moveBullet(void *vb);
void checkCollisions();
void drawBullet(void *vb);
void cleanupBullets();

#endif /* _DEFENSES_H_ */