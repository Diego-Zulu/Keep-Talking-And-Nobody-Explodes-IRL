from django.db import models
from django.contrib.contenttypes import fields
from django.core import validators, exceptions
from modules.models import *

class Game(models.Model):
    name = models.CharField(max_length = 30)
    losses = models.IntegerField(default = 0, editable = False)
    won = models.IntegerField(default = 0, editable = False)
    difficulty = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

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
        if i != 3:
            raise exceptions.ValidationError('They have to be exactly 3 modules.')
        super(Game, self).clean()

    def __str__(self):
        return '{0} - {1}'.format(self.name, self.difficulty)