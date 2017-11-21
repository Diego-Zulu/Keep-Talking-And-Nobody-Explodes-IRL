
from django.conf.urls import url
from games import views

urlpatterns = [
    url(r'^$', views.games_index, name = 'games_index'),
    url(r'^list/$', views.games_list, name = 'games_list'),
    url(r'^handbook/$', views.games_handbook, name = 'games_handbook'),

    url(r'^play/$', views.games_play, name = 'games_play'),

    url(r'^play/get_current_bomb/$', views.current_game, name = 'current_game'),
    url(r'^play/(?P<game_id>[0-9]+)/start/$', views.start_game, name='start_game'),
    url(r'^play/c/end/$', views.end_game, name='end_game'),
    url(r'^play/c/restart/$', views.restart_game, name='restart_game')
]

