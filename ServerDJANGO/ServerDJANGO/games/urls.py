
from django.conf.urls import url
from games import views

urlpatterns = [
    #CRUD
    url(r'^all/$', views.get_all, name = 'get_all'),
    #STATIC
    url(r'^$', views.index, name = 'index'),
    url(r'^list/$', views.list, name = 'list'),
    url(r'^handbook/$', views.handbook, name = 'handbook'),
    url(r'^play/$', views.play, name = 'play'),
    #PLAY
    url(r'^play/get_current_bomb/$', views.current_game, name = 'current'),
    url(r'^play/(?P<game_id>[0-9]+)/start/$', views.start_game, name='start'),
    url(r'^play/c/end/$', views.end_game, name='end'),
    url(r'^play/c/restart/$', views.restart_game, name='restart')
]

