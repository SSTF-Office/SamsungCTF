#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
from plot3d import plot_3d_scatter


def integrate(accels):
    velocities = np.cumsum(accels, axis=0)
    coords = np.cumsum(velocities, axis=0)
    return coords


accels = np.loadtxt('smartwatch.cap', delimiter=' ')
plot_3d_scatter(integrate(accels), 'Only Integration')
