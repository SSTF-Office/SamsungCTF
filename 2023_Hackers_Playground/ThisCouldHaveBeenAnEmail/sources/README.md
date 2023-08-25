## Handwriting detection from accelerometer data
This CTF task provides a toy example of Inertial Navigation problem solving. The player is provided with a 3-axis accelerometer dataset and is tasked to retrieve the device position in time, to recover handwriting on a whiteboard. For the sake of simplicity, data excludes any rotation, as if it was already corrected using gyroscope orientation tracking algorithm.
In a nutshell, the only task for user is to get rid of constant offset (both from gravity vector and IMU's own drift) and then double-integrate provided values to acquire coordinates. However, due to noise floor and no idle time to calibrate gravity vector, fine-tuning by hand is required (as shown in player_solve.py).

---
## Data generation
Acceleration data is generated in reverse to expected solution -- from coordinates.

1. Coordinates are [captured](data/about_data.md)
2. Differentiate twice to obtain acceleration
3. Add third dimension of zeroes
4. Rotate "the whiteboard" at a small angle so gravity doesn't exactly align with the plane of the writing
5. Add gravity vector pointing to the new "down"
6. Rotate "device" -- as IMU's orientation can be random relative to the world (like in a smartwatch)
7. Add (scaled) noise, captured from real IMU at idle, but scaled down so the problem is solvable

---
### Expected solution
[See Writeup](../solution/writeup.md)
