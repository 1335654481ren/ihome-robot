# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render

# Create your views here.
from django.template import loader, Context
from django.http import HttpResponse
from map.models import MapPost

def archive(request):
    #posts = MapPost.objects.all()
    #t = loader.get_template("archive.html")
    #c = Context({ 'posts' : posts })
    #return HttpResponse(t.render(c))
    return HttpResponse("hello world ! i am renxiaoliang")
    #return render(request,'archive.html',Context = 'default')