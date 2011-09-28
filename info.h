/* 
 * File:    info.h
 * Author:  Jayson Rhynas, 20299211
 * Created: January 21, 2011
 *
 * Declares various information about the defenses
 */

#ifndef _INFO_H_
#define _INFO_H_

typedef struct {
    char *name;
    char *desc[3];
    int price;
} Info;

extern Info defenseInfo[];
     
#endif /* _INFO_H_ */