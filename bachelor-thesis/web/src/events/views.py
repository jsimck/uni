from django.shortcuts import render
from django.http import JsonResponse
from django.core.serializers.json import DjangoJSONEncoder
from django.views.decorators.csrf import ensure_csrf_cookie
from django.contrib.auth.decorators import login_required
from django.core.exceptions import ObjectDoesNotExist

import json

from models import Event, Game
from maps.models import Map

@login_required(login_url='/')
@ensure_csrf_cookie
def index(request):
    if request.method == 'POST':
        # POST Method, add new event
        body = json.loads(request.body)

        e = Event()
        e.title = body['title']
        e.description = body['description']
        e.content = body['content']
        e.start_date = body['startDate']
        e.end_date = body['endDate']

        # Assign map
        if body['mapId']:
            e.map = Map.objects.get(id=body['mapId'])
        else:
            e.map = None

        e.save()
        return JsonResponse({
            'success': True,
            'data': e.to_json()
        }, safe=False)

    else:
        # GET Method, render maps list
        events_json = [e.to_json() for e in Event.objects.all()]
        maps_json = [m.to_json() for m in Map.objects.all()]

        return render(request, 'events/list.html', {
            'events': json.dumps(events_json, cls=DjangoJSONEncoder),
            'maps': json.dumps(maps_json, cls=DjangoJSONEncoder)
        })

@login_required(login_url='/')
@ensure_csrf_cookie
def rest(request, event_id):
    # DELETE method, delete event
    if request.method == 'DELETE':
        e = Event.objects.get(id=event_id)

        if not e:
            return JsonResponse({
                'success': False,
                'message': 'Event not found'
            })

        # Delete map
        deleted = e.delete()

        if deleted is not None:
            return JsonResponse({
                'success': False,
                'message': 'No Event was deleted'
            })
        else:
            return JsonResponse({
                'success': True,
                'data': {
                    'id': event_id
                }
            })

    # PUT method, update event
    if request.method == 'PUT':
        # Get Map
        e = Event.objects.get(pk=event_id)

        if e is None:
            return JsonResponse({
                'success': False,
                'message': 'Event not found'
            })

        # Update values
        body = json.loads(request.body)
        e.title = body['title']
        e.description = body['description']
        e.content = body['content']
        e.start_date = body['startDate']
        e.end_date = body['endDate']

        # Assign map
        if body['mapId']:
            e.map = Map.objects.get(id=body['mapId'])
        else:
            e.map = None

        # Create / update game model
        if body['game']:
            # Check if game model doesn't exists
            try:
                g = Game.objects.get(pk=e.id)
            except ObjectDoesNotExist:
                g = Game()
                g.event_id = e.id

            # Assign new data
            g.title=body['game']['title']
            g.info=body['game']['info']
            g.winning_message=body['game']['winningMessage']
            g.qr_count=body['game']['qrCount']

            # Save game
            g.save()
        else:
            # Check if game model exists and delete it
            try:
                g = Game.objects.get(pk=e.id)
                g.delete()
            except ObjectDoesNotExist:
                # Don't do anything if object does not exists
                pass

        # Return updated object
        e.save()
        return JsonResponse({
            'success': True,
            'data': e.to_json()
        }, safe=False)

    return JsonResponse({
        'success': False,
        'message': 'Invalid Request'
    }, safe=False)
