from django.db import models
from django.db.models.signals import pre_save
from django.core import validators
from django.dispatch import receiver
from basics.models import Color, Lit, SerialNumber

class Module(models.Model):
    name = models.CharField(max_length = 30)
    difficulty = models.IntegerField(validators = [validators.MinValueValidator(1), validators.MaxValueValidator(5)])

    def __str__(self):
        return 'N:{0}  D:{1}'.format(self.name, self.difficulty)

    class Meta:
        abstract = True

class MorseCode(Module):
    correct_word = models.CharField(max_length = 10)
    correct_frequency = models.CharField(max_length = 255)
 
    def save(self, *args, **kwargs):
        self.correct_word = self.correct_word.lower()
        return super(MorseCode, self).save(*args, **kwargs)


class Wires(Module):
    wires = models.ManyToManyField(Color)


class SimonSays(Module):
    to_flash = models.ManyToManyField(Color)


class Password(Module):
    correct_password = models.CharField(max_length = 10)

    def save(self, *args, **kwargs):
        self.correct_password = self.correct_password.lower()
        return super(Password, self).save(*args, **kwargs)

class TheButton(Module):
    strip_color = models.ForeignKey(Color, on_delete = models.CASCADE, related_name='%(class)s_strip_color')
    button_color = models.ForeignKey(Color, on_delete = models.CASCADE, related_name='%(class)s_button_color')
    text = models.CharField(max_length = 10)

    def save(self, *args, **kwargs):
        self.text = self.text.upper()
        return super(TheButton, self).save(*args, **kwargs)