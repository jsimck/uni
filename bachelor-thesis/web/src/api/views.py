from django.http import JsonResponse

from events.models import Event
from maps.models import Map

def events(request):
    events_json = [e.to_json() for e in Event.objects.all()]
    return JsonResponse(events_json, safe=False)

def event(request, event_id):
    try:
        e = Event.objects.get(id=event_id)
        return JsonResponse(e.to_json(), safe=False)

    except Event.DoesNotExist:
        return JsonResponse({
            'success': False,
            'message': 'Event not found'
        }, safe=False)

def maps(request):
    maps_json = [m.to_json() for m in Map.objects.all()]
    return JsonResponse(maps_json, safe=False)

def map(request, map_id):
    try:
        m = Map.objects.get(id=map_id)
        return JsonResponse(m.to_json(), safe=False)

    except Map.DoesNotExist:
        return JsonResponse({
            'success': False,
            'message': 'Map not found'
        }, safe=False)