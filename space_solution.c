#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//crt_planets = current planet ID
//*connections = array of integers. ID of planets you can go to
//num_connections = number of planets in array
//distance_from_mixer = crows flies distance to target planet
//*ship_state = storage structure

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int number_of_planets_visited; //number of overall visited planets
};

ShipAction space_hop(unsigned int crt_planet,
                   unsigned int *connections,
                   int num_connections,
                   double distance_from_mixer,
                   void *ship_state)
{
    printf("==============================================\n");
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("Current planet: %u\n", crt_planet);
    printf("Number of connections: %d\n", num_connections);

    //create a ship state if nothing exist. On spawn basically.
    if (ship_state == NULL){
        printf("Ship state is null\n");
        struct ship_state_struct *state = malloc(sizeof(struct ship_state_struct));
        state->number_of_planets_visited = 0;
        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    unsigned int_next_planet;

    //Scan surroundings and update database
    struct ship_state_struct *state = ship_state;
    state->number_of_planets_visited += 1;
    printf("Number of planets visited: %d\n",state->number_of_planets_visited);



    //Next Jump
    struct ship_action next_action = {int_next_planet, state};
    return next_action;
}
