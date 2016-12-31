import numpy as np
from matplotlib import pyplot as plt
import csv
import math


def load():
    f = open('../central/result.csv', 'rb')
    dataReader = csv.reader(f)

    x = []
    y = []
    leg_x = []
    leg_y = []
    cp_x = []
    cp_y = []
    for row in dataReader:
        x.append(float(row[0]))
        y.append(float(row[1]))
        leg_x.append(float(row[2]))
        leg_y.append(float(row[3]))
        cp_x.append(float(row[4]))
        cp_y.append(float(row[5]))

    x = np.array(x)
    y = np.array(y)
    leg_x = np.array(leg_x)
    leg_y = np.array(leg_y)
    cp_x = np.array(cp_x)
    cp_y = np.array(cp_y)
    return x, y, leg_x, leg_y, cp_x, cp_y

def main():
	(x, y, lx, ly, cx, cy) = load()
	plt.plot(x, y, 'o-')
	plt.plot(lx, ly, 'o')
	plt.plot(cx, cy, 'o-')
	plt.show()

if __name__ == '__main__':
    main()
