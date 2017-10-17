#!/usr/bin/env python
# coding=UTF-8
import roslib
#roslib.load_manifest('video')
import sys
import rospy
import cv2
from robot_msgs.msg import *
from std_msgs.msg import String
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage
from cv_bridge import CvBridge, CvBridgeError
import sys,time
import numpy as np
import requests  
import json 
#from scipy.ndimage import filters
global bgr_image
import xml.dom.minidom

class Face:
	
    def __init__(self):
        self.outer_id='renxl'
        self.api_key='ROGBcaFypek59wt-zG_p64dR1FUKV_7c'
        self.api_secret='Sc4Bhg61foFEFc2q_hbnwHlNQ2zLqQ4T'
        self.image_file='temp.png'
        self.bridge = CvBridge()
        self.flag = 0
        self.image_sub = rospy.Subscriber("/camera/image",Image,self.imageCallback)
        self.chat = rospy.Subscriber("/face_chat",String,self.chatCallback)
        
    def face_detect(self,image): 
        url = 'https://api-cn.faceplusplus.com/facepp/v3/detect'  
        files = {'image_file':open(image, 'rb')}  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'return_landmark': 0,  
                   'return_attributes':'gender,age,glass'
                   }   
        r = requests.post(url,files=files,data=payload)  
        data=json.loads(r.text)  
        #print r.text  
        width = data['faces'][0]['face_rectangle']['width']  
        top = data['faces'][0]['face_rectangle']['top']  
        height = data['faces'][0]['face_rectangle']['height']  
        left = data['faces'][0]['face_rectangle']['left']  
        face_tokens = data['faces'][0]['face_token']
        return face_tokens  
        img = cv2.imread(image)  
        vis = img.copy()  
        cv2.rectangle(vis, (left, top), (left+width, top+height),(0, 255, 0), 2)  
        cv2.imshow("Image", vis)  
        cv2.waitKey (0)

    def faceset_create(self):  
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/create'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'display_name':'renxl-face',  
                   'outer_id':'renxl'  
                   }
        r = requests.post(url,data=payload)  
        return r.text  
    
    def add_facetoken(self,face_tokens):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/addface'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'outer_id':'renxl',  
                   'face_tokens':face_tokens  
                   }
        r = requests.post(url,data=payload)  
        return r.text 	
    
    def remove_facetoken(self,face_tokens):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'outer_id':'renxl',  
                   'face_tokens':face_tokens  
                   }
        r = requests.post(url,data=payload)  
        return r.text
    
    def update_faceset_args(self,faceset,new_args):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/update'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'outer_id':faceset,
                   'new_outer_id': new_args, 
                   }
        r = requests.post(url,data=payload)  
        return r.text
    
    def get_faceset_details(self,outer_id):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/getdetail'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'outer_id':outer_id
                   }
        r = requests.post(url,data=payload)  
        return r.text 
    
    def get_facesets_lists(self,outer_id):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/faceset/getfacesets'  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,  
                   'outer_id':self.outer_id,
                   'start': 1
                   }
        r = requests.post(url,data=payload)  
        return r.text
    
    def face_compare(self,file1,file2):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/compare'
        files = {'image_file1':open(file1, 'rb'),'image_file2':open(file2, 'rb')}  
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret
                  }
        r = requests.post(url,files=files,data=payload)  
        return r.text

    def find_name(self,face_token):
        dom1=xml.dom.minidom.parse('./data/face_databases.xml')
        root=dom1.documentElement
        node= root.getElementsByTagName('faceset')[0]
        
        booknode=root.getElementsByTagName('nubmer')
        for booklist in booknode:
            face_tokens=booklist.getElementsByTagName('face_token')[0].childNodes[0].nodeValue.strip()
            if face_tokens == face_token:
                bookdict={}
                bookdict['id']=booklist.getAttribute('id') 
                bookdict['face_token']=booklist.getElementsByTagName('face_token')[0].childNodes[0].nodeValue.strip() 
                bookdict['name']=booklist.getElementsByTagName('name')[0].childNodes[0].nodeValue.strip() 
                bookdict['remark']=booklist.getElementsByTagName('remark')[0].childNodes[0].nodeValue.strip()
                return bookdict['name']
        return 'none'

    def face_search(self,image):
        url = 'https://api-cn.faceplusplus.com/facepp/v3/search'
        files = {'image_file':open(image, 'rb')} 
        payload = {'api_key': self.api_key,  
                   'api_secret': self.api_secret,
                   'outer_id':self.outer_id
                   }
        r = requests.post(url,files=files,data=payload)  
        #print r.text
        data=json.loads(r.text)   
        confidence = data['results'][0]['confidence'] 
        face_tokens = data['results'][0]['face_token']
        #print confidence
        #print face_tokens
        if confidence > 80.0:
            name = self.find_name(face_tokens)
            print 'confidence:' + str(confidence) + '  face_tokens: ' + face_tokens + '  name: ' + name
            return name

    def imageCallback(self,msg):
        #print "Processing frame | Delay:%6.3f" % (rospy.Time.now() - msg.header.stamp).to_sec()
        orig_image = self.bridge.imgmsg_to_cv2(msg, "rgb8")
        bgr_image = cv2.cvtColor(orig_image, cv2.COLOR_RGB2BGR)
        #cv2.imshow("Image window", bgr_image)
        if self.flag > 0 and self.flag < 4:
            print '---------------------'
            cv2.imwrite('./data/temp.png',bgr_image)
            self.flag += 4
        #cv2.waitKey(1)

    def aiui_service_client(self,str_data):
        rospy.wait_for_service('/speak') 
        try:
            request = rospy.ServiceProxy('speak', Sum_Two)
            resp = request(2, 11,'xiaoyan',str_data)
            return resp.code
        except rospy.ServiceException, e:
            print "Service call failed: %s"%e
            
    def chatCallback(self,msg):
        if msg.data == "看看我是谁":
            self.flag = 1
            print msg.data
        elif msg.data == "你认识我吗":
            self.flag = 2
            print msg.data
        elif msg.data == "解锁":
            self.flag = 3
            print msg.data
            #self.face_search('temp.png')

def main(args):
    rospy.init_node('face_service', anonymous=True)
    face_service = Face()
    #print face_service.face_search('./data/2.png')
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        r.sleep()
        try:
            if face_service.flag > 4:
                print '-----------333333----------'
                name = face_service.face_search('./data/2.png')
                if name != 'none':
                    if face_service.flag - 4 == 1:
                        print '你是人见人爱，花见花开的' + name
                    elif face_service.flag - 4 == 2:
                        print '当然认识啦，你是' + name
                    elif face_service.flag - 4 == 3:
                        print '你好！' + name + '解锁成功'
                    #face_service.aiui_service_client(ssend)
                else:
                    ssend = '我们好像没见过哦'
                    face_service.aiui_service_client(ssend)
            face_service.flag = 0
        except KeyboardInterrupt:
            print("Shutting down")
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
