#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the size of the puzzle grid
#define GRID_SIZE 3

// Define the possible moves (up, down, left, right)
#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

// Define the heuristic function (Manhattan distance)
int heuristic(int* state) {
    int distance = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        int x = i % GRID_SIZE;
        int y = i / GRID_SIZE;
        int goal_x = (state[i] - 1) % GRID_SIZE;
        int goal_y = (state[i] - 1) / GRID_SIZE;
        distance += abs(x - goal_x) + abs(y - goal_y);
    }
    return distance;
}

// Define the A\* search algorithm
void a_star_search(int* initial_state, int* goal_state) {
    // Create a priority queue to store nodes to be explored
    typedef struct Node {
        int* state;
        int cost;
        int heuristic;
        struct Node* parent;
    } Node;
    Node* queue[1000]; // assume max 1000 nodes in the queue
    int queue_size = 0;

    // Create the initial node
    Node* initial_node = (Node*)malloc(sizeof(Node));
    initial_node->state = initial_state;
    initial_node->cost = 0;
    initial_node->heuristic = heuristic(initial_state);
    initial_node->parent = NULL;
    queue[queue_size++] = initial_node;

    // Explore nodes until the goal state is reached
    while (queue_size > 0) {
        // Extract the node with the lowest f-score (cost + heuristic)
        Node* current_node = queue[0];
        for (int i = 1; i < queue_size; i++) {
            if (queue[i]->cost + queue[i]->heuristic < current_node->cost + current_node->heuristic) {
                current_node = queue[i];
            }
        }
        queue_size--;
        for (int i = 0; i < queue_size; i++) {
            queue[i] = queue[i + 1];
        }

        // Check if the goal state is reached
        if (memcmp(current_node->state, goal_state, sizeof(int) * GRID_SIZE * GRID_SIZE) == 0) {
            // Reconstruct the solution path
            Node* path[1000]; // assume max 1000 nodes in the path
            int path_size = 0;
            Node* node = current_node;
            while (node!= NULL) {
                path[path_size++] = node;
                node = node->parent;
            }
            // Print the solution path
            for (int i = path_size - 1; i >= 0; i--) {
                for (int j = 0; j < GRID_SIZE * GRID_SIZE; j++) {
                    printf("%d ", path[i]->state[j]);
                    if ((j + 1) % GRID_SIZE == 0) {
                        printf("\n");
                    }
                }
                printf("\n");
            }
            return;
        }

        // Generate successor nodes
        for (int move = 0; move < 4; move++) {
            int* successor_state = (int*)malloc(sizeof(int) * GRID_SIZE * GRID_SIZE);
            memcpy(successor_state, current_node->state, sizeof(int) * GRID_SIZE * GRID_SIZE);
            int blank_index = -1;
            for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
                if (successor_state[i] == 0) {
                    blank_index = i;
                    break;
                }
            }
            int x = blank_index % GRID_SIZE;
            int y = blank_index / GRID_SIZE;
            int new_x = x;
            int new_y = y;
            switch (move) {
                case MOVE_UP:
                    new_y--;
                    break;
                case MOVE_DOWN:
                    new_y++;
                    break;
                case MOVE_LEFT:
                    new_x--;
                    break;
                case MOVE_RIGHT:
                    new_x++;
                    break;
            }
            if (new_x >= 0 && new_x < GRID_SIZE && new_y >= 0 && new_y < GRID_SIZE) {
                int new_index = new_y * GRID_SIZE + new_x;
                int temp = successor_state[blank_index];
                successor_state[blank_index] = successor_state[new_index];
                successor_state[new_index] = temp;
                Node* successor_node = (Node*)malloc(sizeof(Node));
                successor_node->state = successor_state;
                successor_node->cost = current_node->cost + 1;
                successor_node->heuristic = heuristic(successor_state);
                successor_node->parent = current_node;
                queue[queue_size++] = successor_node;
            }
        }
    }
}

int main() {
    // Define the initial state
    int initial_state[] = {
        1, 2, 3,
        4, 0, 6,
        7, 5, 8
    };

    // Define the goal state
    int goal_state[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0
    };

    // Solve the puzzle using A\* search
    a_star_search(initial_state, goal_state);

    return 0;
}
