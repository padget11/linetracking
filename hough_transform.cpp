#include "Arduino.h"
#include "hough_transform.h"

// function to calculate the edge points in an image using the Sobel filter, returns an array
// of the image with a 1 where the edge point occurs
uint8_t** hough_transform::sobel(uint8_t (*im)[COLS])
{
  // reutrn array
  uint8_t** return_im = new uint8_t * [ROW2];
  for (int t = 0; t < ROW2; ++t)
  {
    return_im[t] = new uint8_t[COL2];
    for (int s = 0; s < COL2; s++)
      return_im[t][s] = 0;
  }

  // loop through each point
  for (int x = 2; x < ROW2; x++)
  {
    for (int y = 0; y < COL2; y++)
    {
      int sum_x = 0;
      int mul_x = 0;
      int sum_y = 0;
      int mul_y = 0;
      for (int i = 0; i < WINDOW_SIZE; i++)
      {
        for (int j = 0; j < WINDOW_SIZE; j++)
        {
          // calculate sum of window in x and y direction
          mul_x = im[x + i][y + j] * Mx[i][j];
          sum_x = sum_x + mul_x;
          mul_y = im[x + i][y + j] * My[i][j];
          sum_y = sum_y + mul_y;
        }
      }
      //if (return_im)
        //if (return_im[x])
          // set array to edge point if the sum is above the threshold
          if (abs(sum_x) > sobel_threshold || abs(sum_y) > sobel_threshold)
            return_im[x][y] = 1;
    }
  }
  return return_im;
}

// function to calculate the polar coordinates of the image using the edge points
// as an input and outputs the rho and theta values
void hough_transform::hough_algorithm(uint8_t** im)
{
  // setup parameter space with size of max. value of rho and 0 to 180 degrees
  uint8_t** param_space = new uint8_t * [PARAM_ROW];
  for (int i = 0; i < PARAM_ROW; i++)
  {
    param_space[i] = new uint8_t[PARAM_COL];
    for (int j = 0; j < PARAM_COL; j++)
      param_space[i][j] = 0;
  }

  float theta, rho;

  // loop through every edge point
  for (int x = 0; x < ROW2; x++)
  {
    for (int y = 0; y < COL2; y++)
    {
      if (im[x][y] == 1)
      {
        // loop through each theta
        for (float i = 0; i < PARAM_COL; i++)
        {
          theta = (i / 360) * (2 * 3.1416);
          rho = round(((float)x * cos(theta) + (float)y * sin(theta)));
          int theta_int = (int)i;
          int rho_int = (int)rho;
          // increment value in parameter space
          param_space[198 + rho_int][(int)i] = param_space[198 + rho_int][(int)i] + 1;
        }
      }
    }
  }

  for (uint16_t i = 0; i < ROW2; i++)
  {
    free(im[i]);
  }
  free(im);

  // find maximum value in parameter space
  int max_param = 0;
  for (int x = 0; x < PARAM_ROW; x++)
  {
    for (int y = 0; y < PARAM_COL; y++)
    {
      if (param_space[x][y] > max_param)
      {
        results.rho = x - 198;
        results.theta = y;
        max_param = param_space[x][y];
      }
    }
  }

  for (uint16_t i = 0; i < PARAM_ROW; i++)
  {
    free(param_space[i]);
  }
  free(param_space);
  
}
