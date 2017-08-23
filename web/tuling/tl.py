# -*- coding: utf-8 -*-
import urllib,urllib2
import sys
import json

reload(sys) 
sys.setdefaultencoding('utf-8') 

API_KEY = '23771317366d42ca8cbbc978a6a87364'
raw_TULINURL = "http://www.tuling123.com/openapi/api?key=%s&info=" % API_KEY

def result():
    for i in range(1,100):
        queryStr = raw_input("我:".decode('utf-8'))
        TULINURL = "%s%s" % (raw_TULINURL,urllib2.quote(queryStr))
        req = urllib2.Request(url=TULINURL)
        result = urllib2.urlopen(req).read()
        hjson=json.loads(result)
        length=len(hjson.keys())
        content=hjson['text']

        if length==3:
            return 'robots:' +content+hjson['url']
        elif length==2:
            return 'robots:' +content

if __name__=='__main__':
    print "你好，请输入内容:".decode('utf-8')
    contents=result()
    print contents
