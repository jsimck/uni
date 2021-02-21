from django.db import models

class Map(models.Model):
    title = models.CharField(max_length=200, blank=False, null=False)
    description = models.CharField(max_length=500, blank=True, null=True)
    created = models.DateTimeField('Created', blank=False, null=False)
    updated = models.DateTimeField('Last Updated', blank=True, null=True)
    mapData = models.TextField(blank=True)

    def __str__(self):
        return self.title

    def to_json(self):
        return {
            'id': self.id,
            'title': self.title,
            'description': self.description,
            'created': self.created,
            'updated': self.updated,
            'mapData': self.mapData
        }
