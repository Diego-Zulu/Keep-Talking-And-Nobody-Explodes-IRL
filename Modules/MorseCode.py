class MorseCode:

    def __init__(self, correct_frequency):
        self.frequency = correct_frequency

    def verify_frequency(self, attempt):
        return attempt == self.frequency
