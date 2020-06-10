import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
#one proposer, 5-50 nodes. 
#5% failure rate, 2% machine failure
#myth 63
NODE_SCALE_LOW_FAIL = [
(5, 1.52619), 
(10, 0.409226),
(15, 0.695585),
(20,  0.970267),
(25, 1.51412),
(30, 1.54667),
(35, 2.409),
(40, 2.70895),
(45, 3.40393),
(50, 4.74162),
]
#1-8 proposers, 15 nodes. 
#5% failure rate, 2% machine failure
#myth 63
PROPOSER_SCALE_LOW_FAIL = [
(1, 0.706151),
(2, 0.647582),
(3, 0.656166),
(4, 0.622364),
(5, 0.587349),
(6, 0.561615),
(7, 0.544483),
(8, 0.491702)
]

NODE_SCALE_HIGH_FAIL = [
(5, 2.3024), 
(10, 2.33023),
(15, 2.45719),
(20,  2.60993),
(25, 3.07404),
(30,3.04505),
(35,3.6231),
(40, 3.66753),
(45, 4.05759),
(50, 4.79125),
]

PROPOSER_SCALE_HIGH_FAIL = [
(1, 2.43694),
(2, 2.4343),
(3, 2.44477),
(4, 2.41109),
(5, 4.28203),
(6, 4.41835),
(7, 5.22306),
(8, 5.34827)
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

plot (NODE_SCALE_LOW_FAIL, (0, 55), (0, 6), "Nodes", "Average Consensus Time (Seconds)", "Performance with Increasing Node Count - Low Failure Rate", "node_low.png")
plot (PROPOSER_SCALE_LOW_FAIL, (0, 9), (0.3, 0.8), "Proposers", "Average Consensus Time (Seconds)", "Performance with Increasing Proposer Count - Low Failure Rate", "prop_low.png")
plot (NODE_SCALE_HIGH_FAIL, (0, 55), (2, 6), "Nodes", "Average Consensus Time (Seconds)", "Performance with Increasing Node Count - High Failure Rate", "node_high.png")
plot (PROPOSER_SCALE_HIGH_FAIL, (0, 9), (2, 6), "Proposers", "Average Consensus Time (Seconds)", "Performance with Increasing Proposer Count - High Failure Rate", "prop_high.png")