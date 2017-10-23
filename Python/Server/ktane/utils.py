from . import constants as consts 

def string_to_array(msg, type):
    s = msg.split(",")
    return list(map(lambda x : type(x), s))

def array_to_string(arr):
    return ",".join(arr)

def word_to_morse(word):
    translated = []
    for c in word:
        translated.append(consts.MORESE[c])
    return translated