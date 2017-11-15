from django.db import models
from django.core import validators
from basics.models import Color, Lit, SerialNumber

class Module(models.Model):
    name = models.CharField(max_length = 30)
    losses = models.IntegerField(default = 0, editable = False)
    won = models.IntegerField(default = 0, editable = False)
    difficulty = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

    def __str__(self):
        return '{0} - {1}'.format(self.name, self.difficulty)

    class Meta:
        abstract = True

class MorseCode(Module):
    correct_word = models.CharField(max_length = 10)
    correct_frequency = models.CharField(max_length = 255)

    def __str__(self):
        '{0} (W: {1}, F: {2})'.format(super(MorseCode, self).__str__(), )
        return super(MorseCode, self).__str__() + ' (W: ' + str(self.correct_word) + ', F: ' + str(self.correct_frequency) + ')'

class Wires(Module):
    wires = models.ManyToManyField(Color)

class SimonSays(Module):
    serial_number = models.ForeignKey(SerialNumber)
    to_flash = models.ManyToManyField(Color)

class Passwords(Module):
    correct_password = models.CharField(max_length = 10)

class TheButton(Module):
    strip_color = models.ForeignKey(Color, related_name='%(class)s_strip_color')
    button_color = models.ForeignKey(Color, related_name='%(class)s_button_color')
    text = models.CharField(max_length = 10)
    amount_batteries = models.IntegerField(default = 0)
