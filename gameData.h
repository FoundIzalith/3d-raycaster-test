struct player_t {
    float x, y, deltaX, deltaY, angle;
} player; 

struct map_t {
    int x, y, s;
};

struct map_t map = {
    .x = 8,
    .y = 8,
    .s = 64,
};

int grid[] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 
        1, 0, 1, 0, 0, 1, 0, 1, 
        1, 0, 0, 0, 0, 0, 0, 1, 
        1, 0, 0, 0, 0, 0, 0, 1, 
        1, 0, 1, 0, 0, 1, 0, 1, 
        1, 0, 0, 0, 0, 0, 0, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 
    };