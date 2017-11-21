from django.db import models
from django.core import validators
from basics.models import Color, Lit, SerialNumber

class Module(models.Model):
    name = models.CharField(max_length = 30)
    losses = models.IntegerField(default = 0, editable = False)
    won = models.IntegerField(default = 0, editable = False)
    difficulty = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

    @property
    def str_difficulty(self):
        t = self.won + self.losses
        if t != 0:
            return '{0}%'.format(int((self.won*100)/t))
        return '{0}%'.format(int(100/self.difficulty))

    def __str__(self):
        return 'N:{0}  D:{1}'.format(self.name, self.str_difficulty)

    class Meta:
        abstract = True

class MorseCode(Module):
    correct_word = models.CharField(max_length = 10)
    correct_frequency = models.CharField(max_length = 255)

class Wires(Module):
    wires = models.ManyToManyField(Color)

class SimonSays(Module):
    #serial_number = models.ForeignKey(SerialNumber, on_delete = models.CASCADE)
    to_flash = models.ManyToManyField(Color)

class Password(Module):
    correct_password = models.CharField(max_length = 10)

class TheButton(Module):
    #lit
    #batteries
    strip_color = models.ForeignKey(Color, on_delete = models.CASCADE, related_name='%(class)s_strip_color')
    button_color = models.ForeignKey(Color, on_delete = models.CASCADE, related_name='%(class)s_button_color')
    text = models.CharField(max_length = 10)
    #amount_batteries = models.IntegerField(default = 0)
