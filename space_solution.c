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

    int jump_logic;
    int index;
    unsigned int *connections_to_check;
    int num_connections_to_check;
    double *distances_of_connections;
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
        state->distances_of_planets_visited = malloc((state->number_of_planets_visited)* sizeof(double));
        state->distances_of_planets_visited[0] = distance_from_mixer;

        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;

    }
    //Refresh database
    struct ship_state_struct *state = ship_state;

    //The planets_visited
    state->number_of_planets_visited += 1;
    printf("Number of planets visited: %d\n",state->number_of_planets_visited);

    //Add each planet and its distance to the ships database
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

    //Dollar store Dijkstra's algo
    //From each planet, save connections and num_connections check
    //Substructure????
    //N random jumps (optimise)
    //Jump through each connection in order and save distances to array
    //index of array of distances correspond to array of connections
    //find the shortest distance
    //jump to planet
    //repeat

    unsigned int next_planet = RAND_PLANET;

    if ((distance_from_mixer > TARGET_DISTANCE) && (state->start == 1)){
        printf("Not good enough\n");
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    state->start = 0;
    printf("Random jumps finished\n");

    //start sweep
    if (state->jump_logic == 0){
        state->connections_to_check = malloc(num_connections * sizeof(unsigned int));
        state->distances_of_connections = malloc(num_connections * sizeof(double));
        state->connections_to_check = connections;
        state->num_connections_to_check = num_connections;
        state->jump_logic = 1;
        printf("Start/Reset loop\n");
    }

    if (state->jump_logic == 1){
        if (state->index < state->num_connections_to_check){
            state->distances_of_planets_visited[state->index] = distance_from_mixer;
            next_planet = state->connections_to_check[state->index+1];
            state->index++;
            printf("Gathering information:\n");

        }else{
            double lowest_distance = TARGET_DISTANCE;
            int index_lowest_distance = 0;
            for (int index = 0; index < state->num_connections_to_check; index++){
                if (state->distances_of_planets_visited[index] < lowest_distance){
                    lowest_distance = state->distances_of_planets_visited[index];
                    index_lowest_distance = index;
                }
            }
            next_planet = state->connections_to_check[index_lowest_distance];
            printf("Go to next planet id:%u that is %f away from target\n",next_planet, lowest_distance);
            state->jump_logic = 0;
        }

    }




















    //Next Jump
    struct ship_action next_action = {next_planet, state};
    return next_action;
}
