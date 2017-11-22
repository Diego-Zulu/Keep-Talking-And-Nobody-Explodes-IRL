from ktane.modules import *
from game import Configuration
from game.timer import *

class Game(object):
    def __init__(self):
        self.strikes = 0
        self.time = Configuration.time
        self.lit = ''
        self.serial_number = ''
        self.active = False
        self.amount_batteries = 0
        self.modules = []

        #self.mc = MorseCode(Configuration.morse_word, Configuration.correct_word, self)
        #self.w = Wires(Configuration.wire_config, self)
        #self.s = SimonSays(Configuration.simon_total_flashes, self)
        #self.p = Passwords(Configuration.password, self)
        #self.b = TheButton(Configuration.button_color, Configuration.button_text, Configuration.button_strip_color, self)
        #self.modules = [self.mc, self.w, self.s, self.p, self.b]
    
    def lose_life(self):
        self.strikes -= 1
        if self.strikes < 1:
            self.end()

    def connect(self, ip):
        for module in self.modules:
            module.connect(ip)
            #print(module.topic_to_post)
            #print(module.topic_to_read)

    def is_running(self):
        return self.active

    def start(self):
        for module in self.modules:
            module.on_start()
        self.timer = CountdownTimer(self.time)
        self.timer.start()
        self.active = True

    def end(self):
        for module in self.modules:
            module.on_end()
        self.active = False

    def restart(self):
        if self.is_running():
            self.end()
        self.start()