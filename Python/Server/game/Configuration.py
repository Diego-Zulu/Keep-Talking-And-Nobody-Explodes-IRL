from ktane import constants as Constants

# Delete after Django integration, or keep for debugging

serial_number_last_digit_odd = 'MC3455'
serial_number_last_digit_even = 'CH3456'
serial_number_vowel = 'CA3456'
serial_number_no_vowel = 'CA3456'

posible_batteries_format = {'AA':1, 'D':2}

# keep a reference kinda like this coming from Django

lit_indicators = [Constants.POSIBLE_INDICATORS[0]]

unlit_indicators = [Constants.POSIBLE_INDICATORS[1]]

serial_number = serial_number_last_digit_odd

batteries = posible_batteries_format

def batteries_amount():
    amount = 0
    for key, value in batteries.items():
        amount = amount + value
    return amount

password = Constants.PASSWORD_WORDS[0]

correct_word = 'shell'
morse_word = Constants.WORD_TO_CODE[correct_word]

wire_config = [Constants.RED, Constants.WHITE, Constants.BLACK, Constants.BLUE]

button_color = Constants.BLUE

button_text = Constants.THE_BUTTON_TEXTS[0]

button_strip_color = Constants.BLUE

time = 120

simon_total_flashes = [Constants.BLUE, Constants.RED, Constants.GREEN, Constants.YELLOW, Constants.GREEN, Constants.RED]