# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Map',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('title', models.CharField(max_length=200)),
                ('description', models.CharField(max_length=500, blank=True)),
                ('created', models.DateTimeField(verbose_name=b'Created')),
                ('updated', models.DateTimeField(null=True, verbose_name=b'Last Updated', blank=True)),
            ],
        ),
    ]
