# -*- coding: utf-8 -*-
# Generated by Django 1.11.7 on 2017-11-20 23:49
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('games', '0003_auto_20171120_2047'),
    ]

    operations = [
        migrations.AlterField(
            model_name='game',
            name='lit',
            field=models.ForeignKey(default=0, on_delete=django.db.models.deletion.CASCADE, to='basics.Lit'),
            preserve_default=False,
        ),
        migrations.AlterField(
            model_name='game',
            name='serial_number',
            field=models.ForeignKey(default=0, on_delete=django.db.models.deletion.CASCADE, to='basics.SerialNumber'),
            preserve_default=False,
        ),
    ]