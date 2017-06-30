#!/bin/sh
username="nvidia"  
password="nvidia"  
ip="192.168.1.132"  
  
#start  
(sleep 1;  
 echo $username;  
 sleep 1;  
 echo $password;  
 sleep 1;   
 echo "cd /home/nvidia/";  
 sleep 1;  
 echo "ls -al";  
 sleep 50;   
 #echo exit  
)|telnet $ip  
#end  
