from django.db import models
from django.core import validators

class Color(models.Model):
    name = models.CharField(max_length = 30)
    tipe = models.CharField(max_length = 3)

    def __str__(self):
        return '{0} - {1}'.format(self.name, self.tipe)

class SerialNumber(models.Model):
    name = models.CharField(max_length = 6)

    def __str__(self):
        return self.name

class Lit(models.Model):
    name = models.CharField(max_length = 3, validators = [validators.MinLengthValidator(3)])

    def __str__(self):
        return self.name