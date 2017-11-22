import json

from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest, JsonResponse
from django.forms.models import model_to_dict
from django.core.serializers import serialize
from django import shortcuts

from games.models import Game
from services import GameService

def index(request):
    return render(request, 'index.html')

def list(request):
    return render(
        request, 'list.html', 
        {
            'games' : Game.objects.all()
        }
    )

def play(request):
    return render(request, 'play.html')

def handbook(request):
    return render(request, 'handbook.html')

def get_all(request):
    games = Game.objects.all()
    return HttpResponse(serialize('json', games, fields = ('name')), content_type = 'application/json')

def current_game(request):
    server = GameService.get_instance()
    data = None
    if server.is_running():
        data = {
            'active' : True,
            'time' : server.get_time(),
            'game' : model_to_dict(server.get_game())
        }
    else:
        data = { 'active' : False }
    return JsonResponse(data)

def start_game(request, game_id):
    server = GameService.get_instance()
    if server.is_running():
        return HttpResponseBadRequest()
    game = shortcuts.get_object_or_404(Game, pk = game_id)
    server.start(game)
    return current_game(request)

def end_game(request):
    GameService.get_instance().end()
    return current_game(request)

def restart_game(request):
    GameService.get_instance().restart()
    return current_game(request)