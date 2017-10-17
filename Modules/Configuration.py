from Modules import Constants

# Delete after Django integration, or keep for debugging

serial_number_last_digit_odd = 'MC3455'
serial_number_last_digit_even = 'CH3456'
serial_number_vowel = 'CA3456'
serial_number_no_vowel = 'CA3456'

posible_batteries_format = {'AA':1, 'D':2}


# keep a reference kinda like this coming from Django

lit_indicators = [Constants.posible_indicators[0]]

unlit_indicators = [Constants.posible_indicators[1]]

serial_number = serial_number_last_digit_odd

batteries = posible_batteries_format

def batteries_amount():
    amount = 0
    for key, value in batteries:
        amount = amount + value

    return amount

password = Constants.password_words[0]

morse_word = Constants.word_to_code['shell']

wire_config = [Constants.red, Constants.white, Constants.black, Constants.blue]

button_color = Constants.blue

button_text = Constants.The_Button_texts[0]

button_strip_color = Constants.blue

time = 120

simon_total_flashes = [Constants.blue, Constants.red, Constants.green, Constants.yellow, Constants.green, Constants.red]