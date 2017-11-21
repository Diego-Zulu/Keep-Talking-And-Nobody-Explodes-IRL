import random
from games.models import *

class ServerMQTT(object):
    _instance = None

    def __init__(self):
        self.game = None

    def is_running(self):
        return self.game != None

    def start(self, game):
        self.game = game
        return self.is_running()

    def end(self):
        self.game = None
        return self.is_running()

    def restart(self):
        if self.is_running():
            pass
        return self.is_running()

    def get_time(self):
        if self.is_running():
            return random.randint(1, 300)
        return 0

    def get_game(self):
        return self.game

    @classmethod
    def get_instance(cls):
        if cls._instance is None:
            cls._instance = PiServer()
        return cls._instance