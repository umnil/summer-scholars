/**
 * Joy Servo
 * 
 * This code uses a joystick to control a servo motor using the angle of the motor
 * Because the servo motor only has a short angle change (~130°), the motor is only
 * mapped to the upper right portion on the joystick
 */

#include "math.h"

#define MOTORPIN 9
#define JOYX A15
#define JOYY A14
#define JOYS 52

// Used for calibration
double x0 = 0.0;
double y0 = 0.0;

/**
 * Computes the scaled value of the joystick. The incoming data will be between 0 and 1024, with the neurtral point being about 512V.
 * This scaules it down to [-1,1]
 */
double joyscale(double in) {
  return (in - 512.0) / 512.0;
}

/**
 * Calibrates the joystick
 */
void calibrate_joystick(void) {
  x0 = joyscale(analogRead(JOYX));
  y0 = joyscale(analogRead(JOYY)); 
}

/**
 * Read Joystick
 */
void read_joystick(double* buf) {
  buf[0] = joyscale(analogRead(JOYX)) - x0;
  buf[1] = (joyscale(analogRead(JOYY)) - y0) * -1.0;
}

double read_joyangle(void) {
  double xy[2] = {0.0, 0.0};
  read_joystick(xy);
  double x = xy[0];
  double y = xy[1];
  double angle = atan(y / x) * 180.0 / PI;

  // Angle correction for tangent calculation
  if (x < 0) {
    if (y < 0) {
      angle -= 180;
    } else {
      angle += 180;
    }
  }
  return angle;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTORPIN, OUTPUT);
  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JOYS, INPUT);
  Serial.begin(9600);

  // Zero out
  calibrate_joystick();
}

void loop() {
  // put your main code here, to run repeatedly
  double x = read_joyangle();

  // Map the angle to the servo motor
  double m = (185.0 - 75.0) / 90.0;
  double b = 75.0;
  int y = int(m * x + b);
   analogWrite(MOTORPIN, y);
}

