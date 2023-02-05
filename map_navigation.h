#ifndef MAPNAV_H
#define MAPNAV_H

#include "pathfinder.h"


void initializeWorld(WorldMap *world);
void moveMaps(WorldMap *world, int xCoord, int yCoord);
void deleteWorld(WorldMap *world);
void wprint(WorldMap *world);
void distMapPrint(WorldMap *world);

#endif