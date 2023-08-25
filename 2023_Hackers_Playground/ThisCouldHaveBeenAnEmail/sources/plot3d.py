from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt


def plot_3d_scatter(data, title, ax=None, call_show=True):
    if ax is None:
        fig = plt.figure()
        ax = fig.add_subplot(projection='3d')
    else:
        ax.clear()

    _ = data.T
    ax.plot3D(_[0], _[1], _[2])
    ax.set_aspect('equal')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.ticklabel_format(style='plain')
    plt.title(title, size=20)

    if call_show:
        plt.show()
