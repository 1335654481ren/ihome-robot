#!/usr/local/bin/python
#-*- coding: UTF-8 -*-
####################################################################
# python 自动重启本程序
####################################################################
import time
import sys
import os
from multiprocessing import Pool
import thread
import time
 
# 为线程定义一个函数
def print_time( threadName, delay):
   count = 0
   while count < 5:
      time.sleep(delay)
      count += 1
      print "%s: %s" % ( threadName, time.ctime(time.time()) )

def restart_program():
	thread.start_new_thread( print_time, ("Thread-1", 2, ) )
	python = sys.executable
	os.execl(python, python, * sys.argv)

if __name__ == "__main__":
	print 'start...'
	print "restart"
	time.sleep(2)
	restart_program()
