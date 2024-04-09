#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Ship database. A structure that hold all the information
struct ship_state_struct {
    int number_of_planets_visited; //number of overall visited planets
    unsigned int *planets_seen; //Pointer to array of observed planets
    int number_of_planets_seen; //number of seen planets
    unsigned int *planets_visited; //Pointer to array of visited planets
};

ShipAction space_hop(unsigned int crt_planet, //Current planet
                   unsigned int *connections, //Pointer to array of integers. ID of planets you can go to
                   int num_connections, //number of planets in array - number of neighbour planets
                   double distance_from_mixer, //crows flies distance to target planet
                   void *ship_state) //pointer to storage structure
{
    printf("==============================================\n");
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("Current planet: %u\n", crt_planet);
    printf("Number of connections: %d\n", num_connections);

    //create a ship state if nothing exist. On spawn basically.
    if (ship_state == NULL){
        printf("Ship state is null\n");
        struct ship_state_struct *state = malloc(sizeof(struct ship_state_struct));

        //Have not visited any planets yet


        //All seen planets.
        //Increases the amount of seen planets and adds each pointer to the array
        state->number_of_planets_seen = num_connections;
        state->planets_seen = malloc((num_connections)*sizeof(unsigned int));
        for (int index = (state->number_of_planets_seen-num_connections);index<state->number_of_planets_seen;index++){
            state->planets_seen[index] = connections[index];
        }

        //All visited planets
        state->number_of_planets_visited = 1;
        state->planets_visited = malloc((state->number_of_planets_visited)* sizeof(unsigned int));
        state->planets_visited[0] = crt_planet;

        printf("Number of planets visited: %d\n", state->number_of_planets_visited);
        struct ship_action next_action = {RAND_PLANET, state};
        return next_action;
    }
    //Next planet to jump to
    unsigned int_next_planet = RAND_PLANET;

    //Refresh database
    struct ship_state_struct *state = ship_state;

    //The planets_seen
    int number_new_seen = 0;
    for (int index = 0; index < (state->number_of_planets_seen + num_connections); index++){
        int bool_already_seen = 0;

        for (int inner_index = 0; inner_index < num_connections; inner_index++){

            if (state->planets_seen[index] == connections[inner_index]){
                bool_already_seen = 1;
                break;
            }
        }
        if (!bool_already_seen){
            number_new_seen++;
        }
    }


    state->number_of_planets_seen += number_new_seen;
    unsigned int *planets_temp_ptr;
    planets_temp_ptr = realloc(state->planets_seen,
                               (state->number_of_planets_seen) * sizeof(unsigned int));
    printf("Check\n");
    if (planets_temp_ptr != NULL){
        state->planets_seen = planets_temp_ptr;
    }else{
        printf("Something went wrong with the realloc for planets_seen;\n");
    }

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









    //Next Jump
    struct ship_action next_action = {int_next_planet, state};
    return next_action;
}
