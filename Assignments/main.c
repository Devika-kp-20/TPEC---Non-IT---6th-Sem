#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 3
#define MAX_PQ_SIZE 100000

// Structure to represent the board state
typedef struct {
    int tiles[N][N];
    int zero_row;
    int zero_col;
    int g; // Cost from start to current node
    int h; // Estimated cost from current node to goal
} Board;

// Priority queue to hold the nodes
typedef struct {
    Board boards[MAX_PQ_SIZE];
    int size;
} PriorityQueue;

// Goal state for the 8-puzzle
int goal[N][N] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// Function to print the board state
void printBoard(Board *board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%2d ", board->tiles[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to calculate Manhattan distance
int manhattan(Board *board) {
    int distance = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = board->tiles[i][j];
            if (value != 0) {
                int target_row = (value - 1) / N;
                int target_col = (value - 1) % N;
                distance += abs(i - target_row) + abs(j - target_col);
            }
        }
    }
    return distance;
}

// Function to check if the board is in the goal state
int isGoal(Board *board) {
    return memcmp(board->tiles, goal, sizeof(goal)) == 0;
}

// Function to initialize a board with given tiles
void initBoard(Board *board, int tiles[N][N]) {
    memcpy(board->tiles, tiles, sizeof(board->tiles));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tiles[i][j] == 0) {
                board->zero_row = i;
                board->zero_col = j;
            }
        }
    }
    board->g = 0;
    board->h = manhattan(board);
}

// Function to swap two integers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to generate a new board state by moving the blank tile
Board move(Board board, int new_zero_row, int new_zero_col) {
    Board new_board = board;
    swap(&new_board.tiles[board.zero_row][board.zero_col], &new_board.tiles[new_zero_row][new_zero_col]);
    new_board.zero_row = new_zero_row;
    new_board.zero_col = new_zero_col;
    new_board.g = board.g + 1;
    new_board.h = manhattan(&new_board);
    return new_board;
}

// Function to push a board onto the priority queue
void push(PriorityQueue *pq, Board board) {
    pq->boards[pq->size++] = board;
}

// Function to pop the board with the lowest cost from the priority queue
Board pop(PriorityQueue *pq) {
    int best_index = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->boards[i].g + pq->boards[i].h < pq->boards[best_index].g + pq->boards[best_index].h) {
            best_index = i;
        }
    }
    Board best_board = pq->boards[best_index];
    pq->boards[best_index] = pq->boards[--pq->size];
    return best_board;
}

// Function to check if a board configuration is solvable
int isSolvable(int tiles[N][N]) {
    int inversions = 0;
    int array[N * N];
    int index = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array[index++] = tiles[i][j];
        }
    }
    for (int i = 0; i < N * N; i++) {
        for (int j = i + 1; j < N * N; j++) {
            if (array[i] && array[j] && array[i] > array[j]) {
                inversions++;
            }
        }
    }
    return (inversions % 2) == 0;
}

// Function to solve the 8-puzzle problem using A* search
void solve(int initial_tiles[N][N]) {
    if (!isSolvable(initial_tiles)) {
        printf("No solution possible\n");
        return;
    }

    PriorityQueue pq;
    pq.size = 0;
    Board initial;
    initBoard(&initial, initial_tiles);
    push(&pq, initial);

    while (pq.size > 0) {
        Board current = pop(&pq);
        if (isGoal(&current)) {
            printf("Minimum number of moves = %d\n", current.g);
            printBoard(&current);
            return;
        }

        // Generate all possible moves
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            int new_row = current.zero_row + directions[i][0];
            int new_col = current.zero_col + directions[i][1];
            if (new_row >= 0 && new_row < N && new_col >= 0 && new_col < N) {
                Board neighbor = move(current, new_row, new_col);
                push(&pq, neighbor);
            }
        }
    }
}

// Main function to initialize the puzzle and solve it
int main() {
    int initial_tiles[N][N] = {
        {1, 2, 3},
        {4, 0, 5},
        {7, 8, 6}
    };

    solve(initial_tiles);
    return 0;
}
