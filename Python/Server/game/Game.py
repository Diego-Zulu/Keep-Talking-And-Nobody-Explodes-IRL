from ktane.modules import *
from game import Configuration
from game.timer import *

class Game(object):
    def __init__(self):
        self.strikes = 0
        self.mc = MorseCode(Configuration.morse_word,Configuration.correct_word)
        self.w = Wires(Configuration.wire_config, Configuration.serial_number)
        self.s = SimonSays(Configuration.serial_number, Configuration.simon_total_flashes, self)
        self.p = Passwords(Configuration.password)
        self.b = TheButton(Configuration.button_color, Configuration.button_text,Configuration.batteries_amount(),
                           Configuration.lit_indicators, Configuration.button_strip_color, self)
        self.timer = CountdownTimer(Configuration.time)
        self.modules = [self.mc, self.w, self.s, self.p, self.b]
    
    def connect(self, ip):
        for module in self.modules:
            module.connect(ip)
            print(module.topic_to_post)
            print(module.topic_to_read)

    def start(self):
        for module in self.modules:
            module.on_start()
        self.timer.start()

    def end(self):
        for module in self.modules:
            module.on_end()