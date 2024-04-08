#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//crt_planets = current planet ID
//*connections = array of integers. ID of planets you can go to
//num_connections = number of planets in array
//distance_from_mixer = crows flies distance to target planet
//*ship_state = storage structure

//

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

    struct ship_state_struct data = {1};

    if (ship_state == NULL){
        printf("Ship state is null\n");
        struct ship_state_struct *state = &data;
        ship_state = state;
        struct ship_action next_action = {RAND_PLANET,ship_state};
        return next_action;
    }else{
        struct ship_state_struct *state = ship_state;
        state->number_of_planets_visited++;
        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET,state};
        return next_action;
    }
}
