#include <Arduino.h>

//X axis stepper
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

//Y axis stepper
#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56

//Z axis stepper
#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62

//Limit switches
#define X_LIMIT 3
#define Y_LIMIT 14

//Variable delcarations
int32_t X_position_mm = 0; 
int32_t Y_position_mm = 0;

int32_t step_mm_conversion = 87; 
int32_t step_time = 600;

int32_t X_centre_position_mm = 138; 
int32_t Y_centre_position_mm = 104; 

int32_t X_step_count = 0; 
int32_t Y_step_count = 0; 
int32_t Z_step_count = 0;

int32_t negative_direction = LOW;
int32_t positive_direction = HIGH;


//Define functions in scope
void HOME();
void DRAW_SQUARE(int32_t square_origin_X_mm, int32_t square_origin_Y_mm, int32_t side_length_mm);
void DRAW_TRIANGLE(int32_t triangle_origin_X_mm, int32_t triangle_origin_Y_mm, int32_t side_length_mm);
void CENTRE();
void MOVE_X_DIRECTION(int32_t X_DIRECTION);
void MOVE_Y_DIRECTION(int32_t Y_DIRECTION);
void MOVE_XY_DIRECTION(int32_t X_DIRECTION, int32_t Y_DIRECTION);
void DRAW_CIRCLE(int32_t circle_origin_X_mm, int32_t circle_origin_Y_mm, int32_t circle_radius, float angle_delta);
void CALCULATE_STEPS_PER_MM();
void MOVE_Z_DIRECTION(int32_t Z_DIRECTION);


void setup()
{
  Serial.begin(9600);

  //circle_radius stepper set to output.
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

   //Z stepper set to output.
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);

  //y_point stepper set to output.
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  //circle_radius, y_point limit sensor pins set as input pullup pins to prevent floating.
  pinMode(X_LIMIT, INPUT_PULLUP);
  pinMode(Y_LIMIT, INPUT_PULLUP);

  //Activate the stepper motors to engage them.
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);
}

void loop()
{

  //CALCULATE_STEPS_PER_MM();
  MOVE_Z_DIRECTION(positive_direction); // Z positive = down
  HOME();
  DRAW_SQUARE(70, 70, 50);          //Draw square at co-ordinate Xmm, Ymm with a lenght of ?mm 
  MOVE_Z_DIRECTION(positive_direction); 
  DRAW_CIRCLE(90, 90, 1200, 0.05);  //Draw circle at co-ordinate Xmm, Ymm with a radius of ? and resolution of ? 
  MOVE_Z_DIRECTION(positive_direction);
  DRAW_TRIANGLE(100, 100, 30);      //Draw triangle at co-ordinate Xmm, Ymm with a lenght of ?mm 
  MOVE_Z_DIRECTION(positive_direction);
  CENTRE();
  while (1)
    ; //Freeze the main loop.
}
//----------------------------------------------------------------//


//Move the end effector along the Z axis.
void MOVE_Z_DIRECTION(int32_t Z_DIRECTION)
{
  while (Z_step_count < 6000)
  {
    digitalWrite(Z_DIR_PIN, Z_DIRECTION);
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(step_time);
    digitalWrite(Z_STEP_PIN, LOW);
    Z_step_count = Z_step_count + 1;
  }

  Z_step_count = 0;
}
//-----------------------------------------------------------//

//Move the end effector along the X axis.
void MOVE_X_DIRECTION(int32_t X_DIRECTION)
{

  digitalWrite(X_DIR_PIN, X_DIRECTION);
  digitalWrite(X_STEP_PIN, HIGH);
  delayMicroseconds(step_time);
  digitalWrite(X_STEP_PIN, LOW);
  X_step_count = X_step_count + 1;

  //Add 1 to X_position_mm after number of pulses reach step_mm_conversion
  if (X_step_count == step_mm_conversion)
  {

    X_step_count = 0;
    switch (X_DIRECTION)
    {

    case HIGH:
      X_position_mm = X_position_mm - 1;
      break;
    default:
      X_position_mm = X_position_mm + 1;
      break;
    }
  }
}
//-----------------------------------------------------//

//Move the end effector along the Y axis.
void MOVE_Y_DIRECTION(int32_t Y_DIRECTION)
{

  digitalWrite(Y_DIR_PIN, Y_DIRECTION);
  digitalWrite(Y_STEP_PIN, HIGH);
  delayMicroseconds(step_time);
  digitalWrite(Y_STEP_PIN, LOW);
  Y_step_count = Y_step_count + 1;
  //Add 1 to Y_position_mm after number of pulses reach step_mm_conversion
  if (Y_step_count == step_mm_conversion)
  {

    Y_step_count = 0;
    switch (Y_DIRECTION)
    {

    case HIGH:
      Y_position_mm = Y_position_mm + 1;
      break;
    default:
      Y_position_mm = Y_position_mm - 1;
      break;
    }
  }
}
//----------------------------------------------------//

//Move the end effector along the X and Y axis simultaneously.
void MOVE_XY_DIRECTION(int32_t X_DIRECTION, int32_t Y_DIRECTION)
{

  digitalWrite(X_DIR_PIN, X_DIRECTION);
  digitalWrite(Y_DIR_PIN, Y_DIRECTION);

  digitalWrite(X_STEP_PIN, HIGH);
  digitalWrite(Y_STEP_PIN, HIGH);
  delayMicroseconds(step_time);
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  X_step_count = X_step_count + 1;
  Y_step_count = Y_step_count + 1;

  //Add 1 to X_position_mm after number of pulses reach step_mm_conversion
  if (X_step_count == step_mm_conversion)
  {

    X_step_count = 0;
    switch (X_DIRECTION)
    {

    case HIGH:
      X_position_mm = X_position_mm - 1;
      break;
    default:
      X_position_mm = X_position_mm + 1;
      break;
    }
  }

 
  if (Y_step_count == step_mm_conversion)
  {

    Y_step_count = 0;
    switch (Y_DIRECTION)
    {

    case HIGH:
      Y_position_mm = Y_position_mm + 1;
      break;
    default:
      Y_position_mm = Y_position_mm - 1;
      break;
    }
  }
}
//---------------------------------------------------//


void DRAW_SQUARE(int32_t square_origin_X_mm, int32_t square_origin_Y_mm, int32_t side_length_mm)
{

  //Move towards square origin
  while (X_position_mm < square_origin_X_mm || Y_position_mm < square_origin_Y_mm)
  {

    if (X_position_mm < square_origin_X_mm)
    {
      MOVE_X_DIRECTION(negative_direction);
    }
    if (Y_position_mm < square_origin_Y_mm)
    {
      MOVE_Y_DIRECTION(positive_direction);
    }
  }

  MOVE_Z_DIRECTION(negative_direction);

//Draw square
  while (X_position_mm - square_origin_X_mm < side_length_mm
)
  {
    MOVE_X_DIRECTION(negative_direction);
  }
  while (Y_position_mm - square_origin_Y_mm < side_length_mm
)
  {
    MOVE_Y_DIRECTION(positive_direction);
  }
  while (X_position_mm - square_origin_X_mm > 0)
  {
    MOVE_X_DIRECTION(positive_direction);
  }
  while (Y_position_mm - square_origin_Y_mm > 0)
  {
    MOVE_Y_DIRECTION(negative_direction);
  }
}

//---------------------------------------------------------------------//

void DRAW_TRIANGLE(int32_t triangle_origin_X_mm, int32_t triangle_origin_Y_mm, int32_t side_length_mm)
{

  //Move towards triangle origin
  while (X_position_mm != triangle_origin_X_mm || Y_position_mm != triangle_origin_Y_mm)
  {

    if (X_position_mm < triangle_origin_X_mm)
    {
      MOVE_X_DIRECTION(negative_direction);
    }
    else if (X_position_mm > triangle_origin_X_mm)
    {
      MOVE_X_DIRECTION(positive_direction);
    }
    if (Y_position_mm < triangle_origin_Y_mm)
    {
      MOVE_Y_DIRECTION(positive_direction);
    }
    else if (Y_position_mm > triangle_origin_Y_mm)
    {
      MOVE_Y_DIRECTION(negative_direction);
    }
  }

  MOVE_Z_DIRECTION(negative_direction);

  //Draw triangle 
  while (Y_position_mm - triangle_origin_Y_mm != side_length_mm)
  {
    MOVE_XY_DIRECTION(negative_direction, positive_direction);
  }
  while (X_position_mm - triangle_origin_X_mm != -side_length_mm)
  {
    MOVE_X_DIRECTION(positive_direction);
  }
  while (Y_position_mm - triangle_origin_Y_mm != 0)
  {
    MOVE_XY_DIRECTION(negative_direction, negative_direction);
  }
}
//----------------------------------------------------------------------------//

void DRAW_CIRCLE(int32_t circle_origin_X_mm, int32_t circle_origin_Y_mm, int32_t circle_radius, float angle_delta)
{
  //Move towards triangle origin
  while (X_position_mm != circle_origin_X_mm || Y_position_mm != circle_origin_Y_mm)
  {

    if (X_position_mm < circle_origin_X_mm)
    {
      MOVE_X_DIRECTION(negative_direction);
    }
    else if (X_position_mm > circle_origin_X_mm)
    {
      MOVE_X_DIRECTION(positive_direction);
    }
    if (Y_position_mm < circle_origin_Y_mm)
    {
      MOVE_Y_DIRECTION(positive_direction);
    }
    else if (Y_position_mm > circle_origin_Y_mm)
    {
      MOVE_Y_DIRECTION(negative_direction);
    }
  }

  MOVE_Z_DIRECTION(negative_direction);
  
  int32_t y_point = 0;                                                  ///Ran out of time to convert circle radius to mm. Currently in steps. 
  const float pi = 3.14;                                                //Times the circle_radius_mm by step_mm_conversion should fix the
  for (float angle = 0.0; angle < 2.0 * pi; angle += angle_delta)       //issue user can then input circle radius in mm not steps
  {
    int32_t x_position = circle_radius * cos(angle);
    int32_t y_position = circle_radius * sin(angle);
    while (x_position != circle_radius || y_position != y_point)
    {
      if (x_position > circle_radius)
      {
        MOVE_X_DIRECTION(negative_direction);

        circle_radius += 1;
      }
      else if (x_position < circle_radius)
      {
        MOVE_X_DIRECTION(positive_direction);

        circle_radius -= 1;
      }
      if (y_position > y_point)
      {
        MOVE_Y_DIRECTION(positive_direction);

        y_point += 1;
      }
      else if (y_position < y_point)
      {
        MOVE_Y_DIRECTION(negative_direction);
        y_point -= 1;
      }
    }
    delayMicroseconds(600);
  }
}
//---------------------------------------------------------------------//

//Move end effector to home position
void HOME()
{

  //Move towards X axis home until the X axis limit switch is activated.
  while (digitalRead(X_LIMIT) == LOW)
  {
    MOVE_X_DIRECTION(positive_direction);
  }

  //Move towards Y axis home until the Y axis limit switch is activated.
  while (digitalRead(Y_LIMIT) == LOW)
  {
    MOVE_Y_DIRECTION(negative_direction);
  }

  
  if (digitalRead(X_LIMIT) == HIGH && digitalRead(Y_LIMIT) == HIGH)
  {
    X_position_mm = 0;
    Y_position_mm = 0;
  }
}
//---------------------------------------------------------------------------//

//Move end effector to the centre workspace
void CENTRE()
{
  while (X_position_mm != X_centre_position_mm || Y_position_mm != Y_centre_position_mm)
  {

    if (X_position_mm < X_centre_position_mm)
    {
      MOVE_X_DIRECTION(negative_direction);
    }
    else if (X_position_mm > X_centre_position_mm)
    {
      MOVE_X_DIRECTION(positive_direction);
    }
    if (Y_position_mm < Y_centre_position_mm)
    {
      MOVE_Y_DIRECTION(positive_direction);
    }
    else if (Y_position_mm > Y_centre_position_mm)
    {
      MOVE_Y_DIRECTION(negative_direction);
    }
  }
  MOVE_Z_DIRECTION(negative_direction);
}
//-----------------------------------------------------------------------------//


//Step to mm calabration 
void CALCULATE_STEPS_PER_MM()
{

  X_position_mm = 0;
  Y_position_mm = 0;

  while (millis() < 5000)
  {
    MOVE_Y_DIRECTION(positive_direction);
  }
  Serial.print("Line Y 1 is: ");
  Serial.print(Y_position_mm);
  Serial.println("mm ");

  while (millis() < 10000)
  {
    MOVE_X_DIRECTION(positive_direction);
  }
  Serial.print("Line X 1 is: ");
  Serial.print(X_position_mm);
  Serial.println("mm ");

  Y_position_mm = 0;

  while (millis() < 15000)
  {
    MOVE_Y_DIRECTION(negative_direction);
  }
  Serial.print("Line Y 2 is: ");
  Serial.print(Y_position_mm);
  Serial.println("mm ");
}
