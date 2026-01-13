#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
/* 
  accel is the name of the sensor - aka how we reach it.
  
  12345 is just a random Id. 
  This is important when we have multiple sensors.
*/
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// ----- Setup Options -----
float alpha = 0.7;
int accelRange = ADXL345_RANGE_4_G;

// ----- Function Declarations -----
void setupAccelerometer();
sensors_event_t readAccelEvent();
void computeAccelAngles(sensors_event_t event);
void printRawAccel(sensors_event_t event);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // For Leonardo/Micro/Zero
  Serial.println("Accelerometer Startup");

  // Initialize sensor
  setupAccelerometer();

  Serial.println("Setup Complete\n");
}

void loop() {
  sensors_event_t event = readAccelEvent();
  computeAccelAngles(event);
  // 3. Print raw values (optional)
  // printRawAccel(event);

  delay(200);
}

// -------------------- Functions --------------------

// Setup sensor and options
void setupAccelerometer() {
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1)
      ;
  }
  accel.setRange(accelRange);
  Serial.println("Accelerometer initialized");
}

// Read the latest event
sensors_event_t readAccelEvent() {
  sensors_event_t event;
  accel.getEvent(&event);
  return event;
}

// Compute roll/pitch angles, apply low-pass filter
void computeAccelAngles(sensors_event_t event) {
  // Persistent filtered values
  static float rollFiltered = 0.0;
  static float pitchFiltered = 0.0;

  // Raw acceleration
  float ax = event.acceleration.x;
  float ay = event.acceleration.y;
  float az = event.acceleration.z;

  // Compute angles in degrees
  float roll  = atan2(ay, az) * 180.0 / PI;
  float pitch = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0 / PI;

  // Apply optional low-pass filter
  rollFiltered  = alpha * rollFiltered  + (1 - alpha) * roll;
  pitchFiltered = alpha * pitchFiltered + (1 - alpha) * pitch;

  // Output
  Serial.print("Roll: "); Serial.print(rollFiltered);
  Serial.print("  Pitch: "); Serial.println(pitchFiltered);
}

// Print raw acceleration data
void printRawAccel(sensors_event_t event) {
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");
}








// "Old" code that will be scavenged

// This is static data about the sensor, not motion data.
void displaySensorDetails(void) {
  // sensor_t is a struct for the sensor
  sensor_t sensor;
  // getSensor is a exension that populates the struct
  accel.getSensor(&sensor);
  // print out some data that does two things
  //  1 confirms the sensor is working
  //  2 confirms how the sensor is working
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// This function shows how often the sensor is updating the data.
void displayDataRate(void) {
  Serial.print("Data Rate:    ");

  switch (accel.getDataRate()) {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print("0.10 ");
      break;
    default:
      Serial.print("???? ");
      break;
  }
  Serial.println(" Hz");
}

// The function shows the current range the sensor is set to
// +/- 2g - very sensitive to small motions
// +/- 16g - less sensitive but handles strong motions
void displayRange(void) {
  Serial.print("Range:         +/- ");

  switch (accel.getRange()) {
    case ADXL345_RANGE_16_G:
      Serial.print("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print("2 ");
      break;
    default:
      Serial.print("?? ");
      break;
  }
  Serial.println(" g");
}

// This is the init function - known as setup now
void setup(void) {
#ifndef ESP8266
  while (!Serial)
    ;  // for Leonardo/Micro/Zero
#endif
  // sets the baud rate
  Serial.begin(9600);
  Serial.println("Accelerometer Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1)
      ;
  }

  /* Set the range to whatever is appropriate for your project */
  //accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  accel.setRange(ADXL345_RANGE_4_G);
  //accel.setRange(ADXL345_RANGE_2_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

// The loop continously reads the sensor
void loop(void) {
  /* Get a new sensor event */
  // event is a struct holding live acceleration data
  sensors_event_t event;
  // fills event with the latest data
  accel.getEvent(&event);

  //startingOutputFromExampleFile(event);
  computeAccelAngles(event);

  // don't flood anyone
  delay(500);
}

/* Display the results (acceleration is measured in m/s^2) */
void startingOutputFromExampleFile(sensors_event_t event) {
  // Left/Right
  Serial.print("X: ");
  Serial.print(event.acceleration.x);
  Serial.print("  ");
  // Front/Back
  Serial.print("Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("  ");
  // Up/Down
  Serial.print("Z: ");
  Serial.print(event.acceleration.z);
  Serial.print("  ");
  // Print the unit of measurement as well.
  Serial.println("m/s^2 ");
}

void computeAccelAngles(sensors_event_t event) {
  static float rollFiltered = 0.0;
  static float pitchFiltered = 0.0;

  // event.acceleration.value is already a float but we are explict, for now.
  float ax = event.acceleration.x;
  float ay = event.acceleration.y;
  float az = event.acceleration.z;

  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float rollDeg = roll * 180.0 / PI;
  float pitchDeg = pitch * 180.0 / PI;

  /* 
    smoothing with a low pass filter 
      0.98	Very smooth, very laggy
      0.9	Smooth, noticeable lag
      0.7	Balanced
      0.5	Responsive
      0.2	Barely filtered
  */
  float alpha = 0.7;
  rollFiltered = alpha * rollFiltered + (1 - alpha) * rollDeg;
  pitchFiltered = alpha * pitchFiltered + (1 - alpha) * pitchDeg;

  Serial.print("Roll: ");
  //Serial.print(rollDeg);
  Serial.print(rollFiltered);
  Serial.print(" deg  ");

  Serial.print("Pitch: ");
  //Serial.print(pitchDeg);
  Serial.print(pitchFiltered);
  Serial.println(" deg");
}
