
import Modules.Constants as consts

class SimonSays:

    def __init__(self, numb):
        self.ser_numb = numb

    def check_correct_press(self, pressed, flashed, strikes):

        if self.contains_vowel(self.ser_numb):
            self.check_vowel_presses(pressed, flashed, strikes)
        else:
            self.check_no_vowel_presses(pressed, flashed, strikes)

    def contains_vowel(self, numb):

        vowels = ['a', 'i', 'u', 'e', 'o']
        return any((True for x in numb if x in vowels))

    def check_vowel_presses(self, pressed, flashed, strikes):

        if strikes == 0:
            self.vowel_zero_strikes(pressed, flashed)
        elif strikes == 1:
            self.vowel_one_strike(pressed, flashed)
        else:
            self.vowel_two_strikes(pressed, flashed)

    def vowel_zero_strikes(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.blue:
                return False
            if x == consts.blue and y != consts.red:
                return False
            if x == consts.green and y != consts.yellow:
                return False
            if x == consts.yellow and y != consts.green:
                return False

        return True


    def vowel_one_strike(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.yellow:
                return False
            if x == consts.blue and y != consts.green:
                return False
            if x == consts.green and y != consts.blue:
                return False
            if x == consts.yellow and y != consts.red:
                return False

        return True

    def vowel_two_strikes(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.green:
                return False
            if x == consts.blue and y != consts.red:
                return False
            if x == consts.green and y != consts.yellow:
                return False
            if x == consts.yellow and y != consts.blue:
                return False

        return True

    def check_no_vowel_presses(self, pressed, flashed, strikes):

        if strikes == 0:
            self.no_v_zero_strikes(pressed, flashed)
        elif strikes == 1:
            self.no_v_one_strike(pressed, flashed)
        else:
            self.no_v_two_strikes(pressed, flashed)

    def no_v_zero_strikes(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.blue:
                return False
            if x == consts.blue and y != consts.yellow:
                return False
            if x == consts.green and y != consts.green:
                return False
            if x == consts.yellow and y != consts.red:
                return False

        return True

    def no_v_one_strike(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.red:
                return False
            if x == consts.blue and y != consts.blue:
                return False
            if x == consts.green and y != consts.yellow:
                return False
            if x == consts.yellow and y != consts.green:
                return False

        return True

    def no_v_two_strikes(self, pressed, flashed):

        for x, y in zip(flashed, pressed):
            if x == consts.red and y != consts.yellow:
                return False
            if x == consts.blue and y != consts.green:
                return False
            if x == consts.green and y != consts.blue:
                return False
            if x == consts.yellow and y != consts.red:
                return False

        return True