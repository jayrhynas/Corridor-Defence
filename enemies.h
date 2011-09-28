/* 
 * File:    enemies.h
 * Author:  Jayson Rhynas, 20299211
 * Created: January 14, 2011
 *
 * Declares enemy types and functions.
 */

#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#include "list.h"

typedef enum {
    FAST,
    SLOW,
    FLYING_FAST,
    FLYING_SLOW,
    NUM_ENEMY_TYPES
} EType;

typedef struct {
    // Type
    EType type;
    Node *container;
    int id;
    int showHealth;
    // Velocity and position
    int x;
    int y;
    int dx;
    int dy;
    int size;
    int floor;
    // Misc properties
    int health;
    int maxHealth;
    int flying;
    int slow;
    long long sTime;
    int burn;
    long long bTime;
    long long lastBurn;
    int burnInterval;
    int maxEffectTime;
    long long dTime;
    int damageInterval;
    
    int damage;
    int dollarValue;
} Enemy;

Enemy* createEnemy(EType type, int x, int id);
void moveEnemy(void *ve);
void drawEnemy(void *ve);
void drawThumbEnemy(EType type, int x, int y);
void cleanupEnemies();

void createWave(EType type, int num);
void createRandomWave();
void sendWave();
void sendAndCreateWave();

#endif /* _ENEMIES_H_ */