
code_finished = 2
code_holding = 1
code_fail = 0

class TheButton:

    def __init__(self, color, text, batteries, indicators):
        # self.button_color = color
        # self.button_says = text
        # self.amount_batteries = batteries
        # self.lit_indicators = indicators
        self.button_color = 'BLU'
        self.button_says = 'ABORT'
        self.amount_batteries = 1
        self.lit_indicators = ['CAR']
        self.holding = False

    def the_button_was_handled(self, action, current_countdown):

        if self.holding and action == 'RELEASE':
            return self.holding_logic(action, current_countdown)
        elif not self.holding:
            return self.first_press_logic(action, current_countdown)
        else:
            return code_fail


    def first_press_logic(self, action, current_countdown):

        if self.button_says == 'ABORT' and self.button_color == 'BLU' and action == 'HOLD':
            return code_holding
        elif self.amount_batteries > 1 and self.button_says == 'DETONATE' and action == 'PRESS':
            return code_finished
        elif self.button_color == 'W' and self.includes_indicator('CAR') and action == 'HOLD':
            return code_holding
        elif self.amount_batteries > 2 and self.includes_indicator('CAR') and action == 'PRESS':


    def includes_indicator(self, name):

        if name in self.lit_indicators:
            return True
        else:
            return False
