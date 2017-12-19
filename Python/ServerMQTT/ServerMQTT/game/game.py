from ..ktane.modules import *
from ..songs import SoundEffectPlayer
from .timer import *

class Game(object):

    def __init__(self):
        self.strikes = 0
        self.time = 300
        self.lit = ''
        self.serial_number = ''
        self.amount_batteries = 0
        self.modules = []
        self.active = False
        self.f_lose = None
        self.f_win = None
    
    def connect(self, ip):
        print("----CONNTECTING----")
        print("-TO-IP: " + ip)
        for module in self.modules:
            module.connect(ip)
            print(module.topic_to_post)
            print(module.topic_to_read)
        print("-------------------")

    def is_running(self):
        return self.active

    def start(self):
        print("-------START-------")
        for module in self.modules:
            module.on_start()
        print("-------------------")
        self.timer = CountdownTimer(self.time, self.lose, self.to_lcd)
        self.timer.start()
        self.active = True

    def end(self):
        print("--------END--------")
        self.timer.stop = True
        for module in self.modules:
            module.on_end()
        print("-------------------")
        self.active = False

    def restart(self):
        print("--------RESTART--------")
        if self.is_running():
            self.end()
        self.start()
        print("-----------------------")

    def time_left(self):
        if self.is_running():
            return self.timer.current_countdown()
        return 0

    def lose_life(self):
        self.strikes -= 1
        if self.strikes < 1:
            self.lose()

    def winning(self):
        w = True
        for module in self.modules:
            w = w and module.won
        if w:
            self.win()

    def lose(self):
        print("-- GAME LOST --")
        if self.f_lose != None:
            self.f_lose()
            SoundEffectPlayer.play_lose()
        self.end()

    def win(self):
        print("-- GAME WON --")
        if self.f_win() != None:
            self.f_win()
            SoundEffectPlayer.play_win()
        self.end()
        
    def to_lcd(self):
        x = []
        st = self.strikes
        l1 = "V:{0:02d} B:{1}".format(st, self.amount_batteries)
        x.append(l1)
        l2 = " S:{0} P:{1}".format(self.serial_number, self.lit)
        x.append(l2)
        return x