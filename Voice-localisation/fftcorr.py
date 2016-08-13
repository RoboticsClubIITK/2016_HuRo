import numpy as np
import scipy.io.wavfile
from scipy import signal
import scipy
import matplotlib.pyplot as plt
rate1,data1 = scipy.io.wavfile.read("mono1.wav");#reading 2 wave files
rate2,data2 = scipy.io.wavfile.read("mono2.wav");
num_of_samples1 = data1.size
num_of_samples2 = data2.size
duration_of_signal1 = float(num_of_samples1)/rate1 
duration_of_signal2 = float(num_of_samples2)/rate2
t1 = np.linspace(0, duration_of_signal1, num_of_samples1)#Arranging np array
t2 = np.linspace(0, duration_of_signal2, num_of_samples2)#Arranging np array
plt.subplot(3, 1, 1)
plt.plot(t1, data1)
plt.xlabel("Time")
plt.ylabel("Amplitude")
plt.subplot(3, 1, 2)
plt.plot(t2, data2)
plt.xlabel("Time")
plt.ylabel("Amplitude")
fdata1 = scipy.fft(data1)#FFT of data for cross correlation
fdata2 = scipy.fft(data2)#FFT helps in fast processing
corr = scipy.ifft(fdata2 * scipy.conj(fdata1))#Conjugate basically does the time reversal of signal in frequency domain
time_shift = np.argmax(abs(corr))
t=np.linspace(0,(len(corr)/44100),len(corr))
plt.subplot(3,1,3)
plt.plot(t,abs(corr))
plt.xlabel("Time")
plt.ylabel("Correlation")
plt.show()

print(time_shift/44100.0)#Sample at offset / sampling rate = Time
