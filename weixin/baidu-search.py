# -*- coding: utf-8 -*-  
#find title and href
import sys  
import re   
import urllib2
from PyQt5 import QtWidgets, QtGui  
from bs4 import BeautifulSoup  
def search(key,limit=5):
    search_url='http://www.baidu.com/s?wd=key&rsv_bp=0&rsv_spt=3&rsv_n=2&inputT=6391'
    #search_url='https://www.google.com.hk/?gws_rd=cr,ssl#safe=active&q=key'   
    req=urllib2.urlopen(search_url.replace('key',key))   
    html=req.read()
    soup=BeautifulSoup(html,"html.parser")
    linkpattern=re.compile("href=\"(.+?)\"")
    div=soup.find('div',id='wrapper').find('div',id='wrapper_wrapper').find('div',id='container').find('div',id='content_left')
    re_dict={}
    for i in range(1,limit):
        a=div.find('div',id=str(i)).find('h3').find('a')
        re_link=linkpattern.findall(str(a))
        re_title=a.text
        re_dict[re_title]=re_link[0]
    for r in re_dict:
        print r
        print re_dict[r]
        print '....................\n'
    return re_dict

if __name__=='__main__':
    while True:
       # app = QtWidgets.QApplication(sys.argv)
       # window = QtWidgets.QWidget();
       # window.show()
       # sys.exit(app.exec_())
       	key=raw_input('input key word:')  
        if key != "":
           search(key)

