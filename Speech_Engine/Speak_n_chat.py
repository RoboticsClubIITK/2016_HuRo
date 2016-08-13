import speech_recognition as sr
from chatterbot import ChatBot
from chatterbot.training.trainers import ListTrainer


# Create a new instance of a ChatBot
bot = ChatBot("Terminal",
    storage_adapter="chatterbot.adapters.storage.JsonDatabaseAdapter",
    logic_adapters=[
        "chatterbot.adapters.logic.MathematicalEvaluation",
        "chatterbot.adapters.logic.TimeLogicAdapter",
        "chatterbot.adapters.logic.ClosestMatchAdapter"
    ],
    input_adapter="chatterbot.adapters.input.VariableInputTypeAdapter",
    output_adapter="chatterbot.adapters.output.TerminalAdapter",
    database="../database.db"
)
bot.set_trainer(ListTrainer)
bot.train([
    "hello",
    "hi",
    "how are you",
    "i am fine",
    "that is good to hear",
    "thank you",
    "you are welcome",
    "sorry",
    "its okay", 
    "what is your name",
    "my name is HURO",
])
print("Type something to begin...")

# The following loop will execute each time the user enters input
while True:
    try:
        # obtain audio from the microphone
        r = sr.Recognizer()
        with sr.Microphone() as source:
        	print("\nSay something!")
        	r.dynamic_energy_threshold=True
        	#r.energy_threshold = 200
        	r.adjust_for_ambient_noise(source, 0.5)     # listen for 0.5 second to calibrate the energy threshold for ambient noise levels
        	r.pause_threshold = 0.5
        	r.dynamic_energy_adjustment_ratio = 2.5     # speech is louder than ambedient noise by a factor of 2.5
        	audio = r.listen(source)
        	print(r.recognize_sphinx(audio))
        # We pass None to this method because the parameter
        # is not used by the TerminalAdapter
        bot_input = bot.get_response(r.recognize_sphinx(audio))
        print("\n")
    
    # Press ctrl-c or ctrl-d on the keyboard to exit
    except (KeyboardInterrupt, EOFError, SystemExit):
        break

