# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('maps', '0002_map_mapdata'),
        ('events', '0003_auto_20151122_1305'),
    ]

    operations = [
        migrations.AddField(
            model_name='event',
            name='map',
            field=models.ForeignKey(blank=True, to='maps.Map', null=True),
        ),
    ]
