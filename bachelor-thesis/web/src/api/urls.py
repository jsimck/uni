from django.conf.urls import url

from . import views

urlpatterns = [
    # /api/v1/events/
    url(r'^v1/events/$', views.events, name="events"),
    # /api/v1/events/<event_id>
    url(r'^v1/events/(?P<event_id>[0-9]+)$', views.event, name='event'),

    # /api/v1/maps/
    url(r'^v1/maps/$', views.maps, name="maps"),
    # /api/v1/maps/<map_id>
    url(r'^v1/maps/(?P<map_id>[0-9]+)$', views.map, name='map')
]