# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Event',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('title', models.CharField(max_length=200)),
                ('content', models.TextField(blank=True)),
                ('start_date', models.DateTimeField(default=datetime.datetime(2015, 11, 22, 12, 0, 58, 383773, tzinfo=utc), verbose_name=b'Start date')),
                ('end_date', models.DateTimeField(verbose_name=b'End date', blank=True)),
            ],
        ),
    ]
