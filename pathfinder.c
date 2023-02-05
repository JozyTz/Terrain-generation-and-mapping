#include "pathfinder.h"



//Idk what this is but its in the example code and is used to init the heap..lol
int32_t dMap_cmp(const void *key, const void *with)
{
  return ((MapTile *) key)->cost - ((MapTile *) with)->cost;
}

//Function to determine terrain cell cost
int pathCost(Terrain terrainType, Mobs mobType)
{
    if(mobType == HIKER)
    {
        switch(terrainType)
        {
            case BOULDER :
                return INT_MAX;
            case TREE :
                return INT_MAX;
            case PATH :
                return 10;
            case POKEMART : 
                return 50;
            case POKECENTER : 
                return 50;
            case TALLGRASS : 
                return 15;
            case CLEARING : 
                return 10;
            case MOUNTAIN : 
                return 15;
            case FOREST : 
                return 15;
            case ENTRANCE : 
                return INT_MAX;
            default:
                return INT_MAX;
        }    
    }
   if(mobType == RIVAL)
   {
        switch(terrainType)
        {
            case BOULDER :
                return INT_MAX;
            case TREE :
                return INT_MAX;
            case PATH :
                return 10;
            case POKEMART : 
                return 50;
            case POKECENTER : 
                return 50;
            case TALLGRASS : 
                return 20;
            case CLEARING : 
                return 10;
            case MOUNTAIN : 
                return INT_MAX;
            case FOREST : 
                return INT_MAX;
            case ENTRANCE : 
                return INT_MAX;
            default:
                return INT_MAX;
        }   
   }
   if(mobType == PLAYERCHAR)
   {
        switch(terrainType)
        {
            case BOULDER :
                return INT_MAX;
            case TREE :
                return INT_MAX;
            case PATH :
                return 10;
            case POKEMART : 
                return 10;
            case POKECENTER : 
                return 10;
            case TALLGRASS : 
                return 20;
            case CLEARING : 
                return 10;
            case MOUNTAIN : 
                return INT_MAX;
            case FOREST : 
                return 20;
            case ENTRANCE : 
                return 10;
            default:
                return INT_MAX;
        }   
   }
   return INT_MAX;
}

void dijkstraMap(DistanceMap *dMap, WorldMap *world, int xCoord, int yCoord)
{
    GameMap *map = worldMapPos(xCoord, yCoord);
    heap_t h;
    MapTile *d;
    int x, y;
    int neighbor[2][8] = {{-1, 0, 1, -1, 1, -1, 0, 1}, {-1, -1, -1, 0, 0, 1, 1, 1}};


    //Initialize distanceMap coords and cost
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            dMap -> distanceMap[y][x].coords.y = y;
            dMap -> distanceMap[y][x].coords.x = x;
            dMap -> distanceMap[y][x].cost = INT_MAX;
        }
    }

    //Set player position cost to 0
    dMap -> distanceMap[world -> player.coords.y][world -> player.coords.x].cost = 0;

    heap_init(&h, dMap_cmp, NULL);

    //Add all valid nodes to heap
    for (y = 1; y < HEIGHT - 1; y++)
    {
        for (x = 1; x < WIDTH - 1; x++)
        {
            dMap -> distanceMap[y][x].node = heap_insert(&h, &dMap -> distanceMap[y][x]);
        }
    }

    //Dijkstras algo
    while(h.size)
    {
        d = heap_remove_min(&h);

        if(d -> cost == INT_MAX)
        {
            continue;
        }

        //Check each neighbor of node cost
        for(int i = 0; i < 8; i++)
        {
            if(dMap -> distanceMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]].node)
            {
                if(pathCost(map -> gameMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]], dMap -> mobType) != INT_MAX)
                {
                    if(dMap -> distanceMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]].cost > d -> cost + pathCost(map -> gameMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]], dMap -> mobType))
                    {
                        int newCost = d -> cost + pathCost((map -> gameMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]]), dMap -> mobType);
                        dMap -> distanceMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]].cost = newCost;
                        heap_decrease_key_no_replace(&h, dMap -> distanceMap[d -> coords.y + neighbor[1][i]][d -> coords.x + neighbor[0][i]].node);
                    }
                }
            }
        }
    }

    //Print out distanceMap
    for(y = 0; y < HEIGHT; y++)
    {
        for(x = 0; x < WIDTH; x++)
        {
            if(dMap -> distanceMap[y][x].cost != INT_MAX)
            {
	           printf("%02d", dMap -> distanceMap[y][x].cost % 100);
            }
            else
            {
	           printf("  ");
            }
        printf(" ");
        }
    printf("\n");
    }
}