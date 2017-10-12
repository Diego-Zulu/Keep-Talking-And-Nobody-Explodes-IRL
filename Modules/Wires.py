configured_wires = ['R', 'W', 'BLK', 'BLU']
amount_wires = len(configured_wires)
serial_number = 123456


def wires_module(wires, serNumb):

    configured_wires = wires
    amount_wires = len(configured_wires)
    serial_number = serNumb


def wire_was_cut(cut_wire):

    if amount_wires == 3:
        return wires_module_three_cables(cut_wire)
    elif amount_wires == 4:
        return wires_module_four_cables(cut_wire)
    elif amount_wires == 5:
        return wires_module_five_cables(cut_wire)
    else:
        return wires_module_six_cables(cut_wire)


def wires_module_three_cables(cut_wire):

    if no_wires('R'):
        return cut_wire == 1
    elif last_wire('W'):
        return cut_wire == amount_wires - 1
    elif more_than_one_wire('BLU'):
        return cut_wire == position_of_last_wire('BLU')
    else:
        return cut_wire == amount_wires - 1


def wires_module_four_cables(cut_wire):

    if more_than_one_wire('R') and last_digit_serial_number_is_odd():
        return cut_wire == position_of_last_wire('R')
    elif last_wire('Y') and no_wires('R'):
        return cut_wire == 0
    elif exactly_one_wire('BLU'):
        return cut_wire == 0
    elif more_than_one_wire('Y'):
        return cut_wire == amount_wires - 1
    else:
        return cut_wire == 1


def wires_module_five_cables(cut_wire):

    if last_wire('BLK') and last_digit_serial_number_is_odd():
        return cut_wire == 3
    elif exactly_one_wire('R') and more_than_one_wire('Y'):
        return cut_wire == 0
    elif no_wires('BLK'):
        return cut_wire == 1
    else:
        return cut_wire == 0


def wires_module_six_cables(cut_wire):

    if no_wires('Y') and last_digit_serial_number_is_odd():
        return cut_wire == 2
    elif exactly_one_wire('Y') and more_than_one_wire('W'):
        return cut_wire == 3
    elif no_wires('R'):
        return cut_wire == amount_wires - 1
    else:
        return cut_wire == 3


def no_wires(letter):

    no_red = True
    for wire in configured_wires:
        if wire == letter:
            no_red = False
    return no_red


def last_wire(letter):

    return configured_wires[amount_wires - 1] == letter


def more_than_one_wire(letter):

    count = 0
    for wire in configured_wires:
        if wire == letter:
            count = count + 1
    return count > 1


def last_digit_serial_number_is_odd():

    return serial_number % 2 != 0


def exactly_one_wire(letter):

    count = 0
    for wire in configured_wires:
        if wire == letter:
            count = count + 1
    return count == 1


def position_of_last_wire(letter):

    count = amount_wires - 1
    for x in reversed(configured_wires):
        if x == letter:
            return count
        else:
            count = count - 1



