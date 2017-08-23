;; Auto-generated. Do not edit!


(when (boundp 'robot_msgs::Speek)
  (if (not (find-package "ROBOT_MSGS"))
    (make-package "ROBOT_MSGS"))
  (shadow 'Speek (find-package "ROBOT_MSGS")))
(unless (find-package "ROBOT_MSGS::SPEEK")
  (make-package "ROBOT_MSGS::SPEEK"))
(unless (find-package "ROBOT_MSGS::SPEEKREQUEST")
  (make-package "ROBOT_MSGS::SPEEKREQUEST"))
(unless (find-package "ROBOT_MSGS::SPEEKRESPONSE")
  (make-package "ROBOT_MSGS::SPEEKRESPONSE"))

(in-package "ROS")





(defclass robot_msgs::SpeekRequest
  :super ros::object
  :slots (_id _type _arg1 _arg2 ))

(defmethod robot_msgs::SpeekRequest
  (:init
   (&key
    ((:id __id) 0)
    ((:type __type) 0)
    ((:arg1 __arg1) "")
    ((:arg2 __arg2) "")
    )
   (send-super :init)
   (setq _id (round __id))
   (setq _type (round __type))
   (setq _arg1 (string __arg1))
   (setq _arg2 (string __arg2))
   self)
  (:id
   (&optional __id)
   (if __id (setq _id __id)) _id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:arg1
   (&optional __arg1)
   (if __arg1 (setq _arg1 __arg1)) _arg1)
  (:arg2
   (&optional __arg2)
   (if __arg2 (setq _arg2 __arg2)) _arg2)
  (:serialization-length
   ()
   (+
    ;; uint8 _id
    1
    ;; uint8 _type
    1
    ;; string _arg1
    4 (length _arg1)
    ;; string _arg2
    4 (length _arg2)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _id
       (write-byte _id s)
     ;; uint8 _type
       (write-byte _type s)
     ;; string _arg1
       (write-long (length _arg1) s) (princ _arg1 s)
     ;; string _arg2
       (write-long (length _arg2) s) (princ _arg2 s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _id
     (setq _id (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _type
     (setq _type (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; string _arg1
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _arg1 (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _arg2
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _arg2 (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass robot_msgs::SpeekResponse
  :super ros::object
  :slots (_code ))

(defmethod robot_msgs::SpeekResponse
  (:init
   (&key
    ((:code __code) 0)
    )
   (send-super :init)
   (setq _code (round __code))
   self)
  (:code
   (&optional __code)
   (if __code (setq _code __code)) _code)
  (:serialization-length
   ()
   (+
    ;; uint32 _code
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _code
       (write-long _code s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _code
     (setq _code (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass robot_msgs::Speek
  :super ros::object
  :slots ())

(setf (get robot_msgs::Speek :md5sum-) "ffe2ed0131a0c7d14cc080adec6a4104")
(setf (get robot_msgs::Speek :datatype-) "robot_msgs/Speek")
(setf (get robot_msgs::Speek :request) robot_msgs::SpeekRequest)
(setf (get robot_msgs::Speek :response) robot_msgs::SpeekResponse)

(defmethod robot_msgs::SpeekRequest
  (:response () (instance robot_msgs::SpeekResponse :init)))

(setf (get robot_msgs::SpeekRequest :md5sum-) "ffe2ed0131a0c7d14cc080adec6a4104")
(setf (get robot_msgs::SpeekRequest :datatype-) "robot_msgs/SpeekRequest")
(setf (get robot_msgs::SpeekRequest :definition-)
      "uint8 id
uint8 type
string arg1
string arg2
---
uint32 code

")

(setf (get robot_msgs::SpeekResponse :md5sum-) "ffe2ed0131a0c7d14cc080adec6a4104")
(setf (get robot_msgs::SpeekResponse :datatype-) "robot_msgs/SpeekResponse")
(setf (get robot_msgs::SpeekResponse :definition-)
      "uint8 id
uint8 type
string arg1
string arg2
---
uint32 code

")



(provide :robot_msgs/Speek "ffe2ed0131a0c7d14cc080adec6a4104")


