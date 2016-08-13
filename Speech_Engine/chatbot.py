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
    input_adapter="chatterbot.adapters.input.TerminalAdapter",
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
        # We pass None to this method because the parameter
        # is not used by the TerminalAdapter
        bot_input = bot.get_response(None)

    # Press ctrl-c or ctrl-d on the keyboard to exit
    except (KeyboardInterrupt, EOFError, SystemExit):
        break

