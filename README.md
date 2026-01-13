<h1>
    sensor
</h1>

# ADXL345 Static Tilt & Motion Fundamentals

This project explores the fundamentals of using an ADXL345 accelerometer with Arduino as an early step toward a future autonomous drone platform. At this stage, the focus is **understanding raw sensor data**, **gravity**, and **static tilt**, not flight control.

---

## Hardware & Libraries

- **Sensor:** ADXL345 (3-axis accelerometer)
- **Communication:** I²C
- **Libraries Used:**
  - `Wire.h` – I²C communication
  - `Adafruit_Sensor.h` – Unified sensor interface
  - `Adafruit_ADXL345_U.h` – ADXL345 driver

---

## What an Accelerometer Measures

An accelerometer measures **linear acceleration** along three axes:

- **X:** left / right
- **Y:** forward / backward
- **Z:** up / down

When the sensor is stationary, the dominant acceleration measured is **gravity** (~9.81 m/s²).

---

## Static Condition Assumption

All calculations in this project assume:

- The sensor is **not moving**
- No vibration
- No acceleration except gravity

This condition is referred to as **static tilt**.

---

## Gravity Vector Sanity Check

For a stationary sensor:

|g| = sqrt(X² + Y² + Z²) ≈ 9.81 m/s²

Observed values around **9.3–9.8 m/s²** are normal due to:
- Sensor tolerance
- Range selection
- Minor mechanical tilt

---

## Why Raw Values Are Not Zero

When the sensor is flat on a desk, ideal readings would be:

X ≈ 0
Y ≈ 0
Z ≈ +9.81


In reality, small offsets are observed:

- Manufacturing tolerances
- PCB mounting angle
- Desk not perfectly level
- Sensor bias

These offsets are **expected** and must be calibrated.

---

## What “Level” Really Means

Level does **not** mean zero acceleration.

Level means:
- **Roll angle = 0°**
- **Pitch angle = 0°**
- Gravity aligned with the Z-axis

Gravity should *always* be present in the data.

---

## Static Tilt Angles

Using only accelerometer data, tilt angles can be computed from gravity.

### Standard Formulas

Roll = atan2(Y, Z)
<br>
Pitch = atan2(-X, sqrt(Y² + Z²))

- `atan2()` ensures correct sign and quadrant handling
- Angles are initially in radians

### Convert to Degrees

degrees = radians × 180 / π


---

## Example Arduino Implementation

```cpp
float ax = event.acceleration.x;
float ay = event.acceleration.y;
float az = event.acceleration.z;

float roll  = atan2(ay, az);
float pitch = atan2(-ax, sqrt(ay * ay + az * az));

float rollDeg  = roll  * 180.0 / PI;
float pitchDeg = pitch * 180.0 / PI;
```

# Calibration (Zeroing Angles)

Because no surface is perfectly level, a reference orientation must be defined.

When the board is in the desired “level” position:
```ccp
rollZero  = rollDeg
pitchZero = pitchDeg
```
Then subtract
```ccp
rollDeg  -= rollZero
pitchDeg -= pitchZero
```
This defines the current physical orientation as 0°, 0°.

# Continued Notes for Later
### Limitations of Accelerometer-Only Tilt

* Accelerometer-based tilt works only when stationary.
* It fails when:
    * The sensor is moved
    * The system accelerates
    * The platform vibrates

* This happens because accelerometers cannot distinguish between:

    * Gravity
    * Linear acceleration

### Why a Gyroscope Is Required

* A gyroscope measures angular velocity, not acceleration.
    * Accelerometer: absolute reference (gravity), noisy during motion
    * Gyroscope: smooth rotation sensing, but drifts over time

* Combining both is required for:
    * Stable orientation
    * Motion compensation
    * Flight control

This will be addressed in future work.