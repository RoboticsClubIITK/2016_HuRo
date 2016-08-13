'''
numpy library is used for arrays as there is no direct implmentation
of arrays in python instead there are list which are slow in processing
moreover the data returned from scipy is numpy array so we need to use 
them anyway. scipy is a signal processing library which is able to read 
data from wavfile and so we are using it. matplotlib is used for plotting
graphs and curves. With these 3 librarires most of matlab work can be 
implemented in python also.
'''
import numpy as np
import scipy.io.wavfile
import matplotlib.pyplot as plt

# scipy.io.wavfile.read function reads wav file and return the rate,data
# rate is an integer and data is a numpy integer array
rate,data = scipy.io.wavfile.read("output.wav");

# here we are taking size of array to know how many points do we need to plot
# and then we are finding the time duration of signal(simple mathematics)
num_of_samples = data.size
duration_of_signal = float(num_of_samples)/rate
print duration_of_signal

'''
np.arrange function is used to populate an array from given index to final position with a difference of specified step length aand so we are using that to make our time axis. 
Note that total number of points on time axis should be equal to length
of data array to plot the graph
'''
t = np.arange(0,duration_of_signal/2,1.0/rate) #Duration of signal is divided by 2 because of stereo sound
# these are functions to plot the graph
plt.plot(t,data)
plt.xlabel("Sound")
plt.ylabel("Amplitude")
plt.show()
