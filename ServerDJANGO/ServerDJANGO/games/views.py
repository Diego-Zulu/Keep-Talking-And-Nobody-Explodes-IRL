import json

from django.shortcuts import render
from django.http import JsonResponse, HttpResponse, HttpResponseBadRequest
from django.core.serializers import serialize
from django import shortcuts

from games.models import Game
from PiServer import ServerMQTT

def games_index(request):
    return render(request, 'index.html')

def games_list(request):
    return render(
        request, 'list.html', 
        {
            'games' : Game.objects.all()
        }
    )

def games_play(request):
    return render(request, 'play.html')

def current_game(request):
    server = ServerMQTT.get_instance()
    data = None
    if server.is_running():
        data = {
            'active' : True,
            'time' : server.get_time(),
            'game' : json.loads(serialize('json', [server.get_game()]))
        }
    else:
        data = { 'active' : False }
    return JsonResponse(data)

def start_game(request, game_id):
    server = ServerMQTT.get_instance()
    if server.is_running():
        return HttpResponseBadRequest()
    game = shortcuts.get_object_or_404(Game, pk = game_id)
    server.start(game)
    return current_game(request)

def end_game(request):
    ServerMQTT.get_instance().end()
    return current_game(request)

def restart_game(request):
    ServerMQTT.get_instance().restart()
    return current_game(request)

def games_handbook(request):
    return render(request, 'handbook.html')