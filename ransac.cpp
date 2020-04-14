#include "Arduino.h"
#include "ransac.h"

#define ROWS 120
#define COLS 160
#define ROW2 118
#define COL2 158
#define WINDOW_SIZE 3

void ransac::ransac_algorithm(uint8_t (*im)[COLS])
{
  //Serial.begin(115200);
  uint8_t** arr = new uint8_t * [ROW2];
  for (int i = 0; i < ROW2; ++i)
  {
    arr[i] = new uint8_t[COL2];
  }

  // loop through each pixel
  int count = 0;
  for (int x = 0; x < ROW2; x++)
  {
    for (int y = 0; y < COL2; y++)
    {
      // calculate sobel filter outputs
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

      // if output is above threshold then set to edge point
      if (arr)
      {
        if (arr[x])
        {
          if ((abs(sum_x > sobel_threshold)) || (abs(sum_y) > sobel_threshold))
          {
            count++;
            arr[x][y] = 1;
          }
          else
            arr[x][y] = 0;

        }
      }
      //printf("%d, ", arr[x][y]);
    }
    //printf("\n");
  }

  //printf("count = %d\n", count);
  int** coord = (int**)malloc(count * sizeof(int));
  for (int i = 0; i < count; i++)
    coord[i] = (int*)malloc(2 * sizeof(int));

  // put coordinates of edge points in an array
  int z = 0;
  for (int x = 0; x < ROW2; x++)
  {
    for (int y = 0; y < COL2; y++)
    {
      if (arr[x][y] == 1)
      {
        coord[z][0] = x;
        coord[z][1] = y;
        //printf("x = %d, y = %d\n", coord[z][0], coord[z][1]);
        z++;
      }
    }
  }
  
  for (int i = 0; i < ROW2; i++)
    free(arr[i]);
  free(arr);

  int maxInliers = 0; 
  float maxM = 0;
  float maxC = 0;
  float m = 0;
  float c = 0;

  for (int r = 0; r < iterations; r++)
  {
    int inliers = 0;
    // pick two random edge points and make line between two
    int num1 = (int)random(0, count);
    int num2 = random(0 ,count);
//    if (num1 == num2) // to make sure 2 different points are chosen
//      num2 = random(0, count + 1);
    //printf("x1 = %d, y1 = %d\n", coord[num1][0], coord[num1][1]);
    //printf("x2 = %d, y2 = %d\n", coord[num2][0], coord[num2][1]);
    float x1 = (float)coord[num1][0];
    float y1 = (float)coord[num1][1];
    float x2 = (float)coord[num2][0];
    float y2 = (float)coord[num2][1];

    while ((y2 - y1) == 0 || (num1 == num2))
    {
      num2 = random(0, count);
      x2 = (float)coord[num2][0];
      y2 = (float)coord[num2][1];
//      Serial.print("x2 = ");
//      Serial.print(x2);
//      Serial.print(" , y2 = ");
//      Serial.println(y2);
    }
    m = (x2 - x1) / (y2 - y1);
    c = x1 - (m * y1);

    //printf("x = %f y + %f\n", m, c);
//    Serial.print("x = ");
//    Serial.print(m);
//    Serial.print(", y = ");
//    Serial.println(c);

    for (int x = 0; x < count; x++)
    {
      // calculate distance between vector and line
      float a = -1;
      float b = m;
      float x0 = coord[x][0];
      float y0 = coord[x][1];
      float d = (abs((a * x0) + (b * y0) + c)) / (sqrtf((a * a) + (b * b)));
      //printf("x = %d, y = %d, d = %f\n", coord[x][0], coord[x][1], d);

      // increment the number of inliers that are within threshold
      if (d < ransac_threshold)
      {
        inliers++;
      }
    }
    //printf("m = %f, c = %f, inliers = %d\n", m, c, inliers);

    // save the m and c values with the highest number of inliers
    if (inliers > maxInliers)
    {
      maxInliers = inliers;
      maxM = m;
      maxC = c;
    }
  }

  for (int i = 0; i < count; i++)
    free(coord[i]);
  free(coord);
  
  results.m = maxM;
  results.c = maxC;
}
