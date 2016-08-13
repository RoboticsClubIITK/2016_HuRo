
import time
import pyaudio
import wave
low_cut=100
high_cut=1000

CHUNK = 1024*3 #Chunk is the bytes which are currently processed
FORMAT = pyaudio.paInt16
RATE = 44100
RECORD_SECONDS = 2
WAVE_OUTPUT_FILENAME1 = "output1.wav"
WAVE_OUTPUT_FILENAME2 = "output2.wav"
p = pyaudio.PyAudio()
stream1 = p.open(format=FORMAT,
		channels=1,
		rate=RATE,
		input=True,
		frames_per_buffer=CHUNK,
		input_device_index=4)
stream2 = p.open(format=FORMAT,
		channels=1,
		rate=RATE,
		input=True,
		frames_per_buffer=CHUNK,
		input_device_index=5)


print("* recording")

frames1 = [] #np array storing all the data
frames2 = []
for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)): #simple maths for number of iterations for the loop
    data1 = stream1.read(CHUNK)
    data2 = stream2.read(CHUNK)
    frames1.append(data1)
    frames2.append(data2)
    
print("* done recording")

stream1.stop_stream()
stream2.stop_stream()
stream1.close()
stream2.close()
p.terminate()

wf1 = wave.open(WAVE_OUTPUT_FILENAME1, 'wb') #opening the file
wf2 = wave.open(WAVE_OUTPUT_FILENAME2, 'wb') 
wf1.setnchannels(1)
wf2.setnchannels(1)
wf1.setsampwidth(p.get_sample_size(FORMAT))
wf2.setsampwidth(p.get_sample_size(FORMAT))
wf1.setframerate(RATE)
wf2.setframerate(RATE)
wf1.writeframes(b''.join(frames1))#writing the data to be saved
wf2.writeframes(b''.join(frames2))
wf1.close()
wf2.close()


import numpy as np
import scipy.io.wavfile
from scipy import signal
import scipy
import matplotlib.pyplot as plt

speed_of_sound=350 #set speed of sound in m/sec.
dist_btw_mics=30 #set distance between mics in cm.

rate1,data1 = scipy.io.wavfile.read("output1.wav");
rate2,data2 = scipy.io.wavfile.read("output2.wav");
check1=0
check2=0
for i in range(0,len(data1)):# implementing band pass filter to reduce the noise
    if(data1[i]<200 and data1[i]>-200):
	 data1[i]=0
    else:
	 check1=1

for j in range(0,len(data2)):
   if(data2[j]<200 and data2[j]>-200):
	 data2[j]=0
   else:
	 check2=1

num_of_samples1 = data1.size
num_of_samples2 = data2.size
duration_of_signal1 = float(num_of_samples1)/rate1
duration_of_signal2 = float(num_of_samples2)/rate2
print duration_of_signal1
print duration_of_signal2



t1 = np.arange(0,duration_of_signal1,1.0/rate1) #Duration of signal is divided

t2 = np.arange(0,duration_of_signal2,1.0/rate2)

plt.subplot(3,1,1)
plt.plot(t1,data1)
plt.xlabel("Time")
plt.ylabel("Amplitude")
plt.subplot(3,1,2)
plt.plot(t2,data2)
plt.xlabel("Time")
plt.ylabel("Amplitude")
start=len(data1)
end=len(data1)

fdata1 = scipy.fft(data1)#calculating FFT of both the signals
fdata2 = scipy.fft(data2)
'''
ffdata1=np.lib.pad(fdata1,(len(data1),len(data1)),'constant',constant_values=(0,0))
ffdata2=np.lib.pad(fdata2,(len(data1),len(data1)),'constant',constant_values=(0,0))

fdata1 = scipy.fft(data1)
fdata2 = scipy.fft(data2)
fdata1[:low_cut]=0
fdata1[high_cut:]=0
fdata2[:low_cut]=0
fdata2[high_cut:]=0
'''
corr = scipy.ifft(fdata1 * scipy.conj(fdata2))#correlating the two signals in frequency domain and calculating the inverse FFT

t=np.linspace(0,(len(corr)/44100),len(corr))


shift_frames= np.argmax(abs(corr))


time_shift=(shift_frames/44100.0)
if(time_shift>1):
   time_shift=(time_shift-duration_of_signal1)# assigning direction to the time lag
print(time_shift)
if (time_shift<0):
    dir='L'
else:
    dir='R'

dist_covered = time_shift * speed_of_sound

angle_of_source=np.arcsin((dist_covered*100)/dist_btw_mics)*(180/np.pi)#basic trigonometry for calculating angle_of_source
print(angle_of_source)

plt.subplot(3,1,3)
plt.plot(t,abs(corr))
plt.xlabel("Time")
plt.ylabel("Correlation")
plt.show()
'''
fo=open("s_loc.txt","w")
if(check1 and check2):
  fo.write('1' + '\n' + dir)
else:
  fo.write('0')

fo.close()
'''

