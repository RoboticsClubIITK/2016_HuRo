import speech_recognition as sr
import numpy as np
import scipy.io.wavfile
import matplotlib.pyplot as plt

# obtain audio from the microphone
r = sr.Recognizer()
with sr.Microphone() as source:
    print("Say something!")
    r.dynamic_energy_threshold=True
    #r.energy_threshold = 200
    r.adjust_for_ambient_noise(source, 0.5)     # listen for 0.5 second to calibrate the energy threshold for ambient noise levels
    r.pause_threshold = 0.5
    r.dynamic_energy_adjustment_ratio = 2.5     # speech is louder than ambedient noise by a factor of 2.5
    audio = r.listen(source)
    
    # write audio to a WAV file
with open("microphone-results.wav", "wb") as f:
    f.write(audio.get_wav_data())

# recognize speech using Sphinx
try:
    print("Sphinx thinks you said = " + r.recognize_sphinx(audio))
except sr.UnknownValueError:
    print("Sphinx could not understand audio")
except sr.RequestError as e:
    print("Sphinx error; {0}".format(e))

rate,data = scipy.io.wavfile.read("microphone-results.wav");

num_of_samples = data.size
duration_of_signal = float(num_of_samples)/rate

t = np.arange(0,duration_of_signal,1.0/rate)

plt.plot(t, data)
plt.xlabel("Time(in s)")
plt.ylabel("Amplitude")
plt.show()