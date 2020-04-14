#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <FS.h>
#include "histogram_eq.h"
#include "ransac.h"
#include "sobel_vector.h"
#include "hough_transform.h"

// ESP-EYE camera module pin-out
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#define ROWS 120
#define COLS 160
#define ROW2 118
#define COL2 158
#define WINDOW_SIZE 3
#define PARAM_ROW 397
#define PARAM_COL 180
#define SOBEL_THRESHOLD 100
#define HOUGH_THRESHOLD 70
#define PI 3.14159265
#define RANSAC_THRESHOLD 20
#define ITERATIONS 50

uint8_t image[ROWS][COLS] = {0};
char line_result[20];

histogram_eq Histogram_eq = histogram_eq();
ransac Ransac = ransac();
sobel_vector Sobel_vector = sobel_vector();
hough_transform Hough_transform = hough_transform();

// Set access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String result()
{

  Serial.println("start loop");

  camera_fb_t * fb = NULL;
  uint8_t *ptr = NULL;

  fb = esp_camera_fb_get(); // put image in buffer
  if (!fb) {
    Serial.println("Camera capture failed");
  }

  // fill image[][] from buffer
  ptr = fb->buf;
  for (uint8_t x = 0; x < ROWS; x++)
  {
    for (uint8_t y = 0; y < COLS; y++)
    {
      image[x][y] = *ptr;
      ptr++;
      //Serial.print(image[x][y]);
      //Serial.print(", ");
    }
    //Serial.println();
  }

  Serial.println("equilise");

  esp_camera_fb_return(fb);

  Histogram_eq.equilise(image);

  unsigned long start_millis = millis();
  Serial.print("sobel ");
  Sobel_vector.sobel_threshold = SOBEL_THRESHOLD;
  Sobel_vector.sobel_algorithm(image);
  sprintf(line_result, "%f, %d", Sobel_vector.results.angle, Sobel_vector.results.offset);

//  Serial.print("ransac ");
//  Ransac.sobel_threshold = SOBEL_THRESHOLD;
//  Ransac.ransac_threshold = RANSAC_THRESHOLD;
//  Ransac.iterations = ITERATIONS;
//  Ransac.ransac_algorithm(image);
//  sprintf(line_result, "%d, %d", Ransac.results.m, Ransac.results.c);

//  Serial.print("hough ");
//  Hough_transform.sobel_threshold = SOBEL_THRESHOLD;
//  uint8_t** a = Hough_transform.sobel(image);
//  Hough_transform.hough_algorithm(a);
//  sprintf(line_result, "%d, %d", Hough_transform.results.rho, Hough_transform.results.theta);

  unsigned long stop_millis = millis();
  unsigned long time_millis = stop_millis - start_millis;
  Serial.println(time_millis);
  
  
  return line_result;
}

//******************************************************** setup ********************************************//
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // ESP-EYE camera module
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
  else
  {
    Serial.println("camera initialised");
  }

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/result", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", result().c_str());
  });

  // Start server
  server.begin();
  
  Serial.println("end setup");
}

void loop()
{

}
