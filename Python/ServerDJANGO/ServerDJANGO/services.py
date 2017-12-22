import random, sys
sys.path.append("../../ServerMQTT/")
from games.models import Game as GameModel
from ServerMQTT.ServerMQTT import GameFactory, GameBuilder

class GameService(object):
    _instance = None

    def __init__(self):
        self.game_model = None
        self.game = None

    @classmethod
    def get_instance(cls):
        if cls._instance is None:
            cls._instance = GameService()
        return cls._instance

    #################################################

    def is_running(self):
        return self.game_model != None and self.game.is_running()

    def start(self, game):
        self.game_model = game
        self.synchronize()
        self.game.connect("127.0.0.1")
        self.game.start()
        return self.is_running()

    def end(self):
        self.game_model = None
        self.game.end()
        self.game = None
        return self.is_running()

    def restart(self):
        if self.is_running():
            self.game.restart()
        return self.is_running()
    
    def get_time(self):
        if self.is_running():
            return self.game.time_left()
        return 0

    def get_game(self):
        return self.game_model

    def game_lost(self):
        self.game_model.losses += 1
        self.game_model.save()

    def game_won(self):
        self.game_model.won += 1
        self.game_model.save()

    def synchronize(self):
        builder = GameFactory.builder()
        builder.set_strikes(self.game_model.strikes)\
            .set_time(self.game_model.time)\
            .set_amount_batteries(self.game_model.amount_batteries)\
            .set_lit(self.game_model.lit.name)\
            .set_serial_number(self.game_model.serial_number.name)\
            .set_lose_function(self.game_lost)\
            .set_win_function(self.game_won)

        if self.game_model.wires_module != None:
            c = []
            for wire in self.game_model.wires_module.wires.all():
                c.append(wire.type)
            builder.add_wires(c)

        if self.game_model.morse_code_module != None:
            cw = self.game_model.morse_code_module.correct_word
            cf = self.game_model.morse_code_module.correct_frequency
            builder.add_morse_code(cf, cw)

        if self.game_model.password_module != None:
            builder.add_password(self.game_model.password_module.correct_password)

        if self.game_model.the_button_module != None:
            t = self.game_model.the_button_module.text
            sc = self.game_model.the_button_module.strip_color.type
            bc = self.game_model.the_button_module.button_color.type
            builder.add_the_button(bc, t, sc)

        if self.game_model.simon_says_module != None:
            c = []
            for wire in self.game_model.simon_says_module.to_flash.all():
                c.append(wire.type)
            builder.add_simon_says(c)

        self.game = builder.build()
