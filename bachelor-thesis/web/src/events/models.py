from django.db import models

from maps.models import Map

class Event(models.Model):
    title = models.CharField(max_length=200, blank=False, null=False)
    description = models.CharField(max_length=500, blank=False, null=False)
    content = models.TextField(blank=True, null=True)
    start_date = models.DateTimeField('Start date', blank=False, null=False)
    end_date = models.DateTimeField('End date', blank=False, null=False)
    map = models.ForeignKey(Map, blank=True, null=True)

    def __str__(self):
        return self.title

    def has_game(self):
        if(hasattr(self, 'game')):
            return self.game
        return None

    def to_json(self):
        return {
            'id': self.id,
            'title': self.title,
            'description': self.description,
            'content': self.content,
            'startDate': self.start_date,
            'endDate': self.end_date,
            'mapId': self.map.id if self.map else None,
            'game': self.game.to_json() if self.has_game() else None
        }

class Game(models.Model):
    title = models.CharField(max_length=200, blank=False, null=False)
    info = models.TextField(blank=False, null=False)
    winning_message = models.TextField(blank=False, null=False)
    qr_count = models.IntegerField(blank=False, null=False)
    event = models.OneToOneField(
        Event,
        on_delete=models.CASCADE,
        primary_key=True
    )

    def __str__(self):
        return self.title

    def to_json(self):
        return {
            'title': self.title,
            'info': self.info,
            'winningMessage': self.winning_message,
            'qrCount': self.qr_count
        }