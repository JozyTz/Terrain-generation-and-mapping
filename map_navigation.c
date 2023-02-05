#include "map_navigation.h"

int main(int argc, char *argv[])
{
    int x, y;
    int quitGame = 0;
    char userInput;

    WorldMap world;
    initializeWorld(&world);
    wprint(&world);
    printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);

    //Take user input to do game actions
    while(quitGame != 1)
    {
        scanf(" %c", &userInput);

        switch(userInput)
        {   
            case 'n':
                moveMaps(&world, world.coords.x, world.coords.y - 1);
                wprint(&world);
                printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);
                break;

            case 'e':
                moveMaps(&world, world.coords.x + 1, world.coords.y);
                wprint(&world);
                printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);
                break;

            case 's':
                moveMaps(&world, world.coords.x, world.coords.y + 1);
                wprint(&world);
                printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);
                break;

            case 'w':
                moveMaps(&world, world.coords.x - 1, world.coords.y);
                wprint(&world);
                printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);
                break;

            case 'f':
                scanf("%d %d", &x, &y);
                moveMaps(&world, x, y);
                wprint(&world);
                printf("World Coordinates: %d, %d\n\n\n", world.coords.x, world.coords.y);
                break;

            case 'd':
                distMapPrint(&world);
                break;

            case 'q':
                quitGame = 1;
                deleteWorld(&world);
                printf("GAME OVER\n");
                break;

            default:
                printf("Enter n, e, s, w, f, or q\n");
                break;
        }
    }
    printf("Shutting down\n");
}

/**
 *  Initialize the  WorldMap array to NULL. Create a map for the starting position
 */
void initializeWorld(WorldMap *world)
{
    //Initialize the entire world array as NULL
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            worldMapPos(i, j) = NULL;
        }
    }

    //Create a map at (200, 200) as the starting position
    world -> coords.x = 200;
    world -> coords.y = 200;
    worldMapPos(200, 200) = malloc(sizeof (*worldMapPos(world -> coords.x, world -> coords.y)));
    generateMap(world, 200, 200);
    placePlayerInit(world, worldMapPos(200, 200));
}

/**
 *  Move to a given map based on the given coordinates
 */
void moveMaps(WorldMap *world, int xCoord, int yCoord)
{
    //Do nothing if a map already exists for the given coordinates
    if (worldMapPos(xCoord, yCoord) != NULL && xCoord >= 0 && xCoord < WORLD_SIZE && yCoord >= 0 && yCoord < WORLD_SIZE)
    {
        world -> coords.x = xCoord;
        world -> coords.y = yCoord;
        return;
    }

    //Generate new map at given world coordinates
    if ((worldMapPos(xCoord, yCoord) == NULL) && xCoord >= 0 && xCoord < WORLD_SIZE && yCoord >= 0 && yCoord < WORLD_SIZE)
    {
        world -> coords.x = xCoord;
        world -> coords.y = yCoord;

        GameMap *curMap = worldMapPos(world -> coords.x, world -> coords.y) = (GameMap*) malloc(sizeof (GameMap));
        memset(curMap, 0, sizeof(GameMap));
        generateMap(world, xCoord, yCoord);
    }

    //Error message if coordinates are out of bounds
    else
    {
        printf("Coordinates out of bounds, use x and y values between 0 and %d\n", WORLD_SIZE - 1);
    }
}

/**
 *  Frees the memory used by the WorldMaps effectively deleting the world
 */
void deleteWorld(WorldMap *world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (world -> worldMap[i][j] != NULL)
            {
                free(world -> worldMap[i][j]);
            }
        }
    }
}

/**
 *  Call to the printMap function mapping a WorldMap pointer
 */
void wprint(WorldMap *world)
{
    printMap(worldMapPos(world -> coords.x, world -> coords.y));
}

void distMapPrint(WorldMap *world)
{
    DistanceMap *hikerMap = (DistanceMap*) malloc(sizeof (DistanceMap));
    memset(hikerMap, 0, sizeof(DistanceMap));
    hikerMap -> mobType = HIKER;

    DistanceMap *rivalMap = (DistanceMap*) malloc(sizeof (DistanceMap));
    memset(rivalMap, 0, sizeof(DistanceMap));
    rivalMap -> mobType = RIVAL;

    printf(GREEN "\n\nHiker distance map:\n" RESET);
    dijkstraMap(hikerMap, world, world -> coords.x, world -> coords.y);
    printf(GREEN "\nRival distance map:\n" RESET);
    dijkstraMap(rivalMap, world, world -> coords.x, world -> coords.y);
    wprint(world);

    free(hikerMap);
    free(rivalMap);
}