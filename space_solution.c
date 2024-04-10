#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TARGET_DISTANCE 6

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int start;

    int number_of_planets_visited;                  //number of overall visited planets
    unsigned int *planets_visited;                  //Pointer to array of visited planets
    double *distances_of_planets_visited;           //Pointer to array of distances of visited planets

    int jump_logic;                                 //Jump logic bool
    int index;                                      //index for sweep
    unsigned int *connections_to_check;             //Array of connections for the sweep
    int num_connections_to_check;                   //Number of connections to sweep
    double *distances_of_connections;               //distances of the planets in the sweep
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
    //Refresh database
    struct ship_state_struct *state = ship_state;

    //The planets_visited
    int visited_before = 0;
    for (int index = 0; index < state->number_of_planets_visited; index++){
        if (state->planets_visited[index] == crt_planet){
            visited_before = 1;
            printf("Have been on planet %u\n", crt_planet);
        }
    }
    if (visited_before == 0){
        printf("Check\n");
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


    unsigned int next_planet = 0;

    //Initial random jumps
    if ((distance_from_mixer > TARGET_DISTANCE) && (state->start == 1)){
        //printf("Not good enough\n");
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    state->start = 0;

    //start sweep
    if (state->jump_logic == 0){
        state->connections_to_check = malloc(num_connections * sizeof(unsigned int));
        state->distances_of_connections = malloc(num_connections * sizeof(double));
        state->connections_to_check = connections;
        state->num_connections_to_check = num_connections;
        state->jump_logic = 1;
        printf("Start loop\n");
    }
    //Sweep
    if (state->jump_logic == 1){
        //Gather information
        if (state->index < state->num_connections_to_check){
            //Check if we have visited the planet before
            visited_before = 0;
            for (int index = 0; index < state->number_of_planets_visited;index++){
                for (int inner_index = 0; inner_index < state->num_connections_to_check; inner_index++){
                    if (state->planets_visited[index] == state->connections_to_check[inner_index]){
                        visited_before += 1;
                    }
                    if (visited_before == 1){
                        printf("Planet %u/%d is seen at %u/%d\n",state->connections_to_check[inner_index], inner_index,
                               state->planets_visited[index], index);
                    }
                }
            }

            if (visited_before != 1){
                state->distances_of_connections[state->index] = distance_from_mixer;
                next_planet = state->connections_to_check[state->index+1];
                state->index++;
                printf("Added planet to list\n");
            }

            if (visited_before==1){
                state->index += 2;
                next_planet = state->connections_to_check[state->index+2];
            }

            printf("Jumping through connections %d/%d:\n", state->index,state->num_connections_to_check);
            printf("Next planet: %u\n", next_planet);

        }else{ //Find the lowest distance in array and go there
            double lowest_distance = TARGET_DISTANCE;
            int index_lowest_distance = 0;
            for (int index = 0; index < state->num_connections_to_check; index++){
                if (state->distances_of_connections[index] < lowest_distance){
                    lowest_distance = state->distances_of_connections[index];
                    index_lowest_distance = index;
                }
            }
            next_planet = state->connections_to_check[index_lowest_distance];
            printf("Go to next planet id:%u that is %f away from target\n",next_planet, lowest_distance);
            state->jump_logic = 0;
            state->index = 0;
        }
    }


    //Next Jump
    struct ship_action next_action = {next_planet, state};
    return next_action;
}
