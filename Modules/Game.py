from Modules import MorseCode, Wires, TheButton, SimonSays, Passwords, CountdownTimer, Configuration


class Game:
    def __init__(self):
        self.mc = MorseCode(Configuration.morse_word)
        self.w = Wires(Configuration.wire_config, Configuration.serial_number)
        self.p = Passwords(Configuration.password)
        self.b = TheButton(Configuration.button_color, Configuration.button_text,Configuration.batteries_amount(),
                           Configuration.lit_indicators, Configuration.button_strip_color)
        self.c = CountdownTimer(Configuration.time)
        self.s = SimonSays(Configuration.serial_number, Configuration.simon_total_flashes)