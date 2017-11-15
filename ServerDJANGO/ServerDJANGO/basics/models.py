from django.db import models
from django.core import validators

class Color(models.Model):
    name = models.CharField(max_length = 30)
    tipe = models.CharField(max_length = 3)

class SerialNumber(models.Model):
    name = models.CharField(max_length = 6)

class Lit(models.Model):
    name = models.CharField(max_length = 3, validators = [validators.MinLengthValidator(3)])