#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TARGET_DISTANCE 7
//return planet in array second closest to target

unsigned int second_closest_in_array_to_target(const unsigned int *array_planets,
                                               const double* array_distances,
                                               int arrays_len){
    double closest = array_distances[0];
    double sec_closest = 100.0;
    unsigned int second_closest_planet = 0;
    for (int index=0; index < arrays_len; index++){
        if(array_distances[index] < closest){
            closest = array_distances[index];
        }
    }
    for (int index=0; index < arrays_len; index++){
        if(array_distances[index] != closest && array_distances[index] < sec_closest){
            sec_closest = array_distances[index];
            second_closest_planet = array_planets[index];
        }
    }
    return second_closest_planet;
}
//return planet in array closest to the target
unsigned int closest_planet_in_array_to_target(const unsigned int *array_planets,
                                               const double* array_distances,
                                               int arrays_len){
    double lowest_distance = 100.0;
    int lowest_index;
    for (int index = 0; index < arrays_len; index++){
        if (array_distances[index] < lowest_distance){
            lowest_distance = array_distances[index];
            lowest_index = index;
        }
    }
    return array_planets[lowest_index];
}

//Checks if the planet is in the array. Returns index if it is, and -1 if not
int is_planet_in_array(const unsigned int *array,
                       int array_len,
                       double planet_id){
    for (int index = 0; index < array_len; index++){
        if (array[index] == planet_id){
            return index;
        }
    }
    return -1;
}

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int random_jumps_on;                            //Random jumps pseudo boolean

    int number_of_planets_visited;                  //number of overall unique visited planets
    unsigned int *planets_visited;                  //Pointer to array of unique visited planets
    double *distances_of_planets_visited;           //Pointer to array of distances of unique visited planets

    int jump_logic;                                 //Jump logic pseudo bool
    int index;                                      //index for sweep
    unsigned int *connections_to_check;             //Array of connections for the sweep
    int num_connections_to_check;                   //Number of connections to sweep
    double *distances_of_connections;               //distances of the planets in the sweep
    unsigned int second_lowest_planet;              //Second-closest planet to target
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

    //===============================================START==============================================================
    if (ship_state == NULL){
        printf("Ship state is null\n");
        struct ship_state_struct *state = malloc(sizeof(struct ship_state_struct));

        state->random_jumps_on = 1;
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

    //============================================CHECK PLANET STATUS===================================================
    int visited_before = is_planet_in_array(state->planets_visited,
                                            state->number_of_planets_visited,
                                            crt_planet);
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
    if ((distance_from_mixer > TARGET_DISTANCE) && (state->random_jumps_on == 1)){
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    state->random_jumps_on = 0;

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
            if (state->second_lowest_planet != 0){
                printf("Backtracking\n");
                state->jump_logic = 1;
                struct ship_action next_action = {state->second_lowest_planet, state};
                return next_action;
            }else{
                printf("Commencing random planet jumps\n");
                state->random_jumps_on = 1;
                state->jump_logic = 2;
                struct ship_action next_action = {RAND_PLANET, state};
                return next_action;
            }
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
        next_planet = closest_planet_in_array_to_target(state->connections_to_check,
                                                        state->distances_of_connections,
                                                        state->num_connections_to_check);
        if (state->num_connections_to_check > 1){
            unsigned int temp;
            temp = second_closest_in_array_to_target(state->connections_to_check,
                                                     state->distances_of_connections,
                                                     state->num_connections_to_check);
            if (temp != 0){
                state->second_lowest_planet = temp;
            }
        }

        printf("Lowest distance is planet ID:%u\n", next_planet);

        printf("Backup planet is planet ID:%u\n", state->second_lowest_planet);
        struct ship_action next_action = {next_planet, state};
        return next_action;
    }
}
