

code_unreachable = 4
code_finished = 3
code_holding = 2
code_ignore = 1
code_fail = 0

blue = 'BLU'
red = 'R'
yellow = 'Y'
green = 'G'
black = 'BLK'
white = 'W'

posible_indicators = ['SND', 'CLR', 'CAR', 'IND', 'FRQ', 'SIG',
                      'NSA', 'MSA', 'TRN', 'BOB', 'FRK']

The_Button_texts = ['ABORT', 'DETONATE', 'HOLD']

password_words = ['about', 'after', 'again', 'below', 'could',
                  'every', 'first', 'found', 'great', 'house',
                  'large', 'learn', 'never', 'other', 'place',
                  'plant', 'point', 'right', 'small', 'sound',
                  'spell', 'still', 'study', 'their', 'there',
                  'these', 'thing', 'think', 'three', 'water',
                  'where', 'which', 'world', 'would', 'write', ]

word_to_code = {'shell': '3.505 MHz', 'halls': '3.515 MHz',
                'slick': '3.522 MHz', 'trick': '3.532 MHz',
                'boxes': '3.535 MHz', 'leaks': '3.542 MHz',
                'strobe': '3.545 MHz', 'bistro': '3.552 MHz',
                'flick': '3.555 MHz', 'bombs': '3.565 MHz',
                'break': '3.572 MHz', 'brick': '3.575 MHz',
                'steak': '3.582 MHz', 'sting': '3.592 MHz',
                'vector': '3.595 MHz', 'beats': '3.600 MHz'}

morse = {'a': '.-', 'b': '-...', 'c': '-.-.', 'd': '-..', 'e': '.',
         'f': '..-.', 'g': '--.', 'h': '....', 'i': '..', 'j': '.---',
         'k': '-.-', 'l': '.-..', 'm': '--', 'n': '-.', 'o': '---',
         'p': '.--.', 'q': '--.-', 'r': '.-.', 's': '...', 't': '-',
         'u': '..-', 'v': '...-', 'w': '.--', 'x': '-..-', 'y': '-.--',
         'z': '--..', '1': '.----', '2': '..---', '3': '...--', '4': '....-',
         '5': '.....', '6': '-....', '7': '--...', '8': '---..', '9': '----.',
         '0': '-----'}


def word_to_morse(word):
    translated = []
    for c in word:
        translated.append(morse[c])

    return translated



