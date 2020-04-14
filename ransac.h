#ifndef ransac_h
#define ransac_h

#define ROWS 120
#define COLS 160
#define WINDOW_SIZE 3

struct ransac_line
{
  float m;
  float c;
};

class ransac
{
  public:
  // Mx = -1 0 1  My =  1  2  1
  //      -2 0 1        0  0  0
  //      -1 0 1        -1 -2 -1
  int Mx[WINDOW_SIZE][WINDOW_SIZE] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
  int My[WINDOW_SIZE][WINDOW_SIZE] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
  int sobel_threshold;
  int ransac_threshold;
  int iterations;
  ransac_line results;
  void ransac_algorithm(uint8_t (*im)[COLS]);
};

#endif
