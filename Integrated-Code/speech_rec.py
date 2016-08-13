import speech_recognition as sr
import os
import beeping
import music
# obtain audio from the microphone
def talk():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Say something!")
        beeping.beep()
        r.dynamic_energy_threshold=True
        #r.energy_threshold = 200
        r.adjust_for_ambient_noise(source, 0.5)     # listen for 0.5 second to calibrate the energy threshold for ambient noise levels
        r.pause_threshold = 0.5
        r.dynamic_energy_adjustment_ratio = 2.5     # speech is louder than ambient noise by a factor of 2.5
        audio = r.listen(source)
        beeping.beep()
           
    # write audio to a WAV file
    #with open("microphone-results.wav", "wb") as f:
    #    f.write(audio.get_wav_data())
    ans = r.recognize_sphinx(audio)
    # recognize speech using Sphinx

    try:
        print("Sphinx thinks you said = " + ans)
    except sr.UnknownValueError:
        print("Sphinx could not understand audio")
    except sr.RequestError as e:
        print("Sphinx error; {0}".format(e))

    command = '0'

    if ans=='' :
    	return command

    if 'START' in ans:
        if 'FORWARD' in ans:
            print("Moving Forward")
            command='f'
        elif 'LEFT' in ans:
            print("Moving left" )
            command='m'
        elif 'NAME' in ans:
            print("Moving left" )
            command='m'
        elif 'GOOD' in ans:
            print("Moving left" )
            command='m'
        elif 'QUIT' in ans:
            print("Moving left" )
            command='m'
        elif 'RIGHT' in ans:
            print("Moving right")
            command='n'
        elif 'HEIGHT' in ans:
            print("Moving right")
            command='n'
        elif 'BYE' in ans:
            print("Moving right")
            command='n'
        elif 'BACK' in ans:
            print("Moving back")
            command='b'
        elif 'LINE' in ans and 'FOLLOW' in ans:
            print("Going to follow a line")
            command='t'
        elif 'OBJECT' in ans and 'TRACK' in ans:
            print("Going for object tracking")
            command='o'
        elif 'FOLLOW' in ans:
            print("Moving Forward")
            command='f'
        return command           
    # elif 'TURN' in ans:
    #     if 'LEFT' in ans:
    #         print("Moving left" )
    #         command='m'
    #     elif 'NAME' in ans:
    #         print("Moving left" )
    #         command='m'
    #     elif 'GOOD' in ans:
    #         print("Moving left" )
    #         command='m'
    #     elif 'RIGHT' in ans:
    #         print("Moving right")
    #         command='n'
    #     elif 'HEIGHT' in ans:
    #         print("Moving right")
    #         command='n'
    #     elif 'BYE' in ans:
    #         print("Moving right")
    #         command='n'
    #     return command
    elif 'STRAIGHT' in ans:
        print("Moving Forward")
        command='f'
        return command
    elif 'COME BACK' in ans:
        print("Moving back")
        command='b'
        return command
    elif 'STOP' in ans:
        command='s'
        return command
    elif 'HELLO' in ans or 'GOOD' in ans :
        command = 'g'
    elif 'MUSIC' in ans:
        music.music()
        return command
    elif 'PLAY' in ans:
        music.music()
        return command
    # elif 'EXIT' in ans:
    #     command='q'
    #     return command
    elif 'FINISH' in ans:
        command='q'
        return command
    elif 'QUIT' in ans:
        command='q'
        return command
    # elif 'HELLO' in ans or 'GOOD' in ans :
    # 	command = 'g'
    	# return command
        
    from chatterbot import ChatBot
    from chatterbot.training.trainers import ListTrainer

    if os.path.isfile("./database.db"):
    	os.remove("./database.db")
    # Create a new instance of a ChatBot
    bot = ChatBot("Terminal",
        storage_adapter="chatterbot.adapters.storage.JsonDatabaseAdapter",
        logic_adapters=[
            "chatterbot.adapters.logic.MathematicalEvaluation",
            "chatterbot.adapters.logic.TimeLogicAdapter",
            "chatterbot.adapters.logic.ClosestMatchAdapter"
        ],
        input_adapter="chatterbot.adapters.input.VariableInputTypeAdapter",
        #output_adapter="chatterbot.adapters.output.OutputFormatAdapter",
        format='text',
        database="./database.db"
    )

    bot.set_trainer(ListTrainer)
    bot.train([
        "hello",
        "hi",
        "how are you",
        "i am fine",
        "that is good to hear",
        # "thank you",
        # "you are welcome",
        # "sorry",
        # "its okay", 
        "what is your name",
        "my name is HURO",
        "bye",
        "bye bye",
        "see you later",
        "take care",
        "you too",
        "good morning",
        "very good morning",
        "good afternoon",
        "very good afternoon",
        "good evening",
        "charming evening",
        "good night",
        "sweet dreams",
        "same to you",
        # "tell me about yourself",
        # "I am HURO. I am 60 centimeters tall and weighs 3 kilograms. I have 8 degrees of freedom, 2 in head and 3 in each arm. I am using ODroid and Arduino UNO.",
        # "what is your height",
        # "I am 60 centimeters tall",
        # "what is your weight",
        # "3 kilograms",
        "team members",
        "Ujjwal, Rijak, Abhishek, Saurabh, Mrinaal, Bhuvi, Shruti, Nitish",
        "what is ",
        "Ask a better question"

    ])

    response = bot.get_response(ans)
    print(response)

    import pyttsx

    engine = pyttsx.init()
    engine.setProperty('rate', 135)
    engine.say(response)
    engine.runAndWait()
    return command


while 1:
	cmnd = talk()
	with open("./s_rec.txt", "w") as f:
		f.write(cmnd)
		f.close()
	if cmnd == 'q' :
		break