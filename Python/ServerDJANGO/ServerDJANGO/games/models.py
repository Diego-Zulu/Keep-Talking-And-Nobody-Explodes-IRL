from django.db import models
from django.contrib.contenttypes import fields
from django.core import validators, exceptions

from basics.models import Lit, Color, SerialNumber
from modules.models import Wires, MorseCode, TheButton, SimonSays, Password

class Game(models.Model):
    name = models.CharField(max_length = 30)
    losses = models.IntegerField(default = 0, editable = False)
    won = models.IntegerField(default = 0, editable = False)
    difficulty = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

    time = models.IntegerField(validators = [validators.MinValueValidator(30), validators.MaxValueValidator(600)])
    strikes = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

    amount_batteries = models.IntegerField(default = 0)
    lit = models.ForeignKey(Lit, on_delete = models.CASCADE)
    serial_number = models.ForeignKey(SerialNumber, on_delete = models.CASCADE)

    wires_module = models.ForeignKey(Wires, blank = True, null = True, on_delete = models.CASCADE)
    morse_code_module = models.ForeignKey(MorseCode, blank = True, null = True, on_delete = models.CASCADE)
    password_module = models.ForeignKey(Password, blank = True, null = True, on_delete = models.CASCADE)
    the_button_module = models.ForeignKey(TheButton, blank = True, null = True, on_delete = models.CASCADE)
    simon_says_module = models.ForeignKey(SimonSays, blank = True, null = True, on_delete = models.CASCADE)

    def clean(self):
        i = 5
        if self.wires_module is None:
            i = i - 1
        if self.morse_code_module is None:
            i = i - 1
        if self.password_module is None:
            i = i - 1
        if self.the_button_module is None:
            i = i - 1
        if self.simon_says_module is None:
            i = i - 1
        if i != 5:
            raise exceptions.ValidationError('They have to be exactly 5 modules.')
        super(Game, self).clean()

    @property
    def str_difficulty(self):
        t = self.won + self.losses
        if t != 0:
            return '{0}%'.format(int((self.won*100)/t))
        return '{0}%'.format(int(100/self.difficulty))

    def __str__(self):
        return '{0} - {1}'.format(self.name, self.difficulty)