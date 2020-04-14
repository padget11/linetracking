#include "Arduino.h"
#include "histogram_eq.h"

#define ROWS 120
#define COLS 160

void histogram_eq::equilise(uint8_t (*im)[COLS])
{
  // count the number that each pixel value occurs
  float histogram[256][4] = { 0 };
  for (int x = 0; x < ROWS; x++)
  {
    for (int y = 0; y < COLS; y++)
    {
      histogram[im[x][y]][0] = histogram[im[x][y]][0] + 1;
    }
  }

  for (int x = 0; x < 256; x++)
  {
    histogram[x][1] = histogram[x][0] / (ROWS * COLS); // divide by total number of pixels
    for (int y = 0; y < (x + 1); y++)
    {
      histogram[x][2] = histogram[x][2] + histogram[y][1]; // cumulative sum
    }
    histogram[x][3] = round(histogram[x][2] * 255); // multiply by number of levels and round to new level
  }

  for (int x = 0; x < ROWS; x++)
  {
    for (int y = 0; y < COLS; y++)
    {
      int new_value = (int)histogram[im[x][y]][3];
      im[x][y] = new_value;
      //printf("%d, ", new_image[x][y]);
    }
    //printf("\n");
  }
}
