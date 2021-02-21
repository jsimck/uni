from django.shortcuts import render
from django.http import JsonResponse
from django.core.serializers.json import DjangoJSONEncoder
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import ensure_csrf_cookie

import json

from events.models import Event
from models import Map

@login_required(login_url='/')
@ensure_csrf_cookie
def index(request):
    if request.method == 'POST':
        # POST Method, add new map
        body = json.loads(request.body)

        m = Map()
        m.title = body['title']
        m.description = body['description']
        m.created = body['created']
        m.mapData = json.dumps(body['mapData'])

        # Save and return new object
        m.save()
        return JsonResponse({
            'success': True,
            'data': m.to_json()
        }, safe=False)

    else:
        # GET Method, render maps list
        maps_json = [m.to_json() for m in Map.objects.all()]

        return render(request, 'maps/list.html', {
            'maps': json.dumps(maps_json, cls=DjangoJSONEncoder)
        })

@login_required(login_url='/')
@ensure_csrf_cookie
def rest(request, map_id):
    # DELETE method, delete map
    if request.method == 'DELETE':
        m = Map.objects.get(pk=map_id)

        if not m:
            return JsonResponse({
                'success': False,
                'message': 'Map not found'
            })

        # Delete map
        deleted = m.delete()

        if deleted is not None:
            return JsonResponse({
                'success': False,
                'message': 'No Map was deleted'
            })
        else:
            return JsonResponse({
                'success': True,
                'data': {
                    'id': map_id
                }
            })

    # PUT method, update map
    if request.method == 'PUT':
        # Get Map
        m = Map.objects.get(pk=map_id)

        if m is None:
            return JsonResponse({
                'success': False,
                'message': 'Map not found'
            })

        # Update values
        body = json.loads(request.body)
        m.title = body['title']
        m.description = body['description']
        m.created = body['created']
        m.updated = body['updated']
        m.mapData = json.dumps(body['mapData'])

        # Return updated object
        m.save()
        return JsonResponse({
            'success': True,
            'data': m.to_json()
        }, safe=False)

    return JsonResponse({
        'success': False,
        'message': 'Invalid Request'
    }, safe=False)

@login_required(login_url='/')
@ensure_csrf_cookie
def editor(request, map_id):
    # GET Method, render maps list
    m = Map.objects.get(pk=map_id)
    events_json = [e.to_json() for e in Event.objects.all()]

    return render(request, 'maps/editor.html', {
        'events': json.dumps(events_json, cls=DjangoJSONEncoder),
        'map': json.dumps(m.to_json(), cls=DjangoJSONEncoder)
    })