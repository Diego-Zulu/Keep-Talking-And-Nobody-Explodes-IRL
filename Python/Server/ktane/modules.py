import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

class Module(object):

    def __init__(self):
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.topic_to_post = self.__class__.__name__+"_server"
        self.topic_to_read = self.__class__.__name__+"_client"
        self.host = ""
        self.port = 1883
        self.keepalive = 60
    
    def on_connect(self, client, userdata, flags, rc):
        self.client.subscribe(self.topic_to_read)

    def on_message(self, client, userdata, msg):
        self.message(msg.payload.decode("utf-8"))

    def on_start(self):
        self.client.loop_start()
        self.send_message("START");
        self.start()

    def on_end(self):
        self.end()
        self.send_message("END");
        self.client.loop_stop()

    def connect(self, host, port = 1883, keepalive = 60, bind_address = ""):
        self.host = host
        self.port = port
        self.keepalive = keepalive
        self.client.connect(host, port, keepalive, bind_address)

    def send_message(self, msg):
        print(msg)
        qos = 0
        retain = False
        client_id = ""
        publish.single(self.topic_to_post, msg, qos, retain, self.host, self.port, client_id, self.keepalive)

    def send_error(self):
        self.send_message("ERROR");

    def send_ok(self):
        self.send_message("OK");

    def send_start(self):
        self.send_message("START");

    def send_end(self):
        self.send_message("END");

    #USER TRIGGERS AN ACTION
    def message(self, msg):
        pass

    #THE GAME START
    #USE IT TO SEND INITIAL 
    #SETUP TO THE ARDUINO
    def start(self):
        pass

    #THE GAME END/RESET?
    def end(self):
        pass

from . import constants as consts 
from . import utils as utils

#########################################
class MorseCode(Module):

    def __init__(self, correct_frequency, correct_word):
        self.frequency = correct_frequency
        self.word = correct_word
        super(MorseCode, self).__init__()

    def message(self, msg):
        if self.verify_frequency(msg):
            self.send_ok()
        else:
            self.send_error()

    def start(self):
        code = utils.word_to_morse(self.word)
        self.send_message(utils.array_to_string(code))
        
    def verify_frequency(self, attempt):
        return attempt == self.frequency

#########################################
class Wires(Module):

    def __init__(self, wires, serNumb):
        self.configured_wires = wires
        self.amount_wires = len(wires)
        self.serial_number = serNumb
        super(Wires, self).__init__()

    def message(self, msg):
        i = int(msg)
        if self.wire_was_cut(i):
            self.send_ok()
        else:
            self.send_error()

    def start(self):
        self.send_message(str(self.amount_wires))

    def wire_was_cut(self, cut_wire):
        if self.amount_wires == 3:
            return self.wires_module_three_cables(cut_wire)
        elif self.amount_wires == 4:
            return self.wires_module_four_cables(cut_wire)
        elif self.amount_wires == 5:
            return self.wires_module_five_cables(cut_wire)
        else:
            return self.wires_module_six_cables(cut_wire)

    def wires_module_three_cables(self, cut_wire):
        if self.no_wires(consts.RED):
            return cut_wire == 1
        elif self.last_wire(consts.WHITE):
            return cut_wire == self.amount_wires - 1
        elif self.more_than_one_wire(consts.BLUE):
            return cut_wire == self.position_of_last_wire(consts.BLUE)
        else:
            return cut_wire == self.amount_wires - 1

    def wires_module_four_cables(self, cut_wire):
        if self.more_than_one_wire(consts.RED) and self.last_digit_serial_number_is_odd():
            return cut_wire == self.position_of_last_wire(consts.RED)
        elif self.last_wire(consts.YELLOW) and self.no_wires(consts.RED):
            return cut_wire == 0
        elif self.exactly_one_wire(consts.BLUE):
            return cut_wire == 0
        elif self.more_than_one_wire(consts.YELLOW):
            return cut_wire == self.amount_wires - 1
        else:
            return cut_wire == 1

    def wires_module_five_cables(self, cut_wire):
        if self.last_wire(consts.BLACK) and self.last_digit_serial_number_is_odd():
            return cut_wire == 3
        elif self.exactly_one_wire(consts.RED) and self.more_than_one_wire(consts.YELLOW):
            return cut_wire == 0
        elif self.no_wires(consts.BLACK):
            return cut_wire == 1
        else:
            return cut_wire == 0

    def wires_module_six_cables(self, cut_wire):
        if self.no_wires(consts.YELLOW) and self.last_digit_serial_number_is_odd():
            return cut_wire == 2
        elif self.exactly_one_wire(consts.YELLOW) and self.more_than_one_wire(consts.WHITE):
            return cut_wire == 3
        elif self.no_wires(consts.RED):
            return cut_wire == self.amount_wires - 1
        else:
            return cut_wire == 3

    def no_wires(self, letter):
        no_red = True
        for wire in self.configured_wires:
            if wire == letter:
                no_red = False
        return no_red

    def last_wire(self, letter):
        return self.configured_wires[self.amount_wires - 1] == letter

    def more_than_one_wire(self, letter):
        count = 0
        for wire in self.configured_wires:
            if wire == letter:
                count = count + 1
        return count > 1

    def last_digit_serial_number_is_odd(self):
        return (ord(self.serial_number[-1]) - ord('0')) % 2 != 0

    def exactly_one_wire(self, letter):
        count = 0
        for wire in self.configured_wires:
            if wire == letter:
                count = count + 1
        return count == 1

    def position_of_last_wire(self, letter):
        count = self.amount_wires - 1
        for x in reversed(self.configured_wires):
            if x == letter:
                return count
            else:
                count = count - 1

#########################################
class SimonSays(Module):

    def __init__(self, numb, to_flash, game):
        self.ser_numb = numb
        self.flashed = to_flash
        self.game = game
        super(SimonSays, self).__init__()

    def message(self, msg):
        pressed = utils.string_to_array(msg, str)
        if self.check_correct_press(pressed, self.game.strikes):
            self.send_ok()
        else:
            self.send_error()

    def start(self):
        self.send_message(utils.array_to_string(self.flashed))

    def check_correct_press(self, pressed, strikes):
        if self.contains_vowel(self.ser_numb):
            return self.check_vowel_presses(pressed, self.flashed, strikes)
        else:
            return self.check_no_vowel_presses(pressed, self.flashed, strikes)

    def contains_vowel(self, numb):
        vowels = ['a', 'i', 'u', 'e', 'o']
        return any((True for x in numb if x in vowels))

    def check_vowel_presses(self, pressed, flashed, strikes):
        if strikes == 0:
            return self.vowel_zero_strikes(pressed, flashed)
        elif strikes == 1:
            return self.vowel_one_strike(pressed, flashed)
        else:
            return self.vowel_two_strikes(pressed, flashed)

    def vowel_zero_strikes(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.BLUE and y != consts.BLUE:
                return False
            if x == consts.BLUE and y != consts.RED:
                return False
            if x == consts.GREEN and y != consts.YELLOW:
                return False
            if x == consts.YELLOW and y != consts.YELLOW:
                return False
        return True

    def vowel_one_strike(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.RED and y != consts.YELLOW:
                return False
            if x == consts.BLUE and y != consts.GREEN:
                return False
            if x == consts.GREEN and y != consts.BLUE:
                return False
            if x == consts.YELLOW and y != consts.RED:
                return False
        return True

    def vowel_two_strikes(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.RED and y != consts.GREEN:
                return False
            if x == consts.BLUE and y != consts.RED:
                return False
            if x == consts.GREEN and y != consts.YELLOW:
                return False
            if x == consts.YELLOW and y != consts.BLUE:
                return False
        return True

    def check_no_vowel_presses(self, pressed, flashed, strikes):
        if strikes == 0:
            return self.no_v_zero_strikes(pressed, flashed)
        elif strikes == 1:
            return self.no_v_one_strike(pressed, flashed)
        else:
            return self.no_v_two_strikes(pressed, flashed)

    def no_v_zero_strikes(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.RED and y != consts.BLUE:
                return False
            if x == consts.BLUE and y != consts.YELLOW:
                return False
            if x == consts.GREEN and y != consts.GREEN:
                return False
            if x == consts.YELLOW and y != consts.RED:
                return False
        return True

    def no_v_one_strike(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.RED and y != consts.RED:
                return False
            if x == consts.BLUE and y != consts.BLUE:
                return False
            if x == consts.GREEN and y != consts.YELLOW:
                return False
            if x == consts.YELLOW and y != consts.GREEN:
                return False
        return True

    def no_v_two_strikes(self, pressed, flashed):
        for x, y in zip(flashed, pressed):
            if x == consts.RED and y != consts.YELLOW:
                return False
            if x == consts.BLUE and y != consts.GREEN:
                return False
            if x == consts.GREEN and y != consts.BLUE:
                return False
            if x == consts.YELLOW and y != consts.RED:
                return False
        return True

#########################################
class Passwords(Module):

    def __init__(self, correct_password):
        self.password = correct_password
        super(Passwords, self).__init__()

    def message(self, msg):
        if self.verify_password(msg):
            self.send_ok()
        else:
            self.send_error()

    def start(self):
        self.send_message(self.password)

    def verify_password(self, attempt):
        return attempt == self.password

#########################################
class TheButton(Module):

    def __init__(self, color, text, batteries, indicators, strip, game):
        self.button_color = color
        self.button_says = text
        self.amount_batteries = batteries
        self.lit_indicators = indicators
        self.strip_color = strip
        self.holding = False
        self.game = game
        super(TheButton, self).__init__()

    def message(self, msg):
        result = self.the_button_was_handled(msg, self.game.timer.current_countdown_formater())
        if result == 0:
            self.send_error()
        elif result == 1: 
            self.send_message('IGNORE')
        elif result == 2:
            self.send_message('HOLDING')
        elif result == 3:
            self.send_ok()
        else:
            self.send_message('UNREACHABLE')

    def start(self):
        x = [self.button_color, self.button_says, self.strip_color]
        self.send_message(utils.array_to_string(x))

    def the_button_was_handled(self, action, current_countdown):
        if self.holding and action == 'RELEASE':
            return self.holding_logic(current_countdown)
        elif not self.holding:
            result = self.first_press_logic(action)
            if result == consts.CODE_HOLDING:
                self.holding = True
            return result
        else:
            return consts.CODE_UNREACHABLE

    def first_press_logic(self, action):
        if self.button_says == 'ABORT' and self.button_color == 'BLU' and action == 'HOLD':
            return consts.CODE_HOLDING
        elif self.amount_batteries > 1 and self.button_says == 'DETONATE' and action == 'PRESS':
            return consts.CODE_FINISHED
        elif self.button_color == 'W' and self.includes_indicator('CAR') and action == 'HOLD':
            return consts.CODE_HOLDING
        elif self.amount_batteries > 2 and self.includes_indicator('FRK') and action == 'PRESS':
            return consts.CODE_FINISHED
        elif self.button_color == 'Y' and action == 'HOLD':
            return consts.CODE_HOLDING
        elif self.button_color == 'R' and self.button_says == 'HOLD' and action == 'PRESS':
            return consts.CODE_FINISHED
        elif action == 'HOLD':
            return consts.CODE_HOLDING
        elif action == 'PRESS':
            return consts.CODE_FAIL
        else:
            return consts.CODE_IGNORE

    def holding_logic(self, current_countdown):
        if self.strip_color == 'BLU' and self.countdown_includes_number(current_countdown, '4'):
            return consts.CODE_FINISHED
        elif self.strip_color == 'W' and self.countdown_includes_number(current_countdown, '1'):
            return consts.CODE_FINISHED
        elif self.strip_color == 'Y' and self.countdown_includes_number(current_countdown, '5'):
            return consts.CODE_FINISHED
        elif (self.strip_color != 'BLU' and self.strip_color != 'W' and self.strip_color != 'Y'
              and self.countdown_includes_number(current_countdown, '1')):
            return consts.CODE_FINISHED
        else:
            self.holding = False
            return consts.CODE_FAIL

    def includes_indicator(self, name):
        return name in self.lit_indicators

    def countdown_includes_number(self, current_countdown, number):
        return number in current_countdown