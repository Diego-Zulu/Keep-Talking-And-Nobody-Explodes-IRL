class Wires:

    def __init__(self, wires, serNumb):
        # self.configured_wires = wires
        # self.amount_wires = len(wires)
        # self.serial_number = serNumb
        self.configured_wires = ['R', 'W', 'BLK', 'BLU']
        self.amount_wires = len(self.configured_wires)
        self.serial_number = 123456

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

        if self.no_wires('R'):
            return cut_wire == 1
        elif self.last_wire('W'):
            return cut_wire == self.amount_wires - 1
        elif self.more_than_one_wire('BLU'):
            return cut_wire == self.position_of_last_wire('BLU')
        else:
            return cut_wire == self.amount_wires - 1

    def wires_module_four_cables(self, cut_wire):

        if self.more_than_one_wire('R') and self.last_digit_serial_number_is_odd():
            return cut_wire == self.position_of_last_wire('R')
        elif self.last_wire('Y') and self.no_wires('R'):
            return cut_wire == 0
        elif self.exactly_one_wire('BLU'):
            return cut_wire == 0
        elif self.more_than_one_wire('Y'):
            return cut_wire == self.amount_wires - 1
        else:
            return cut_wire == 1

    def wires_module_five_cables(self, cut_wire):

        if self.last_wire('BLK') and self.last_digit_serial_number_is_odd():
            return cut_wire == 3
        elif self.exactly_one_wire('R') and self.more_than_one_wire('Y'):
            return cut_wire == 0
        elif self.no_wires('BLK'):
            return cut_wire == 1
        else:
            return cut_wire == 0

    def wires_module_six_cables(self, cut_wire):

        if self.no_wires('Y') and self.last_digit_serial_number_is_odd():
            return cut_wire == 2
        elif self.exactly_one_wire('Y') and self.more_than_one_wire('W'):
            return cut_wire == 3
        elif self.no_wires('R'):
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

        return self.serial_number % 2 != 0

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
