#!/usr/bin/python3

import numpy as np
from scipy.spatial.transform import Rotation
from plot3d import plot_3d_scatter


def generate(coords, noise_sample=None, noise_coefficient=1, gravity_amount=1):
    # Stages:

    # Differentiate twice
    velocities = np.diff(coords, axis=0)
    accels = np.diff(velocities, axis=0)
    accels *= 15

    # Add third dimension
    normal_axis_data = np.zeros((accels.shape[0], 1))
    accels = np.hstack((accels, normal_axis_data))

    # Rotate "the whiteboard" (probably a small angle)
    desk_rotation_axis = np.array([0.8, 0.3, 0.4])
    theta = np.pi/2
    rot = Rotation.from_rotvec(theta * desk_rotation_axis)
    accels = rot.apply(accels)

    # Add gravity vector
    gravity = np.array([0.0, 0.0, -gravity_amount])
    accels += gravity

    # Rotate "device" (random angle)
    device_rotation_axis = np.array([0.3, 0.2, 0.1])
    theta = 10
    rot = Rotation.from_rotvec(theta * device_rotation_axis)
    accels = rot.apply(accels)

    # Add (scaled) noise
    if noise_sample is not None:
        # Match noise size
        if noise_sample.shape[1] != 3:
            raise ValueError
        noise_sample = np.resize(noise_sample, accels.shape)

        noise_sample -= np.average(noise_sample, axis=0)  # Remove DC component from noise
        noise_sample *= noise_coefficient  # Scale

        noise_rms = np.sqrt(np.mean(np.linalg.norm(noise_sample, axis=1) ** 2))
        data_rms = np.sqrt(np.mean(np.linalg.norm(accels, axis=1) ** 2))
        print(f'Signal/Noise ratio is {data_rms/noise_rms}')

        accels += noise_sample

    return accels


# Just hardcoded perfect values, noise is just accepted
# (it doesn't have any DC component, so gravity subtraction takes off all drift)
def perfect_solve(accels):
    device_rotation_axis = np.array([0.3, 0.2, 0.1])
    theta = -10
    rot = Rotation.from_rotvec(theta * device_rotation_axis)
    accels = rot.apply(accels)

    gravity = np.array([0.0, 0.0, -1])
    accels -= gravity

    velocities = np.cumsum(accels, axis=0)
    coords = np.cumsum(velocities, axis=0)
    return coords


if __name__ == '__main__':

    coords = np.loadtxt('data/handwrite_0005.csv', delimiter=',', skiprows=1, usecols=(1, 2))
    noise_sample = np.loadtxt('data/mpu_6050_capture_idle.cap', delimiter=' ')

    np.random.seed(1335)
    np.random.shuffle(noise_sample)  # Helps to avoid unreadable symbols

    generated_data = generate(coords, noise_sample=noise_sample, noise_coefficient=0.2)
    generated_data = generated_data[:-200]

    plot_3d_scatter(perfect_solve(generated_data), "Generated Data")
    np.savetxt('smartwatch.cap', generated_data, fmt='%f')
