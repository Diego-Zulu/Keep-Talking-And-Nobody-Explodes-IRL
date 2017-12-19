CODE_UNREACHABLE = 4
CODE_FINISHED = 3
CODE_HOLDING = 2
CODE_IGNORE = 1
CODE_FAIL = 0

POSIBLE_INDICATORS = ['SND', 'CLR', 'CAR', 'IND', 'FRQ', 'SIG',
                      'NSA', 'MSA', 'TRN', 'BOB', 'FRK']

THE_BUTTON_TEXTS = ['ABORT', 'DETONATE', 'HOLD']

BLUE = 'B'
RED = 'R'
YELLOW = 'Y'
GREEN = 'G'
BLACK = 'BLK'
WHITE = 'W'

PASSWORD_WORDS = ['about', 'after', 'again', 'below', 'could',
                  'every', 'first', 'found', 'great', 'house',
                  'large', 'learn', 'never', 'other', 'place',
                  'plant', 'point', 'right', 'small', 'sound',
                  'spell', 'still', 'study', 'their', 'there',
                  'these', 'thing', 'think', 'three', 'water',
                  'where', 'which', 'world', 'would', 'write', ]

WORD_TO_CODE = {'shell': '3.505 MHz', 'halls': '3.515 MHz',
                'slick': '3.522 MHz', 'trick': '3.532 MHz',
                'boxes': '3.535 MHz', 'leaks': '3.542 MHz',
                'strobe': '3.545 MHz', 'bistro': '3.552 MHz',
                'flick': '3.555 MHz', 'bombs': '3.565 MHz',
                'break': '3.572 MHz', 'brick': '3.575 MHz',
                'steak': '3.582 MHz', 'sting': '3.592 MHz',
                'vector': '3.595 MHz', 'beats': '3.600 MHz'}

MORESE = {'a': '.-', 'b': '-...', 'c': '-.-.', 'd': '-..', 'e': '.',
         'f': '..-.', 'g': '--.', 'h': '....', 'i': '..', 'j': '.---',
         'k': '-.-', 'l': '.-..', 'm': '--', 'n': '-.', 'o': '---',
         'p': '.--.', 'q': '--.-', 'r': '.-.', 's': '...', 't': '-',
         'u': '..-', 'v': '...-', 'w': '.--', 'x': '-..-', 'y': '-.--',
         'z': '--..', '1': '.----', '2': '..---', '3': '...--', '4': '....-',
         '5': '.....', '6': '-....', '7': '--...', '8': '---..', '9': '----.',
         '0': '-----'}