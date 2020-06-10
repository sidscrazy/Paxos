import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
#one proposer, 5-50 nodes. 
#5% failure rate, 2% machine failure
#myth 63
NODE_SCALE_LOW_FAIL = [(5, 0.793652), 
(10, 1.38079),
(15, 1.72451),
(20,  1.90061)
(25, 2.67765)
(30,3.14768)
(35,5.5033)
(40, 8.32407)
(45, 8.73985)
(50,)
]
#1-8 proposers, 15 nodes. 
#5% failure rate, 2% machine failure
#myth 63
PROPOSER_SCALE_LOW_FAIL = [
(1,),
(2,),
(3,),
(4,),
(5,),
(6,),
(7,),
(8,)
]

def plot (data, xr, yr, xl, yl, title, f):
	plt.close()
	plt.plot (data)
	plt.xlim (xr[0], xr[1])
	plt.ylim (yr[0], yr[1])
	plt.ylabel(yl)
	plt.xlabel(xl)
	plt.title('title')
	plt.savefig(f)
	plt.show ()

plot (NODE_SCALE_LOW_FAIL, )