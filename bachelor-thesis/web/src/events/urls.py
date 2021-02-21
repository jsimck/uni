from django.conf.urls import url

from . import views

urlpatterns = [
    # /events/
    url(r'^$', views.index, name='events'),
    # /events/[event_id]
    url(r'^(?P<event_id>[0-9]+)', views.rest, name='events_rest')
]