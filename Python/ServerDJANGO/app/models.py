"""
Definition of models.
"""

from django.db import models

class Persona(models.Model):
    name = models.TextField(max_length = 256)