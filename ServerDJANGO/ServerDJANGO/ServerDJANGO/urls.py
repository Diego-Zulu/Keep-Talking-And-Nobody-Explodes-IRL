"""
Definition of urls for ServerDJANGO.
"""

from django.conf.urls import include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
# admin.autodiscover()

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^modules/', include('modules.urls')),
    url(r'^basics/', include('basics.urls')),
    url(r'^games/', include('games.urls'))

    # Examples:
    # url(r'^$', ServerDJANGO.views.home, name='home'),
    # url(r'^ServerDJANGO/', include('ServerDJANGO.ServerDJANGO.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
]
