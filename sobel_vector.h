#ifndef sobel_vector_h
#define sobel_vector_h

#define ROWS 120
#define COLS 160
#define ROW2 118
#define COL2 158
#define WINDOW_SIZE 3

struct sobel_line
{
  float angle;
  int offset;
};

class sobel_vector
{
  public:
  // Mx = -1 0 1  My =  1  2  1
  //      -2 0 1        0  0  0
  //      -1 0 1        -1 -2 -1
  int Mx[WINDOW_SIZE][WINDOW_SIZE] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
  int My[WINDOW_SIZE][WINDOW_SIZE] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
  int sobel_threshold;
  sobel_line results;
  void sobel_algorithm(uint8_t (*im)[COLS]);
};

#endif
