import matplotlib.pyplot as plt
import numpy as np
from scipy.fftpack import fftfreq
from scipy.fftpack import fft
import math

N = 600   # Number of samplepoints
T = 1.0 / 800.0# sample spacing
t = np.linspace(0.0, N*T, N)#Arranging np array
y = 10 + np.sin(5.0 * 2.0*np.pi*t) + 0.5*np.sin(200.0 * 2.0*np.pi*t)#Sine waves
yf = fft(y)
yf=np.abs(yf)#yf is a complex value
freq = np.linspace(0.0, 1/(2*T), N/2)#N/2 because first half of array contains positive frequencies


plt.plot(freq[1:], 2.0/N * np.abs(yf[0:N/2])[1:])#yf(0) is a large value , therefore for scaling of our graph we start from '1' index
plt.show()



