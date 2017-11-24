# -*- coding: utf-8 -*-
# Generated by Django 1.11.7 on 2017-11-20 23:15
from __future__ import unicode_literals

import django.core.validators
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('games', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='game',
            name='strikes',
            field=models.IntegerField(default=2, validators=[django.core.validators.MinValueValidator(1), django.core.validators.MaxValueValidator(5)]),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='game',
            name='time',
            field=models.IntegerField(default=300, validators=[django.core.validators.MinValueValidator(30), django.core.validators.MaxValueValidator(600)]),
            preserve_default=False,
        ),
    ]