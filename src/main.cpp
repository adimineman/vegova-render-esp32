#include <Arduino.h>
//#include <SPI.h>
//#include <Wire.h>
#include <MatrixMath.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tools.h"
#include "pic.h"

#define SSD1306_NO_SPLASH

#define MX_PNT 512

#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

point cloud[MX_PNT];
float jx, jy;

void setup()
{
  /*for (int a = 0; a < MX_PNT; a++)
  {
    long angl = random(0, 360);
    cloud[a].poz[0] = cos(angl),
    cloud[a].poz[1] = sin(angl);
    cloud[a].color = 1;
  }
  for (int a = 0; a < 10; a++)
    for (int b = 0; b < 10; b++)
      cloud[a * 10 + b].poz[0] = a / 10. - .5,
                     cloud[a * 10 + b].poz[1] = .5,
                     cloud[a * 10 + b].poz[2] = b / 10. - .5,
                     cloud[a * 10 + b].color = 1;*/
  for (int a = 0; a < MX_PNT; a++)
  {
    while (cloud[a].color != 1)
    {
      float x = random(1023) / 1024., y = random(1023) / 1024.;
      cloud[a].poz[0] = x - .5,
      cloud[a].poz[1] = y - .5;
      //cloud[a].poz[1] = sin(cloud[a].poz[2] * 6.) * sin(cloud[a].poz[0] * 6.) / 4.;
      cloud[a].color = COLOFPIC(x, y);
    }
  }
  for (int a = 0; a < 100; a++)
  {
    cloud[a].poz[0] = (random(1023) - 512.) * 2. / 1024.,
    cloud[a].poz[1] = (random(1023) - 512.) * 2. / 1024.;
    cloud[a].poz[2] = (random(1023) - 512.) * 2. / 1024.;
    cloud[a].color = random(2);
  }
  Serial.begin(115200);
  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    delay(1000);
  }
  display.display();
}

void joysticc()
{
  float x = (analogRead(A0) - 2048.) / 2048.;
  float y = (analogRead(A3) - 2048.) / 2048.;
  if (abs(x) < .1 && abs(y) < .1)
    return;
  double a = sqrt(x * x + y * y);
  if (a > 1.)
  {
    x /= a;
    y /= a;
  }
  x /= 10, y /= 10;
  jx += x, jy -= y;
  jy = constrain(jy, -PI / 2, PI / 2);
}

void render()
{
  display.clearDisplay();
  initzbuff();
  mtx_type temp[3];
  for (int a = 0; a < MX_PNT; a++)
  {
    Matrix.Copy(cloud[a].poz, 3, 1, temp);
    rotY((mtx_type *)temp, jx);
    rotX((mtx_type *)temp, jy);
    transform((mtx_type *)temp, 0, 0, -1);
    project((mtx_type *)temp, 1.5);

    draw(display.getBuffer(), temp, cloud[a].color);
  }
  display.display();
}

void loop()
{
  render();
  joysticc();
}