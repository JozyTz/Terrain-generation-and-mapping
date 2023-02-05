#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "heap.h"


// COLORS
#define B_RED   "\033[31;7;1m"   //Pokecenter
#define B_BLUE  "\033[34;7;1m"   //Pokemart
#define WHITE   "\033[2;7;30m"     //Path
#define GRAY    "\033[2;7;37m"   //Boulder
#define GREEN   "\033[7;32m"     //Tallgrass
#define YELLOW  "\033[7;33m"     //Clearing
#define B_GREEN "\033[7;32m"     //Tree
#define RESET "\e[0m"

// CONSTANTS
#define WIDTH 80
#define HEIGHT 21
#define NUM_REGIONS 7
#define WORLD_SIZE 401

// STRUCTURES
typedef enum terrain
{
  POKECENTER,
  POKEMART,
  PATH,
  TALLGRASS,
  CLEARING,
  BOULDER,
  TREE,
  MOUNTAIN,
  FOREST,
  ENTRANCE,
  EMPTY,
  PLAYER,
} Terrain;

//Coordinate set for world/game maps
typedef struct coordinates
{
  int x;
  int y;
} Coordinates;

typedef enum mobs
{
  HIKER,
  RIVAL,
  PLAYERCHAR,
} Mobs;

typedef struct player
{
  Coordinates coords;
} Player;

//Used for dijkstras, represents a single node in the gameMap grid
typedef struct mapTile
{
  int cost;
  bool visited;
  heap_node_t *node;
  Coordinates coords;
} MapTile;

//Store array of map nodes and mobtype
typedef struct distMap
{
  MapTile distanceMap[HEIGHT][WIDTH];
  Mobs mobType;
} DistanceMap;

//Map struct
typedef struct gMap
{
  Terrain gameMap[HEIGHT][WIDTH];
  int northEntrance[2];
  int eastEntrance[2];
  int southEntrance[2];
  int westEntrance[2];
  int regionSeeds[NUM_REGIONS][2];
} GameMap;

//World struct
typedef struct wMap
{
  GameMap *worldMap[WORLD_SIZE][WORLD_SIZE];
  Coordinates coords;
  Player player;
} WorldMap;


//Functions
void generateMap(WorldMap *world, int xCoord, int yCoord);
void mapBaseCreator(GameMap *map);
void createBorder(GameMap *map);
void plantRegionSeeds(GameMap *map);
void generateRegions(GameMap *map, int distance);
void generateRegionsIterator(GameMap *map, int x1, int y1, int distance, char type);
int  numberInBound(int lowerBound, int upperBound);
float distanceBetweenPoints(int x1, int y1, int x2, int y2);
float probabilityHelper(WorldMap *world);
void createEntrances(WorldMap *world, int xCoord, int yCoord);
void placeEntrances(GameMap *map);
void placePlayerInit(WorldMap *world, GameMap *map);
void createPaths(GameMap *map);
void buildPathBetweenPoints(GameMap *map, int point1[], int point2[], char pathType);
bool spaceOccupied(GameMap *map, int xCoord, int yCoord);
void placeBuilding(GameMap *map, Terrain buildingType);
void addRandomObjects(GameMap *map);
void printMap(GameMap *map);
void printMapColorHelper(Terrain type);

void initializeWorld(WorldMap *world);
void moveMaps(WorldMap *world, int xCoord, int yCoord);
void deleteWorld(WorldMap *world);
void wprint(WorldMap *world);

//Helper functions
#define gameMapPos(x, y) (map -> gameMap[y][x])
#define worldMapPos(x, y) (world -> worldMap[y][x])

#define malloc(size) ({        \
void *_tmp;                    \
assert((_tmp = malloc(size))); \
_tmp;                          \
})

#endif
