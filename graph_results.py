import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
#one proposer, 5-50 nodes. 
#5% failure rate, 2% machine failure
#myth 63
NODE_SCALE_LOW_FAIL = [
(5, 0.198357), 
(10, 0.488691),
(15, 0.816268),
(20,  0.967529),
(25, 1.88506),
(30, 1.75283),
(35, 2.75955),
(40, 2.90222),
(45, 3.58492),
(50, 5.05761),
]
#1-8 proposers, 15 nodes. 
#5% failure rate, 2% machine failure
#myth 63
PROPOSER_SCALE_LOW_FAIL = [
(1, 0.736943),
(2, 0.704526),
(3, 0.73005),
(4, 0.701995),
(5, 0.697743),
(6, 0.668605),
(7, 0.609713),
(8, 0.56605)
]

NODE_SCALE_HIGH_FAIL = [
(5, 9.00261), 
(10, 9.01139),
(15, 9.62561),
(20,  10.0114),
(25, 10.0295),
(30,10.9565),
(35,11.8791),
(40, 12.493),
(45, 12.7495),
(50, 14.2063),
]

PROPOSER_SCALE_HIGH_FAIL = [
(1, 9.60234),
(2, 9.73579),
(3, 100),
(4, 100),
(5, 100),
(6, 100),
(7, 100),
(8, 100)
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
plot (NODE_SCALE_HIGH_FAIL, (0, 55), (8.5, 14), "Nodes", "Average Consensus Time (Seconds)", "Performance with Increasing Node Count - High Failure Rate", "node_high.png")
plot (PROPOSER_SCALE_HIGH_FAIL, (0, 9), (8.5, 12), "Proposers", "Average Consensus Time (Seconds)", "Performance with Increasing Proposer Count - High Failure Rate", "prop_high.png")