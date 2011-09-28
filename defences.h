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
    /* TRAPS */
    MUD,
    ACID,
    SPIKE,
    BEAR_TRAP,
    PROX_MINE,
    REMOTE_MINE,
    TRAP_DOOR,
    /* TURRETS */
    GUN,
    SHOTGUN,
    MACHINE_GUN,
    TASER,
    LASER,
    FLAMETHROWER,
    ROCKET
} DType;

typedef struct {
    // Type
    DType type;
    // Position
    int r;
    int c;
    // Misc properties
} Defense;


#endif /* _DEFENSES_H_ */