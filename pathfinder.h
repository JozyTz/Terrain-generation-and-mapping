#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <limits.h>
#include "map_generator.h"



int32_t dMap_cmp(const void *key, const void *with);
int pathCost(Terrain terrainType, Mobs mobType);
void dijkstraMap(DistanceMap *dMap, WorldMap *world, int xCoord, int yCoord);

#endif