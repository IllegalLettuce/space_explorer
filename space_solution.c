#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define RANDOM_JUMP_AMOUNT 5

struct planet_storage {

};

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int number_of_planets_visited;                  //number of overall visited planets
    unsigned int *planets_visited;                  //Pointer to array of visited planets
    int first_random_jumps_amount;                  //Amount of random jumps storage (this number is decremented during the sweep)
    int num_connections_count;                      //Number of connections in current sweep
    unsigned int *connections_data;                 //Data of the neighbouring connections
    double *distance_of_connections_array;          //Data of the distance to the mixer for each sweep
};

ShipAction space_hop(unsigned int crt_planet,       //Current planet
                   unsigned int *connections,       //Pointer to array of integers. ID of planets you can go to
                   int num_connections,             //number of planets in array - number of neighbour planets
                   double distance_from_mixer,      //crows flies distance to target planet
                   void *ship_state)                //pointer to storage structure
{
    printf("==============================================\n");
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("Current planet: %u\n", crt_planet);
    printf("Number of connections: %d\n", num_connections);

    //create a ship state if nothing exist. On spawn basically.
    if (ship_state == NULL){
        printf("Ship state is null\n");
        struct ship_state_struct *state = malloc(sizeof(struct ship_state_struct));
        state->first_random_jumps_amount = RANDOM_JUMP_AMOUNT;
        state->connections_data = malloc(RANDOM_JUMP_AMOUNT * sizeof(unsigned int));
        state->num_connections_count = RANDOM_JUMP_AMOUNT;
        state->number_of_planets_visited = 1;
        state->planets_visited = malloc((state->number_of_planets_visited)* sizeof(unsigned int));
        state->planets_visited[0] = crt_planet;

        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;

    }
    //Refresh database
    struct ship_state_struct *state = ship_state;

    //The planets_visited
    state->number_of_planets_visited += 1;
    printf("Number of planets visited: %d\n",state->number_of_planets_visited);
    unsigned int *planets_visited_temp;
    planets_visited_temp = realloc(state->planets_visited,
                                   (state->number_of_planets_visited) * sizeof(unsigned int));
    if (planets_visited_temp != NULL){
        state->planets_visited = planets_visited_temp;
    }else{
        printf("Something went wrong with the realloc for planets_visited;\n");
    }


    unsigned int next_planet;

    //Random jump logic
    if (state->first_random_jumps_amount > -1){
        if (state->first_random_jumps_amount == RANDOM_JUMP_AMOUNT){
            state->distance_of_connections_array = malloc(RANDOM_JUMP_AMOUNT * sizeof(unsigned int));
        }
        state->distance_of_connections_array[RANDOM_JUMP_AMOUNT-(state->first_random_jumps_amount)]=distance_from_mixer;

        state->connections_data[RANDOM_JUMP_AMOUNT-(state->first_random_jumps_amount)]=crt_planet;
        printf("%d\n", state->first_random_jumps_amount);
        state->first_random_jumps_amount--;
        printf("RANDOM JUMP %d\n", state->first_random_jumps_amount);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    printf("Check\n");
    double lowest_distance = 10.5;
    int index_of_lowest_distance = 0;

    for (int index = 0; index < state->num_connections_count; index++){
        if (state->distance_of_connections_array[index] > lowest_distance){
            lowest_distance = state->distance_of_connections_array[index];
            index_of_lowest_distance = index;
        }
    }

    next_planet = state->connections_data[index_of_lowest_distance];




    //Dollar store Dijkstra's algo
    //From each planet, save connections and num_connections check
    //Substructure????
    //N random jumps (optimise)
    //Jump through each connection in order and save distances to array
    //index of array of distances correspond to array of connections
    //find the shortest distance
    //jump to planet
    //repeat







    //Next Jump
    struct ship_action next_action = {next_planet, state};
    return next_action;
}
