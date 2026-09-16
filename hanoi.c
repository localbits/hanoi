#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "stack.h"
#include "types.h"

#define TOWER_COUNT 3
#define TARGET_TOWER 2

typedef struct {
    Stack* stack;
    u32 diskAmount;
} Towers;

typedef struct {
    u32* items;
    u32 length;
} Moves;

Moves* parse_moves_from_file(const char* fPath)
{
    u32 move[2] = {0};

    Moves* moves = (Moves*)(malloc(sizeof(Moves)));
    assert(moves != NULL);

    FILE* file = fopen(fPath, "r");
    assert(file != NULL);

    u32 moveCount = 0;
    fscanf(file, "%d", &moveCount);

    u32* movesVector = (u32*)malloc(sizeof(u32) * moveCount * 2);
    assert(movesVector != NULL);

    u32 moveIndex = 0;

    for (u32 i = 0; i < moveCount; ++i) {
        fscanf(file, "%d %d", &move[0], &move[1]);

        movesVector[moveIndex] = move[0];
        movesVector[++moveIndex] = move[1];
        moveIndex++;
    }

    moves->items = movesVector;
    moves->length = moveCount * 2;

    fclose(file);

    return moves;
}

void free_parsed_moves(Moves* moves)
{
    free(moves->items);
    free(moves);
    moves = NULL;
}

Towers* create_towers(u32 diskCount)
{
    Towers* towers = (Towers*)malloc(sizeof(Towers) * TOWER_COUNT);
    assert(towers != NULL);

    for (u32 i = 0; i < TOWER_COUNT; ++i) {
        towers[i].diskAmount = 0;
        towers[i].stack = create_stack(diskCount);
    }

    return towers;
}

bool is_tower_valid(s32 number)
{
    if (number < 0 || number > TOWER_COUNT - 1) {
        return false;
    }

    return true;
}

s32 disk_compare(const void* a, const void* b)
{
    s32* diskA = (s32*)a;
    s32* diskB = (s32*)b;

    if (*diskA > *diskB) {
        return -1;
    }

    if (*diskA == *diskB) {
        return 0;
    }

    return 1;
}

void sort_disks(s32* disks, u32 diskCount)
{
    qsort(disks, diskCount, sizeof(disks[0]), disk_compare);
}

bool can_move_disk(Towers* source, Towers* destiny)
{
    if (is_stack_empty(destiny->stack)) {
        return true;
    }


    if (is_stack_full(destiny->stack)) {
        return false;
    }

    s32 diskToMove = get_stack_top(source->stack);
    
    if (diskToMove > get_stack_top(destiny->stack)) {
        return false;
    }

    return true;
}

void insert_disk(Towers* tower, int disk)
{
    push_stack(tower->stack, disk);
    tower->diskAmount++;
}

s32 remove_disk(Towers* tower)
{
    s32 disk = pop_stack(tower->stack);
    tower->diskAmount--;
    return disk;
}

bool move_disk(Towers* source, Towers* destiny)
{
    if (!can_move_disk(source, destiny)) {
        return false;
    }

    s32 disk = remove_disk(source);
    insert_disk(destiny, disk);

    return true;
}

bool game_won(Towers* towers, u32 diskCount)
{
    if (towers[0].diskAmount > 0) {
        return false;
    }

    if (towers[TARGET_TOWER].diskAmount != diskCount) {
        return false;
    }

    return true;
}

void get_disks_from_player(s32* disks, u32 diskCount)
{
    for (u32 i = 0; i < diskCount; ++i) {
        printf("Type the disk value: ");
        u32 disk;
        scanf("%d", &disk);
        disks[i] = disk;
    }
}

void print_towers(Towers* towers)
{
    for (u32 i = 0; i < TOWER_COUNT; ++i) {
        printf("Tower[%d]\n", i);
        printf("Disks:\n");

        for (s32 j = towers[i].diskAmount - 1; j >= 0; --j) {
            printf("%d\n", towers[i].stack->items[j]);
        }

        printf("---\n");
    }
}

void free_towers(Towers* towers)
{
    for (u32 i = 0; i < TOWER_COUNT; ++i) {
        free_stack(towers[i].stack);
    }
    free(towers);
    towers = NULL;
}

s32* create_disks(u32 diskCount)
{
    s32* disks = (s32*)malloc(sizeof(s32) * diskCount);
    assert(disks != NULL);

    get_disks_from_player(disks, diskCount);
    sort_disks(disks, diskCount);

    return disks;
}

void play_from_file(const char* fPath, u32 diskCount)
{
    s32* disks = create_disks(diskCount);
    Towers* towers = create_towers(diskCount);

    for (u32 i = 0; i < diskCount; ++i) {
        insert_disk(&towers[0], disks[i]);
    }

    Moves* parsedMoves = parse_moves_from_file(fPath);

    u32* items = parsedMoves->items;
    u32 totalMoves = parsedMoves->length;

    for (u32 i = 0; i < totalMoves; i += 2) {
        print_towers(towers);
        printf("\n\n");
        u32 src = items[i];
        u32 dst = items[i+1];
        move_disk(&towers[src], &towers[dst]);
    }

    printf("--- FINAL TOWERS --- \n");
    print_towers(towers);
    
    if (game_won(towers, diskCount)) {
        printf("You won\n");
    } else {
        printf("You lost\n");
    }

    free_towers(towers);
    free_parsed_moves(parsedMoves);
    free(disks);
}

void play_manually(u32 diskCount)
{
    s32* disks = create_disks(diskCount);
    Towers* towers = create_towers(diskCount);

    for (u32 i = 0; i < diskCount; ++i) {
        insert_disk(&towers[0], disks[i]);
    }

    while (!game_won(towers, diskCount)) {
        printf("--- TURN ---\n");

        s32 src, dst;

        printf("Your turn: choose a source and destiny tower to make your move\n");
        print_towers(towers);
        printf("Pick 2 numbers: ");

        scanf("%" SCNi32, &src);
        scanf("%" SCNi32, &dst);

        if (!is_tower_valid(src) || !is_tower_valid(dst) || !move_disk(&towers[src], &towers[dst])) {
            printf("--- Invalid move ---\n\n");
        }
    }

    print_towers(towers);
    printf("You won\n");

    free_towers(towers);
    free(disks);
}

int main(void)
{
    bool playFromFile = false;
    u32 gameMode = 0;

    printf("Would you like to play manually or parse a move file?\n");
    printf("Type 1 for manual or 2 for parsing a file: ");
    scanf("%d", &gameMode);

    if (gameMode == 2) {
        playFromFile = true;
    }

    u32 diskCount = 0;
    printf("What amount of disks would you like? ");
    scanf("%d", &diskCount);

    if (playFromFile) {
        play_from_file("moves.txt", diskCount);
    } else {
        play_manually(diskCount);
    }

    return 0;
}
