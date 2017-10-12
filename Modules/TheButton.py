# I know code can be reduced and cleaned, but I prefer it this way because it respects the way the
# manual is written, so I think it should help us when debugging

code_unreachable = 4
code_finished = 3
code_holding = 2
code_ignore = 1
code_fail = 0


class TheButton:
    def __init__(self, color, text, batteries, indicators, strip):
        # self.button_color = color
        # self.button_says = text
        # self.amount_batteries = batteries
        # self.lit_indicators = indicators
        # self.strip_color = strip
        self.button_color = 'BLU'
        self.strip_color = 'BLU'
        self.button_says = 'ABORT'
        self.amount_batteries = 1
        self.lit_indicators = ['CAR']
        self.holding = False

    def the_button_was_handled(self, action, current_countdown):

        if self.holding and action == 'RELEASE':
            return self.holding_logic(current_countdown)
        elif not self.holding:
            result = self.first_press_logic(action)
            if result == code_holding:
                self.holding = True
            return result
        else:
            return code_unreachable

    def first_press_logic(self, action):

        if self.button_says == 'ABORT' and self.button_color == 'BLU' and action == 'HOLD':
            return code_holding
        elif self.amount_batteries > 1 and self.button_says == 'DETONATE' and action == 'PRESS':
            return code_finished
        elif self.button_color == 'W' and self.includes_indicator('CAR') and action == 'HOLD':
            return code_holding
        elif self.amount_batteries > 2 and self.includes_indicator('FRK') and action == 'PRESS':
            return code_finished
        elif self.button_color == 'Y' and action == 'HOLD':
            return code_holding
        elif self.button_color == 'R' and self.button_says == 'HOLD' and action == 'PRESS':
            return code_finished
        elif action == 'HOLD':
            return code_holding
        elif action == 'PRESS':
            return code_fail
        else:
            return code_ignore

    def holding_logic(self, current_countdown):

        if self.strip_color == 'BLU' and self.countdown_includes_number(current_countdown, '4'):
            return code_finished
        elif self.strip_color == 'W' and self.countdown_includes_number(current_countdown, '1'):
            return code_finished
        elif self.strip_color == 'Y' and self.countdown_includes_number(current_countdown, '5'):
            return code_finished
        elif (self.strip_color != 'BLU' and self.strip_color != 'W' and self.strip_color != 'Y'
              and self.countdown_includes_number(current_countdown, '1')):
            return code_finished
        else:
            self.holding = False
            return code_fail

    def includes_indicator(self, name):

        return name in self.lit_indicators

    def countdown_includes_number(self, current_countdown, number):

        return number in current_countdown
