#include "Arduino.h"
#include "sobel_vector.h"

#define ROWS 120
#define COLS 160
#define ROW2 118
#define COL2 158
#define WINDOW_SIZE 3

void sobel_vector::sobel_algorithm(uint8_t (*im)[COLS])
{
  int** arr = new int * [ROW2];
  for (int t = 0; t < ROW2; ++t)
  {
    arr[t] = new int[COL2];
  }

  float mag = 0;
  float dir = 0;
  float vec_x = 0;
  float vec_y = 0;
  float vector = 0;
  for (int x = 1; x < ROW2; x++) // igmore first row since its black line
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
          mul_x = im[x + i][y + j] * Mx[i][j];
          sum_x = sum_x + mul_x;
          mul_y = im[x + i][y + j] * My[i][j];
          sum_y = sum_y + mul_y;
        }
      }

      // determine if edge point by thresholding
      if (arr)
      {
        if (arr[x])
        {
          if ((abs(sum_x > sobel_threshold)) || (abs(sum_y) > sobel_threshold))
          {
            arr[x][y] = 1;
            // calculate the direction of the edge
            if (sum_y == 0)
              dir = 0;
            else
            {
              if (sum_x == 0)
                dir = PI / 2;
              else
                dir = atan((double)sum_y / (double)sum_x);
            }

            // calculate the magnitude of the edge
            float f = (sum_x * sum_x) + (sum_y * sum_y);
            mag = sqrt(f);
            //printf("%f, ", dir);

            // calculate vector of point
            vec_x = vec_x + mag * cos(dir);
            //printf("vec_x = %f", vec_x);
                //Serial.print("row = ");
                //Serial.print(x);
                //Serial.print(" col = ");
                //Serial.println(y);
                //Serial.print("vector x = ");
                //Serial.println(vec_x);
            vec_y = vec_y + mag * sin(dir);
            //printf(" vec_y = %f\n", vec_y);
            //      Serial.print("vector y = ");
            //      Serial.println(vec_y);
            //      Serial.print(mag);
            //      Serial.print(", ");
          }
            
          else
            arr[x][y] = 0;
        }
      }

      
    }
    //    Serial.print("row = ");
    //    Serial.println(x);
    //    Serial.print("vector x = ");
    //    Serial.println(vec_x);
    //    Serial.print("vector y = ");
    //    Serial.println(vec_y);
    //    Serial.print("\n");
    //printf("\n");
  }



  vector = atan(vec_y / vec_x) * (180 / PI); // add vectors

  int offset = 0;
  int x = 0;
  while ((offset == 0) && x < (COL2 / 2))
  {
    if (arr[ROW2 / 2][x + (COL2 / 2)] == 1)
      offset = x;
    if (arr[ROW2 / 2][(COL2 / 2) - (x + 1)] == 1)
      offset = -1 * (x + 1);
    x++;
  }

  for (int i = 0; i < ROW2; i++)
    free(arr[i]);
  free(arr);

  results.angle = vector;
  results.offset = offset;
  //Serial.print("angle = ");
  //Serial.print(vector);

}
