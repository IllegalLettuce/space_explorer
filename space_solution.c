#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TARGET_DISTANCE 7

//Checks if the planet is in the array. Returns index if it is, and -1 if not
int is_planet_in_array(const unsigned int *array, int array_len, double planet_id){
    for (int index = 0; index < array_len; index++){
        if (array[index] == planet_id){
            return index;
        }
    }
    return -1;
}

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int start;                                      //Start pseudo boolean

    int number_of_planets_visited;                  //number of overall visited planets
    unsigned int *planets_visited;                  //Pointer to array of visited planets
    double *distances_of_planets_visited;           //Pointer to array of distances of visited planets

    int jump_logic;                                 //Jump logic pseudo bool
    int index;                                      //index for sweep
    unsigned int *connections_to_check;             //Array of connections for the sweep
    int num_connections_to_check;                   //Number of connections to sweep
    double *distances_of_connections;               //distances of the planets in the sweep
    unsigned int second_lowest_planet;
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

        state->start = 1;
        state->jump_logic = 0;
        state->index = 0;
        state->number_of_planets_visited = 1;
        state->planets_visited = malloc((state->number_of_planets_visited)* sizeof(unsigned int));
        state->planets_visited[0] = crt_planet;
        state->distances_of_planets_visited = malloc((state->number_of_planets_visited)*sizeof(double));
        state->distances_of_planets_visited[0] = distance_from_mixer;
        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;

    }
    //============================================Refresh database======================================================
    struct ship_state_struct *state = ship_state;
    unsigned int next_planet;

    //Check if the planet has been visited before
    int visited_before = is_planet_in_array(state->planets_visited,
                                            state->number_of_planets_visited,
                                            crt_planet);

    //==================================If not visited, add to list=====================================================
    if (visited_before == -1){
        printf("This planet is new, added to list\n");
        state->number_of_planets_visited += 1;
        unsigned int *planets_visited_temp;
        double *distance_planet_temp;
        planets_visited_temp = realloc(state->planets_visited,
                                       (state->number_of_planets_visited) * sizeof(unsigned int));
        distance_planet_temp = realloc(state->distances_of_planets_visited,
                                       (state->number_of_planets_visited) * sizeof(double));
        if ((planets_visited_temp != NULL) && (distance_planet_temp != NULL)){
            state->planets_visited = planets_visited_temp;
            state->distances_of_planets_visited = distance_planet_temp;
            state->planets_visited[state->number_of_planets_visited-1] = crt_planet;
            state->distances_of_planets_visited[state->number_of_planets_visited-1] = distance_from_mixer;

        }else{
            printf("Something went wrong with the realloc;\n");
        }
    }
    printf("Number of unique planets visited: %d\n",state->number_of_planets_visited);
    //===========================================INITIAL RANDOM JUMPS===================================================
    if ((distance_from_mixer > TARGET_DISTANCE) && (state->start == 1)){
        //printf("Not good enough\n");
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    state->start = 0;
    //===================================================START SWEEP====================================================
    if (state->jump_logic == 0){
        state->num_connections_to_check = 0;
        state->connections_to_check = malloc(num_connections * sizeof(unsigned int));
        state->distances_of_connections = malloc(num_connections * sizeof(double));
        printf("Start loop\n");
    //====================================================GATHER DATA===================================================
        for (int index = 0; index < num_connections; index++) {
            int result = is_planet_in_array(state->planets_visited,
                                            state->number_of_planets_visited,
                                            connections[index]);
            if (result == -1) {
                state->num_connections_to_check++;//flag
                printf("Planet %u is unique\n", connections[index]);
                unsigned int *temp_ptr_connections;
                double *temp_ptr_distances;
                temp_ptr_connections = realloc(state->connections_to_check,
                                               (state->num_connections_to_check) * sizeof(unsigned int));
                temp_ptr_distances = realloc(state->distances_of_connections,
                                             (state->num_connections_to_check) * sizeof(double));
                state->connections_to_check = temp_ptr_connections;
                state->distances_of_connections = temp_ptr_distances;
                state->connections_to_check[state->num_connections_to_check-1]=connections[state->num_connections_to_check-1];
            } else {
                printf("Planet %u is NOT unique\n", connections[index]);
            }
        }
        printf("Information gathered\n");
        if (state->num_connections_to_check == 0){
            printf("No unique planets found\n");
            struct ship_action next_action = {RAND_PLANET, state};
            return next_action;
        }
        state->jump_logic = 2;
        struct ship_action next_action = {state->connections_to_check[0], state};
        return next_action;

    }
    //==================================================CHECK PLANETS===================================================
    else if ((state->index < state->num_connections_to_check)&&(state->jump_logic == 2)){
        state->distances_of_connections[state->index] = distance_from_mixer;
        next_planet = state->connections_to_check[state->index];
        state->index++;
        printf("Checking planet %d/%d\n",state->index, state->num_connections_to_check);
        struct ship_action next_action = {next_planet, state};
        return next_action;
    }else{
        state->jump_logic = 0;
        state->index = 0;
        double lowest_distance = 10.0;
        int lowest_index;
        for (int index = 0; index < state->num_connections_to_check; index++){
            if (state->distances_of_connections[index] < lowest_distance){
                lowest_distance = state->distances_of_connections[index];
                lowest_index = index;
            }
        }
        printf("Lowest index %d\n", lowest_index);
        next_planet = state->connections_to_check[lowest_index];
        printf("Lowest distance is %f for planet ID:%u\n", lowest_distance, next_planet);
        struct ship_action next_action = {next_planet, state};
        return next_action;
    }
}
