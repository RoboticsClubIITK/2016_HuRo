# Voice-localisation
Sound localization included finding the angle of the sound source with respect to the humanoid.It was implemented using Python 
and its libraries like Numpy,Scipy and Matplotlib.The time lag between the two microphones was calculated using some 
mathematical tools such as Cross-correlation and FFT of a signal.

Read live audio from microphones in small chunks and stored the data in Numpy arrays ,saving it as a "wav" file.

Input the data from "wav" file and implementing a band pass filter to reduce noise in the audio signal

As the data size was too large , we cross-correlated the two signals in the frequency domain and took the Inverse FFT of the product. 

Using basic trigonometry the angle between the sound source and humanoid was calculated successfully.
