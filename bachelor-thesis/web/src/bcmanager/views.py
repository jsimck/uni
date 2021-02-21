# -*- coding: utf-8 -*-
from django.shortcuts import render
from django.http import JsonResponse, HttpResponseRedirect
from django.core.serializers.json import DjangoJSONEncoder
from django.views.decorators.csrf import ensure_csrf_cookie
from django.core.exceptions import ObjectDoesNotExist
from django.contrib.auth import authenticate, login, logout

import json

@ensure_csrf_cookie
def index(request):
    if request.method == 'POST':
        # POST Method, handle login
        body = json.loads(request.body)
        user = authenticate(username=body['login'], password=body['password'])

        if user is not None:
            # The password verified for the user
            if user.is_active:
                # Check if remember me is checked
                if body['remember']:
                    request.session.set_expiry(0)

                # Login user
                login(request, user)

                # Return success action
                return JsonResponse({
                    'success': True
                }, safe=False)
            else:
                return JsonResponse({
                    'success': False,
                    'message': 'Uživatel byl zablokován'
                }, safe=False)
        else:
            # The authentication system was unable to verify the username and password
            return JsonResponse({
                'success': False,
                'message': 'Uživatelské jméno a heslo je špatné'
            }, safe=False)

    else:
        if request.user.is_authenticated():
            return HttpResponseRedirect('/events')
        else:
            return render(request, 'login.html')

def logout_view(request):
    logout(request)
    return HttpResponseRedirect('/')

def qr_view(request):
    return render(request, 'qrgenerator.html')