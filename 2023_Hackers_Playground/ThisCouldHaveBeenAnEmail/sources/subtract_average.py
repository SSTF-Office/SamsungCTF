#!/usr/bin/python3

import numpy as np
from plot3d import plot_3d_scatter


def integrate(accels):
    velocities = np.cumsum(accels, axis=0)
    coords = np.cumsum(velocities, axis=0)
    return coords


accels = np.loadtxt('smartwatch.cap', delimiter=' ')

gravity_from_average = np.average(accels, axis=0)
accels -= gravity_from_average

plot_3d_scatter(integrate(accels), 'Subtract Average')
