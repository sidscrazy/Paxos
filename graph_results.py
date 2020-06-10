import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
#one proposer, 5-50 nodes. 
#5% failure rate, 2% machine failure
#myth 63
NODE_SCALE_LOW_FAIL = [(5, 0.793652), 
(10, 1.38079),
(15, 1.72451),
(20,  1.90061),
(25, 2.67765),
(30,3.14768),
(35,5.5033),
(40, 8.32407),
(45, 8.73985),
(50, 9.18241),
]
#1-8 proposers, 15 nodes. 
#5% failure rate, 2% machine failure
#myth 63
PROPOSER_SCALE_LOW_FAIL = [
(1, 1.50695),
(2, 1.51098),
(3, 1.53151),
(4, 1.53042),
(5, 1.56312),
(6, 1.58356),
(7, 1.61133),
(8, 1.63268)
]

def convert_input (inp_arr):
	x = [a[0] for a in inp_arr]
	y = [a[1] for a in inp_arr]
	return x, y


def plot (data, xr, yr, xl, yl, title, f):
	plt.close()
	x, y = convert_input (data)
	plt.plot (x, y)
	plt.xlim (xr[0], xr[1])
	plt.ylim (yr[0], yr[1])
	plt.ylabel(yl)
	plt.xlabel(xl)
	plt.title(title)
	plt.savefig(f)
	plt.show ()

plot (NODE_SCALE_LOW_FAIL, (0, 60), (0, 10), "Nodes", "Average Consensus Time (Seconds)", "Performance with Increasing Node Count - Low Failure Rate", "node_low.png")
plot (PROPOSER_SCALE_LOW_FAIL, (0, 10), (1.3, 1.8), "Proposers", "Average Consensus Time (Seconds)", "Performance with Increasing Proposer Count - Low Failure Rate", "prop_low.png")