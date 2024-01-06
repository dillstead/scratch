// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g -o problem19b problem19b.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MAX(a, b)	      (((a) > (b)) ? (a) : (b))
#define MAX_TIME              32
#define MAX_ROCKS             4
#define MAX_ROBOTS            4
#define BLUEPRINT_FMT         "Blueprint %d: Each ore robot costs %d ore. "     \
                              "Each clay robot costs %d ore. "                  \
                              "Each obsidian robot costs %d ore and %d clay. "  \
                              "Each geode robot costs %d ore and %d obsidian.\n"
#define DIV_ROUND_UP(X, STEP) (((X) + (STEP) - 1) / (STEP))

enum rock
{
    ORE, CLAY, OBSIDIAN, GEODE
};

struct blueprint
{
    int cost[MAX_ROBOTS][MAX_ROCKS];
};

struct state
{
    int time;
    int robots[MAX_ROBOTS];
    int rocks[MAX_ROCKS];
};

static int calculate_upper_bound(struct state *state)
{
    int time_remain = MAX_TIME - state->time;
    int upper_bound = state->rocks[GEODE]
        + state->robots[GEODE] * time_remain;

    if (state->robots[OBSIDIAN] == 0)
    {
        time_remain -= 2;
        if (state->robots[CLAY] == 0)
        {
            time_remain -= 1;
        }
    }
    if (time_remain > 0)
    {
        upper_bound += time_remain * (time_remain + 1) / 2;
    }
    return upper_bound;
}

static int get_next_states(struct blueprint *blueprint, struct state *state,
                           int max_geodes, struct state *states)
{
    int num_states = 0;
    // Number of rocks needed to build a robot.
    int rocks;
    // The minium amount of time needed to make a robot.
    int time;
    int i, j;
    bool build_robot;

    if (calculate_upper_bound(state) <= max_geodes)
    {
        return 0;
    }

    for (i = MAX_ROBOTS - 1; i >= 0; i--)
    {
        build_robot = false;
        time = 1;
        for (j = 0; j < MAX_ROCKS; j++)
        {
            rocks = blueprint->cost[i][j] - state->rocks[j];
            if (rocks > 0)
            {
                // Don't have enough rocks, collect more.
                if (state->robots[j] == 0)
                {
                    break;
                }
                // + 1, It takes 1 unit of time to build a robot.
                time = MAX(time, DIV_ROUND_UP(rocks, state->robots[j]) + 1);
                build_robots = true;
            }
        }
        // If there are robots to collect the rocks and there's enough time
        // or already enough rocks, build the robot.
        if (j == MAX_ROCKS && state->time + time <= MAX_TIME)
        {
            memcpy(states + num_states, state, sizeof *state);
            for (j = 0; j < MAX_ROCKS; j++)
            {
                states[num_states].rocks[j] += time * state->robots[j] -
                    blueprint->cost[i][j];
            }
            states[num_states].time += time;
            states[num_states].robots[i]++;
            num_states++;
            if (!build_robot)
            {
                break;
            }
        }
    }
    if (num_states == 0)
    {
        // No robots can be built, collect as many rocks as possible before the
        // time expires.
        memcpy(states, state, sizeof *state);
        for (i = 0; i < MAX_ROCKS; i++)
        {
            states[0].rocks[i] += (MAX_TIME - state->time) * state->robots[i];
        }
        states[0].time = MAX_TIME;
        num_states = 1;
    }
    return num_states;
}

static int calculate_max_geodes(struct blueprint *blueprint, struct state *state, int
                                max_geodes)
{
    int num_states;
    int num_geodes = 0;
    struct state states[MAX_ROBOTS + 1];
    
    if (state->time == MAX_TIME)
    {
        return state->rocks[GEODE];
    }
    
    num_states = get_next_states(blueprint, state, max_geodes, states);
    for (int i = 0; i < num_states; i++)
    {
        num_geodes = calculate_max_geodes(blueprint, states + i, max_geodes);
        max_geodes = MAX(max_geodes, num_geodes);
    }
    return max_geodes;
}

int main(void)
{
    struct state state;
    struct blueprint blueprint;
    int id;
    int product = 1;

    memset(&state, 0, sizeof state);
    state.robots[ORE] = 1;
    for (;;)
    {
        memset(&blueprint, 0, sizeof blueprint);
        if (scanf(BLUEPRINT_FMT, &id, &blueprint.cost[ORE][ORE],
                  &blueprint.cost[CLAY][ORE],
                  &blueprint.cost[OBSIDIAN][ORE],
                  &blueprint.cost[OBSIDIAN][CLAY],
                  &blueprint.cost[GEODE][ORE],
                  &blueprint.cost[GEODE][OBSIDIAN]) != 7)
        {
            break;
        }
        product *= calculate_max_geodes(&blueprint, &state, 0);
        if (id == 3)
        {
            break;
        }
    }
    printf("%d\n", product);
    return EXIT_SUCCESS;
}
