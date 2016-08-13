import speech_recognition as sr

m = None
for microphone_name in sr.Microphone.list_microphone_names():
    # if microphone_name == "HDA Intel HDMI: 0 (hw:0,3)":
    #     m = sr.Microphone()
    print(microphone_name)