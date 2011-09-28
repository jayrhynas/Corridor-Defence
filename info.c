/* 
 * File:    info.c
 * Author:  Jayson Rhynas, 20299211
 * Created: January 21, 2011
 *
 * Declares various information about the defenses
 */

#include "info.h"

Info defenseInfo[] = {
    {"Mud",
     {"Slows down enemies to half speed for 10 seconds",
      "",
      ""},
     100},
    {"Acid",
     {"Causes continuous damage to enemies",
      "Lasts 10 seconds",
      "Repeats every second"},
     100},
    {"Spikes",
     {"Causes low damage to all enemies that step on it",
      "",
      ""},
     100},
    {"Bear Trap",
     {"Causes high damage to one enemy",
      "One-time use",
      ""},
     100},
    {"Trap Door",
     {"Instantly kills all enemies above open door",
      "Very slow reload",
      ""},
     100},
    {"",
     {"", "", ""},
     0}, //Placeholder for NUM_TRAPS
    {"Gun",
     {"Basic gun",
      "Medium damage, medium reload time",
      ""},
     100},
    {"Shotgun",
     {"Shoots 3 bullets at a time",
      "Mid-low damage, mid-high reload time",
      ""},
     100},
    {"Machine Gun",
     {"High rate of fire, low damage",
      "",
      ""},
     100},
    {"Rocket",
     {"Slow moving missile, but deals high damage",
      "Slow reload time",
      ""},
     100},
    {"Wooden Door",
     {"Low health",
      "",
      ""},
     100},
    {"Brick Door",
     {"Medium health",
      "",
      ""},
     100},
    {"Iron Door",
     {"High health",
      "",
      ""},
     100},
    {"Steel Door",
     {"Very high health",
      "",
      ""},
     100},
    {"Repair",
     {"Repairs 500 damage to the door",
      "",
      ""},
     100},
    {"Racer",
     {"Fast moving ground enemy",
      "",
      ""},
     0},
    {"Crawler",
     {"Slow moving ground enemy",
      "",
      ""},
      0},
    {"Zoomer",
     {"Fast moving flying enemy",
      "",
      ""},
      0},
    {"Floater",
     {"Slow moving flying enemy",
      ""
      ""},
     0}};