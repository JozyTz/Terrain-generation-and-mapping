#include "map_generator.h"

/**
 *   Create a map, populate it with components, print out the map
 */
void generateMap(WorldMap *world, int xCoord, int yCoord)
{
    srand(time(NULL));
    rand();
    mapBaseCreator(worldMapPos(xCoord, yCoord));
    plantRegionSeeds(worldMapPos(xCoord, yCoord));
    createBorder(worldMapPos(xCoord, yCoord));
    createEntrances(world, xCoord, yCoord);
    createPaths(worldMapPos(xCoord, yCoord));
    if (numberInBound(0, 100) <= probabilityHelper(world))
    {
        placeBuilding(worldMapPos(xCoord, yCoord), POKECENTER);
    }
    if (numberInBound(0, 100) <= probabilityHelper(world))
    {
        placeBuilding(worldMapPos(xCoord, yCoord), POKEMART);
    }
    addRandomObjects(worldMapPos(xCoord, yCoord));
}

/**
 *   Initialize the map array as EMPTY characters
 */
void mapBaseCreator(GameMap *map)
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
    	{
    	    map -> gameMap[i][j] = EMPTY;
    	}
    }
}

/**
 *   Create a border of BOULDERs around the map
 */
void createBorder(GameMap *map)
{
    for (int i = 0; i < HEIGHT; i++)
    {
    	map -> gameMap[i][0] = BOULDER;
    	map -> gameMap[i][WIDTH - 1] = BOULDER;
    }
    for (int i = 0; i < WIDTH; i++)
    {
    	map -> gameMap[0][i] = BOULDER;
    	map -> gameMap[HEIGHT - 1][i] = BOULDER;
    }
}

/**
 *   Plant biome seeds based on the NUM_REGIONS and the biome weighting. Call generateRegions function
 *      which grows the seeds.
 */
void plantRegionSeeds(GameMap *map)
{
    //region weighting
    int regionNum = NUM_REGIONS;
    int numForests = regionNum / 5;
    regionNum -= numForests;
    int numMountains = regionNum / 5;
    regionNum -= numMountains;
    int numTallGrass = regionNum / 2;
    regionNum -= numTallGrass;
    int numClearings = regionNum;
    
    //region seed creation/planting
    for (int i = 0; i < NUM_REGIONS; i++)
    {
        if (numTallGrass > 0)
	{
	    map -> regionSeeds[i][0] = numberInBound(0, HEIGHT - 1);
	    map -> regionSeeds[i][1] = numberInBound(0, WIDTH - 1);
	    map -> gameMap[map -> regionSeeds[i][0]][map -> regionSeeds[i][1]] = TALLGRASS;
	    numTallGrass -= 1;
	}
        else if (numClearings > 0)
	{
	    map -> regionSeeds[i][0] = numberInBound(0, HEIGHT - 1);
	    map -> regionSeeds[i][1] = numberInBound(0, WIDTH - 1);
	    map -> gameMap[map -> regionSeeds[i][0]][map -> regionSeeds[i][1]] = CLEARING;
	    numClearings -= 1;
	}
	else if (numForests > 0)
	{
	    map -> regionSeeds[i][0] = numberInBound(0, HEIGHT - 1);
	    map -> regionSeeds[i][1] = numberInBound(0, WIDTH - 1);
	    map -> gameMap[map -> regionSeeds[i][0]][map -> regionSeeds[i][1]] = FOREST;
	    numForests -= 1;
	}
	else if (numMountains > 0)
	{
	    map -> regionSeeds[i][0] = numberInBound(0, HEIGHT - 1);
	    map -> regionSeeds[i][1] = numberInBound(0, WIDTH - 1);
	    map -> gameMap[map -> regionSeeds[i][0]][map -> regionSeeds[i][1]] = MOUNTAIN;
	    numMountains -= 1;
	}
    }
    generateRegions(map, 1);
}

/**
 *   Expand each region by 1 distance recursively (from the seed) until the region runs out of EMPTY nodes to
 *       spread to, or until the max region size is met.
 */
void generateRegions(GameMap *map, int distance)
{
    //expand each region by 1 distance
    for (int i = 0; i < NUM_REGIONS; i++)
    {
       generateRegionsIterator(map, map -> regionSeeds[i][0], map -> regionSeeds[i][1], distance, map -> gameMap[map -> regionSeeds[i][0]][map -> regionSeeds[i][1]]);
    }

    //max region size set, any EMPTY spaces left are filled with CLEARINGs
    if (distance < 15)
    {
        distance++;
	generateRegions(map, distance);
    }
    else
    {
    	for (int i = 0; i < HEIGHT; i++)
    	{
    	    for (int j = 0; j < WIDTH; j++)
    	    {
        		if (map -> gameMap[i][j] == EMPTY)
        		{
        		    map -> gameMap[i][j] = CLEARING;
        		}
    	    }
    	}
    }
}

/**
 *   Fill each EMPTY node within a set distance from the given coordinates with the given character type
 */
void generateRegionsIterator(GameMap *map, int x1, int y1, int distance, char type)
{
    for (int i = 0; i < HEIGHT; i++)
    {
    	for (int j = 0; j < WIDTH; j++)
    	{
    	    if (distanceBetweenPoints(i, j, x1, y1) <= distance && map -> gameMap[i][j] == EMPTY)
    	    {
        		map -> gameMap[i][j] = type;
    	    }
    	}
    }
}

/**
 *   Get a random number between two bounds
 */
int numberInBound(int lowerBound, int upperBound)
{
    if (upperBound - lowerBound + 1 != 0)
    {
    	return (rand() % (upperBound - lowerBound + 1)) + lowerBound;
    }
    else
    {
    	return (rand() % (upperBound - lowerBound)) + lowerBound;
    }
}

/**
 *  Get a distance between two points
 */
float distanceBetweenPoints(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/**
 *  Get a probability to use for an event (in our case building spawn chance)
 * TODO
 */
float probabilityHelper(WorldMap *world)
{
    return 100 * pow((0.992 - (1 / WORLD_SIZE)), distanceBetweenPoints(world -> coords.x, world -> coords.y, WORLD_SIZE / 2, WORLD_SIZE / 2));
}

/**
 *   Call to create an entrance location on each side of a map
 */
void createEntrances(WorldMap *world, int xCoord, int yCoord)
{
    int northEntrancePositionX = numberInBound(6, WIDTH - 6); //6-74
    int eastEntrancePositionY = numberInBound(4, HEIGHT - 5);//4-16
    int southEntrancePositionX = numberInBound(6, WIDTH - 6); //6-74
    int westEntrancePositionY = numberInBound(4, HEIGHT - 5);//4-16

    //North entrance
    if (yCoord > 0)
    {
        worldMapPos(xCoord, yCoord) -> northEntrance[1] = northEntrancePositionX;
        worldMapPos(xCoord, yCoord) -> northEntrance[0] = 0;
        if (worldMapPos(xCoord, yCoord - 1) != NULL)
        {
            worldMapPos(xCoord, yCoord) -> northEntrance[1] = worldMapPos(xCoord, yCoord - 1) -> southEntrance[1];
        }
    }

    //East entrance
    if (xCoord < WORLD_SIZE - 1)
    {
        worldMapPos(xCoord, yCoord) -> eastEntrance[1] = WIDTH - 1;
        worldMapPos(xCoord, yCoord) -> eastEntrance[0] = eastEntrancePositionY;
        if (worldMapPos(xCoord + 1, yCoord) != NULL)
        {
            worldMapPos(xCoord, yCoord) -> eastEntrance[0] = worldMapPos(xCoord + 1, yCoord) -> westEntrance[0];
        }
    }

    //South entrance
    if (yCoord < WORLD_SIZE - 1)
    {
        worldMapPos(xCoord, yCoord) -> southEntrance[1] = southEntrancePositionX;
        worldMapPos(xCoord, yCoord) -> southEntrance[0] = HEIGHT - 1;
        if (worldMapPos(xCoord, yCoord + 1) != NULL)
        {
            worldMapPos(xCoord, yCoord) -> southEntrance[1] = worldMapPos(xCoord, yCoord + 1) -> northEntrance[1];
        }
    }

    //West entrance
    if (xCoord > 0)
    {
        worldMapPos(xCoord, yCoord) -> westEntrance[1] = 0;
        worldMapPos(xCoord, yCoord) -> westEntrance[0] = westEntrancePositionY;
        if (worldMapPos(xCoord - 1, yCoord) != NULL)
        {
            worldMapPos(xCoord, yCoord) -> westEntrance[0] = worldMapPos(xCoord - 1, yCoord) -> eastEntrance[0];
        }
    }

    //placeEntrances(worldMapPos(xCoord, yCoord));
}

// void placeEntrances(GameMap *map)
// {
//     gameMapPos(map -> northEntrance[1], map -> northEntrance[0]) = ENTRANCE;
//     gameMapPos(map -> eastEntrance[1], map -> eastEntrance[0]) = ENTRANCE;
//     gameMapPos(map -> southEntrance[1], map -> southEntrance[0]) = ENTRANCE;
//     gameMapPos(map -> westEntrance[1], map -> westEntrance[0]) = ENTRANCE;
// }

void placePlayerInit(WorldMap *world, GameMap *map)
{
    while(1)
    {
        world -> player.coords.x = numberInBound(10, WIDTH - 11);
        world -> player.coords.y = numberInBound(5, HEIGHT - 5);
        if (gameMapPos(world -> player.coords.x, world -> player.coords.y) == PATH)
        {
            gameMapPos(world -> player.coords.x, world -> player.coords.y) = PLAYER;
            break;
        }
    }
}

/**
 *   Create a connection point for the N/S annd E/W path to meet. Call to buildPathBetweenPoints function for each direction.
 */
void createPaths(GameMap *map)
{
    //Intersection beetween n/s and e/w paths
    int connectionPoint[2] = {numberInBound(5, HEIGHT - 6), numberInBound(8, WIDTH - 8)};
    map -> gameMap[connectionPoint[0]][connectionPoint[1]] = PATH;

    //Build N/S path
    if (map -> northEntrance[1] != 0)
    {
        buildPathBetweenPoints(map, map -> northEntrance, connectionPoint, PATH);
        gameMapPos(map -> northEntrance[1], map -> northEntrance[0]) = ENTRANCE;
    }
    if (map -> southEntrance[0] == HEIGHT - 1)
    {
        buildPathBetweenPoints(map, map -> southEntrance, connectionPoint, PATH);
        gameMapPos(map -> southEntrance[1], map -> southEntrance[0]) = ENTRANCE;
    }

    //Build E/W path
    if (map -> eastEntrance[1] == WIDTH - 1)
    {
        buildPathBetweenPoints(map, map -> eastEntrance, connectionPoint, PATH);
        gameMapPos(map -> eastEntrance[1], map -> eastEntrance[0]) = ENTRANCE;
    }
    if (map -> westEntrance[0] != 0)
    {
        buildPathBetweenPoints(map, map -> westEntrance, connectionPoint, PATH);
        gameMapPos(map -> westEntrance[1], map -> westEntrance[0]) = ENTRANCE;
    }
}

/**
 * Builds a path betweeen two given points
 * point1[] is starting point, point2[] is destination point, pathType is the road material
 */
void buildPathBetweenPoints(GameMap *map, int *point1, int *point2, char pathType)
{
    //Init variables
    int shortestPoint[2] = {point1[0], point1[1]};
    int shortestDistance = 999;
    
    //Distances for each direction to move
    double moveNorth = sqrt((pow(point1[0] - 1 - point2[0], 2)) + (pow(point1[1] - point2[1], 2)));
    double moveSouth = sqrt((pow(point1[0] + 1 - point2[0], 2)) + (pow(point1[1] - point2[1], 2)));
    double moveEast = sqrt((pow(point1[0] - point2[0], 2)) + (pow(point1[1] + 1 - point2[1], 2)));
    double moveWest = sqrt((pow(point1[0] - point2[0], 2)) + (pow(point1[1] - 1 - point2[1], 2)));

    //Find which move brings the epath closer to the target, update the shortestDistance and shortestPoint variables
    if (point1[0] - 1 > 0 && point1[1] > 0 && point1[1] < WIDTH - 1 && moveNorth < shortestDistance)
    {
    	shortestDistance = moveNorth;
    	shortestPoint[0] = point1[0] - 1;
    	shortestPoint[1] = point1[1];
    }
 
    if (point1[0] + 1 < HEIGHT - 1 && point1[1] > 0 && point1[1] < WIDTH - 1 && moveSouth < shortestDistance)
    {
    	shortestDistance = moveSouth;
    	shortestPoint[0] = point1[0] + 1;
    	shortestPoint[1] = point1[1];
    }
    
    if (point1[0] < HEIGHT - 1 && point1[1] + 1 < WIDTH - 1 && point1[0] > 0 && moveEast < shortestDistance)
    {
    	shortestDistance = moveEast;
    	shortestPoint[0] = point1[0];
    	shortestPoint[1] = point1[1] + 1;
    }

    if (point1[0] < HEIGHT - 1 && point1[1] - 1 > 0 && point1[0] > 0 && moveWest < shortestDistance)
    {
    	shortestDistance = moveWest;
    	shortestPoint[0] = point1[0];
    	shortestPoint[1] = point1[1] - 1;
    }

    //Iterate until the target is reached
    if (shortestPoint[0] == point2[0] && shortestPoint[1] == point2[1])
    {
    	map -> gameMap[point2[0]][point2[1]] = pathType;
    	return;
    }
    map -> gameMap[shortestPoint[0]][shortestPoint[1]] = pathType;
    buildPathBetweenPoints(map, shortestPoint, point2, pathType);
}

/**
 *   Check whether a building already exists in a 7x7 area surrounding given coordinates
 */
bool spaceOccupied(GameMap *map, int xCoord, int yCoord)
{
    for (int k = xCoord - 3; k <= xCoord + 3; k++)
    {
    	for (int l = yCoord - 3; l <= yCoord + 3; l++)
    	{
    	    if (map -> gameMap[k][l] == POKECENTER || map -> gameMap[k][l] == POKEMART)
    	    {
        		return true;
    	    }
    	}
    }
    return false;
}

/**
 * Places a given building along the path
 */
void placeBuilding(GameMap *map, Terrain buildingType)
{
    while(1)
    {
    	int i = numberInBound(3, HEIGHT - 4);
    	int j = numberInBound(3, WIDTH - 3);
    	
    	if (map -> gameMap[i][j] == PATH && spaceOccupied(map, i, j) == false)
    	{
    	    for (int k = i - 2; k <= i + 2; k++)
    	    {
        		for (int l = j - 2; l <= j + 2; l++)
        		{
        		    if (map -> gameMap[k][l] != PATH && map -> gameMap[k + 1][l] != PATH && map -> gameMap[k][l + 1] != PATH && map -> gameMap[k + 1][l + 1] != PATH)
        		    {
            			map -> gameMap[k][l] = buildingType;
            			map -> gameMap[k + 1][l] = buildingType;
            			map -> gameMap[k][l + 1] = buildingType;
            			map -> gameMap[k + 1][l + 1] = buildingType;
            			
            			//edge case fix for diagonals
            			if (map -> gameMap[k + 2][l + 1] == TALLGRASS || map -> gameMap[k + 2][l + 1] == MOUNTAIN|| map -> gameMap[k + 2][l + 1] == BOULDER)
            			{
            			    map -> gameMap[k + 2][l + 1] = CLEARING;
            			}
            			if (map -> gameMap[k + 2][l] == TALLGRASS || map -> gameMap[k + 2][l] == MOUNTAIN || map -> gameMap[k + 2][l] == BOULDER)
            			{
            			    map -> gameMap[k + 2][l] = CLEARING;
            			}
            			return;
        		    }
        		}
    	    }
    	}
    }
}

/**
 *   Adds filler objects TREEs and BOULDERs randomly around the map
 */
void addRandomObjects(GameMap *map)
{
    int i, j, odds;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (map -> gameMap[i][j] != PATH && map -> gameMap[i][j] != POKEMART && map -> gameMap[i][j] != POKECENTER && map -> gameMap[i][j] != BOULDER && map -> gameMap[i][j] != ENTRANCE)
            {
                odds = rand() % 70;

                if (odds == 1) 
                {
                    map -> gameMap[i][j] = TREE;
                }
                else if (odds == 2)
                {
                    map -> gameMap[i][j] = BOULDER;
                }
            }
        }
    }
}

/**
 *   Prints the gameMap array to the screen
 */
void printMap(GameMap *map)
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            printMapColorHelper(map -> gameMap[i][j]);
            printf(RESET);
        }
        printf("\n");
    }
}

/**
 *   Helper to print array characters in color
 */
void printMapColorHelper(Terrain type)
{
    switch(type)
    {
	    case POKEMART :
	        printf(B_BLUE "M");
            return;
        case POKECENTER :
            printf(B_RED "C");
            return;
	    case PATH : 
            printf(WHITE "#");
            return;
        case BOULDER :
            printf(GRAY "%%");
            return;
        case MOUNTAIN :
            printf(GRAY "%%");
            return;
	    case TALLGRASS :
            printf(GREEN ":");
            return;
        case CLEARING :
            printf(YELLOW ".");
            return;
        case TREE :
            printf(B_GREEN "^");
            return;
        case FOREST :
            printf(B_GREEN "^");
            return;
        case ENTRANCE :
            printf(WHITE "#");
            return;
        case PLAYER :
            printf("@");
            return;
        default:
            printf("E");
    }
}