#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
import matplotlib.gridspec as gridspec
from mpl_toolkits.mplot3d import Axes3D
from plot3d import plot_3d_scatter


def integrate(accels):
    velocities = np.cumsum(accels, axis=0)
    coords = np.cumsum(velocities, axis=0)
    return coords


def main():
    accels = np.loadtxt('smartwatch.cap', delimiter=' ')

    gravity_from_average = np.average(accels, axis=0)
    accels -= gravity_from_average

    fig = plt.figure()
    gs = gridspec.GridSpec(4, 1, height_ratios=[1.0, 0.02, 0.02, 0.02])
    ax = plt.subplot(gs[0], projection='3d', title='asd')

    slider_x = Slider(ax=plt.subplot(gs[1]), label='X', valmin=-0.0005, valmax=0.0005, valinit=0, orientation='horizontal')
    slider_y = Slider(ax=plt.subplot(gs[2]), label='Y', valmin=-0.0005, valmax=0.0005, valinit=0, orientation='horizontal')
    slider_z = Slider(ax=plt.subplot(gs[3]), label='Z', valmin=-0.0005, valmax=0.0005, valinit=0, orientation='horizontal')

    callback = lambda val : plot_3d_scatter(integrate(accels - np.array([slider_x.val, slider_y.val, slider_z.val])), None, ax, False)

    slider_x.on_changed(callback)
    slider_y.on_changed(callback)
    slider_z.on_changed(callback)

    callback(None)
    plt.show()


if __name__ == '__main__':
    main()
