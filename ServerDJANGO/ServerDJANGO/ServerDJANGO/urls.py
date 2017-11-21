
from django.conf.urls import include, url
from django.contrib import admin

urlpatterns = [
    url(r'^admin/', admin.site.urls, name = 'admin'),
    url(r'^', include('games.urls'), name = 'games')

    #url(r'^modules/', include('modules.urls')),
    #url(r'^basics/', include('basics.urls')),
    #url(r'^games/', include('games.urls'))
]
