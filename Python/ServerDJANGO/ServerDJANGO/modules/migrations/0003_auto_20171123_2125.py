# -*- coding: utf-8 -*-
# Generated by Django 1.11.7 on 2017-11-24 00:25
from __future__ import unicode_literals

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('modules', '0002_auto_20171120_2047'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='morsecode',
            name='losses',
        ),
        migrations.RemoveField(
            model_name='morsecode',
            name='won',
        ),
        migrations.RemoveField(
            model_name='password',
            name='losses',
        ),
        migrations.RemoveField(
            model_name='password',
            name='won',
        ),
        migrations.RemoveField(
            model_name='simonsays',
            name='losses',
        ),
        migrations.RemoveField(
            model_name='simonsays',
            name='won',
        ),
        migrations.RemoveField(
            model_name='thebutton',
            name='losses',
        ),
        migrations.RemoveField(
            model_name='thebutton',
            name='won',
        ),
        migrations.RemoveField(
            model_name='wires',
            name='losses',
        ),
        migrations.RemoveField(
            model_name='wires',
            name='won',
        ),
    ]
