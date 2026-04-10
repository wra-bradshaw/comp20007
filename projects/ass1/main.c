/*
 * Starter file for Project 1 Part 1 (Link
 * https://edstem.org/au/courses/31090/lessons/104103/slides/714972)
 *
 * Created for COMP20007 Design of Algorithms 2026
 * Template written by Thomas Minuzzo <thomas.minuzzo@unimelb.edu.au>
 * Implementation by YOUR NAME HERE.
 * Compile with gcc -Wall -g -o lantern lantern.c -lm
 */

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEDY_ALG 1
#define DRONE_SPEED 1
#define PLANE_WIDTH 10
#define POSITIVE 1
#define NEGATIVE -1
#define CANNOT_BURST -1

#define MAX_DRONES 10
#define MAX_LANTERNS 1000
#define MAX_TIME 1000

#define TRUE 1
#define FALSE 0

#define DEBUG 0

/* This struct stores all relevant information about a drone/lantern */
typedef struct {
  int x;
  int trajectory_x[MAX_TIME];
  int trajectory_time;
  int lantern_path[MAX_LANTERNS];
  int path_length;
} drone_t;

typedef struct {
  int x;
  int is_lit;
  int arrival_time;
} lantern_t;

typedef struct {
  drone_t drone[MAX_DRONES];
  int num_drones;
  lantern_t lantern[MAX_LANTERNS];
  int num_lanterns;
  int total_time;
} map_t;

/*
 * Recall, these are called function prototypes and they declare the function
 * names, argument types and return types that will be implemented in this file.
 */

/* reads the map */
void read_map(map_t *map);

/* simulates the map */
void simulate_map(map_t *map);

/* output the simulation resuls */
void simulation_results(map_t map);

/* TODO: create lantern paths for drones to eliminate lanterns */
void generate_lantern_paths(map_t *map);

/* creates trajectories for simulation converting indicies stored in lantern
 * path to trajectories*/
void generate_trajectories(map_t *map);

/*
 * This is the main function, which is the entry point to your program.
 *
 * Recall that argc is the number of command line arguments, and argv
 * is an array of those arguments (an array of pointers to null terminated
 * strings).
 */
int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Usage: <executable> <stage number>\ne.g., lantern 1\n");
    return 1;
  }

  // Convert stage number into integer format
  int stage_num = atoi(argv[1]);

  map_t map;

  switch (stage_num) {
  case GREEDY_ALG:
    read_map(&map);
    generate_lantern_paths(&map);
    generate_trajectories(&map);
    simulate_map(&map);
    simulation_results(map);
    break;

  default:
    fprintf(stderr, "Error: unsupported stage %d.\n", stage_num);
    return 2;
  }

  return 0;
}

/* ------------------ PROVIDED FUNCTIONS -------------------- */

static void die(const char *msg) {
  fprintf(stderr, "Input error: %s\n", msg);
  exit(1);
}

/* reads the map */
void read_map(map_t *map) {
  // Read "Total time: <int>"
  if (scanf("Total time: %d\n", &map->total_time) != 1)
    die("could not read total time");
  if (map->total_time < 0 || map->total_time > MAX_TIME)
    die("total time out of bounds");

  // Read "Number of drones: <int>"
  if (scanf("Number of drones: %d\n", &map->num_drones) != 1)
    die("could not read number of drones");
  if (map->num_drones < 0 || map->num_drones > MAX_DRONES)
    die("number of drones out of bounds");

  // Read "Number of lanterns: <int>"
  if (scanf("Number of lanterns: %d\n", &map->num_lanterns) != 1)
    die("could not read number of lanterns");
  if (map->num_lanterns < 0 || map->num_lanterns > MAX_LANTERNS)
    die("number of lanterns out of bounds");

  // Read "Drones (starting location):"
  scanf("Drones (starting location):\n");
  for (int i = 0; i < map->num_drones; i++) {
    if (scanf("%d\n", &map->drone[i].x) != 1)
      die("could not read drone starting position");
    if (map->drone[i].x < 1 || map->drone[i].x > PLANE_WIDTH)
      die("drone starting position out of bounds");

    // Initialise drone trajectory: stationary hover
    for (int t = 0; t < map->total_time; t++)
      map->drone[i].trajectory_x[t] = 0;

    map->drone[i].trajectory_time = 0;
    // Currently set to move after zero drones
    map->drone[i].path_length = 0;
  }

  // Read "Lanterns (location, arrival time):"
  scanf("Lanterns (location, arrival time):\n");
  for (int j = 0; j < map->num_lanterns; j++) {
    int xj, aj;
    if (scanf("%d %d\n", &xj, &aj) != 2)
      die("could not read lantern (x, arrival_time)");
    if (xj < 1 || xj > PLANE_WIDTH)
      die("lantern x out of bounds");
    if (aj < 0 || aj > map->total_time)
      die("lantern arrival time out of bounds");

    map->lantern[j].x = xj;
    map->lantern[j].arrival_time = aj;
    map->lantern[j].is_lit = TRUE;
  }
}

/* simulates the map */
void simulate_map(map_t *map) {
  for (int t = 0; t < map->total_time; t++) {
    for (int i = 0; i < map->num_drones; i++) {
      // Check to see if bursting any lanterns
      for (int j = 0; j < map->num_lanterns; j++) {
        if (map->lantern[j].is_lit == FALSE) {
          continue; // already extinguished
        }

        if (t == map->lantern[j].arrival_time &&
            map->drone[i].x == map->lantern[j].x) {
          printf("At time %d drone %d eliminates lantern %d\n", t, i, j);
          map->lantern[j].is_lit = FALSE;
        }
      }

      // Move drone along trajectory
      map->drone[i].x += map->drone[i].trajectory_x[t];

      // Ensure drone remains within legal bounds
      if (map->drone[i].x < 1) {
        map->drone[i].x = 1;
      } else if (map->drone[i].x > PLANE_WIDTH) {
        map->drone[i].x = PLANE_WIDTH;
      }
    }
  }
}

/* output the simulation resuls */
void simulation_results(map_t map) {
  int num_eliminated = 0;
  for (int i = 0; i < map.num_lanterns; i++)
    if (!map.lantern[i].is_lit)
      num_eliminated++;

  printf("Lanterns eliminated %d of %d\n", num_eliminated, map.num_lanterns);

  if (DEBUG) {
    for (int i = 0; i < map.num_drones; i++) {
      printf("Drone[%d] trajectory: ", i);
      for (int j = 0; j < map.total_time; j++)
        printf("%d ", map.drone[i].trajectory_x[j]);
      printf("\n");
    }
  }

  for (int i = 0; i < map.num_drones; i++) {
    printf("Drone[%d] lantern path: ", i);
    for (int j = 0; j < map.drone[i].path_length; j++)
      printf("%d ", map.drone[i].lantern_path[j]);
    printf("\n");
  }
}

/* TODO: create lantern paths for drones to eliminate lanterns,
    i.e., a set of indices that correspond to the order that a drone will
   eliminate lanterns*/
void generate_lantern_paths(map_t *map) {
  for (int j = 0; j < map->num_lanterns; j++) {
    lantern_t *current_lantern = &map->lantern[j];
    drone_t *most_eligible_drone = NULL;
    int most_eligible_drone_t_free = 0;
    int most_eligible_drone_dist = -1;
    int lantern_t = current_lantern->arrival_time;
    int lantern_x = current_lantern->x;

    for (int i = 0; i < map->num_drones; i++) {
      drone_t *current_drone = &map->drone[i];
      int last_lantern_j = -1;
      int t_free = 0;
      int x_free = current_drone->x;

      if (current_drone->path_length > 0) {
        last_lantern_j =
            current_drone->lantern_path[current_drone->path_length - 1];
        t_free = map->lantern[last_lantern_j].arrival_time;
        x_free = map->lantern[last_lantern_j].x;
      }

      int dist = abs(x_free - lantern_x);

      if (dist <= DRONE_SPEED * (lantern_t - t_free)) {
        int is_better = FALSE;

        if (most_eligible_drone == NULL) {
          is_better = TRUE;
        } else if (dist < most_eligible_drone_dist) {
          is_better = TRUE;
        } else if (dist == most_eligible_drone_dist &&
                   t_free < most_eligible_drone_t_free) {
          is_better = TRUE;
        }

        if (is_better) {
          most_eligible_drone = current_drone;
          most_eligible_drone_t_free = t_free;
          most_eligible_drone_dist = dist;
        }
      }
    }

    if (most_eligible_drone != NULL) {
      most_eligible_drone->lantern_path[most_eligible_drone->path_length] = j;
      most_eligible_drone->path_length++;
    }
  }
}

/* creates trajectories for simulation converting indicies stored in lantern
 * path to trajectories*/
void generate_trajectories(map_t *map) {
  for (int i = 0; i < map->num_drones; i++) {
    int Dx = map->drone[i].x;
    for (int j = 0; j < map->drone[i].path_length; j++) {
      int dest_idx = map->drone[i].lantern_path[j];
      int Lx = map->lantern[dest_idx].x;
      int Lt = map->lantern[dest_idx].arrival_time;

      // Guard against non-monotone times; skip if an earlier arrival appears
      if (Lt < map->drone[i].trajectory_time) {
        if (DEBUG) {
          fprintf(stderr,
                  "Warning: non-monotone arrival time for drone %d: have t=%d, "
                  "got %d\n",
                  i, map->drone[i].trajectory_time, Lt);
        }
        continue;
      }

      // Calculate direction of drone travel
      int direction = (Dx > Lx) ? NEGATIVE : POSITIVE;

      if (DEBUG) {
        printf("drone pos = %d, Drone dest = %d, Drone time = %d, direction = "
               "%d\n",
               Dx, Lx, Lt, direction);
      }

      // Move one step per tick toward the destination until the arrival time
      while (map->drone[i].trajectory_time < Lt &&
             map->drone[i].trajectory_time < map->total_time) {
        if (Dx != Lx) {

          map->drone[i].trajectory_x[map->drone[i].trajectory_time] =
              DRONE_SPEED * direction;
          Dx += DRONE_SPEED * direction;
        }
        map->drone[i].trajectory_time++;
      }
    }
  }
}

/* =====================================================================
   This skeleton/program is compiled by the comp20007 teaching team,
   the university of Melbourne.
   Last Updated: 16/02/2026
   ================================================================== */
