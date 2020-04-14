#ifndef hough_transform_h
#define hough_transform_h

#define ROWS 120
#define COLS 160
#define WINDOW_SIZE 3
#define ROW2 118
#define COL2 158
#define PARAM_ROW 397
#define PARAM_COL 180

struct hough_line
{
  int rho;
  int theta;
};

class hough_transform
{
  public:
  // Mx = -1 0 1  My =  1  2  1
  //      -2 0 1        0  0  0
  //      -1 0 1        -1 -2 -1
  int Mx[WINDOW_SIZE][WINDOW_SIZE] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
  int My[WINDOW_SIZE][WINDOW_SIZE] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
  int sobel_threshold;
  hough_line results;
  uint8_t** sobel(uint8_t (*im)[COLS]);
  void hough_algorithm(uint8_t** im);
};

#endif
