from django.conf.urls import url

from . import views

urlpatterns = [
    # /maps/
    url(r'^$', views.index, name='maps'),
    # /maps/[map_id]
    url(r'^(?P<map_id>[0-9]+)$', views.rest, name='maps_rest'),
    # /maps/[map_id]/editor
    url(r'^(?P<map_id>[0-9]+)/editor', views.editor, name='maps_editor')
]