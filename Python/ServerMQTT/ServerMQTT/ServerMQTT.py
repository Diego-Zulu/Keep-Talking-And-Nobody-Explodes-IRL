from .game.game import Game
from .ktane.modules import Wires, TheButton, SimonSays, Password, MorseCode

class GameFactory(object):

    @classmethod
    def builder(cls):
        return GameBuilder()

    @classmethod
    def create(cls):
        return Game()

class GameBuilder(object):

    def __init__(self):
        self.game_building = GameFactory.create()

    def set_time(self, time):
        self.game_building.time = time
        return self

    def set_strikes(self, strikes):
        self.game_building.strikes = strikes
        return self

    def set_amount_batteries(self, amount_batteries):
        self.game_building.amount_batteries = amount_batteries
        return self

    def set_lit(self, lit):
        self.game_building.lit = lit
        return self

    def set_serial_number(self, serial_number):
        self.game_building.serial_number = serial_number
        return self

    def set_win_function(self, f_win):
        self.game_building.f_win = f_win
        return self

    def set_lose_function(self, f_lose):
        self.game_building.f_lose = f_lose
        return self

    def add_wires(self, wires):
        self.game_building.modules.append(Wires(wires, self.game_building))
        return self

    def add_morse_code(self, correct_frequency, correct_word):
        self.game_building.modules.append(MorseCode(correct_frequency, correct_word, self.game_building))
        return self

    def add_simon_says(self, to_flash):
        self.game_building.modules.append(SimonSays(to_flash, self.game_building))
        return self

    def add_password(self, correct_password):
        self.game_building.modules.append(Password(correct_password, self.game_building))
        return self

    def add_the_button(self, color, text, strip):
        self.game_building.modules.append(TheButton(color, text, strip, self.game_building))
        return self

    def build(self):
        game = self.game_building
        self.game_building = GameFactory.create()
        return game