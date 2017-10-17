;; Auto-generated. Do not edit!


(when (boundp 'robot_msgs::Head)
  (if (not (find-package "ROBOT_MSGS"))
    (make-package "ROBOT_MSGS"))
  (shadow 'Head (find-package "ROBOT_MSGS")))
(unless (find-package "ROBOT_MSGS::HEAD")
  (make-package "ROBOT_MSGS::HEAD"))
(unless (find-package "ROBOT_MSGS::HEADREQUEST")
  (make-package "ROBOT_MSGS::HEADREQUEST"))
(unless (find-package "ROBOT_MSGS::HEADRESPONSE")
  (make-package "ROBOT_MSGS::HEADRESPONSE"))

(in-package "ROS")





(defclass robot_msgs::HeadRequest
  :super ros::object
  :slots (_id _type _x _y ))

(defmethod robot_msgs::HeadRequest
  (:init
   (&key
    ((:id __id) 0)
    ((:type __type) 0)
    ((:x __x) 0)
    ((:y __y) 0)
    )
   (send-super :init)
   (setq _id (round __id))
   (setq _type (round __type))
   (setq _x (round __x))
   (setq _y (round __y))
   self)
  (:id
   (&optional __id)
   (if __id (setq _id __id)) _id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:serialization-length
   ()
   (+
    ;; uint8 _id
    1
    ;; uint8 _type
    1
    ;; uint32 _x
    4
    ;; uint32 _y
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _id
       (write-byte _id s)
     ;; uint8 _type
       (write-byte _type s)
     ;; uint32 _x
       (write-long _x s)
     ;; uint32 _y
       (write-long _y s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _id
     (setq _id (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _type
     (setq _type (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint32 _x
     (setq _x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _y
     (setq _y (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass robot_msgs::HeadResponse
  :super ros::object
  :slots (_x _y _error ))

(defmethod robot_msgs::HeadResponse
  (:init
   (&key
    ((:x __x) 0)
    ((:y __y) 0)
    ((:error __error) 0)
    )
   (send-super :init)
   (setq _x (round __x))
   (setq _y (round __y))
   (setq _error (round __error))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:error
   (&optional __error)
   (if __error (setq _error __error)) _error)
  (:serialization-length
   ()
   (+
    ;; uint32 _x
    4
    ;; uint32 _y
    4
    ;; uint8 _error
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _x
       (write-long _x s)
     ;; uint32 _y
       (write-long _y s)
     ;; uint8 _error
       (write-byte _error s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _x
     (setq _x (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _y
     (setq _y (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _error
     (setq _error (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(defclass robot_msgs::Head
  :super ros::object
  :slots ())

(setf (get robot_msgs::Head :md5sum-) "692066f040717cc8522264699ee67211")
(setf (get robot_msgs::Head :datatype-) "robot_msgs/Head")
(setf (get robot_msgs::Head :request) robot_msgs::HeadRequest)
(setf (get robot_msgs::Head :response) robot_msgs::HeadResponse)

(defmethod robot_msgs::HeadRequest
  (:response () (instance robot_msgs::HeadResponse :init)))

(setf (get robot_msgs::HeadRequest :md5sum-) "692066f040717cc8522264699ee67211")
(setf (get robot_msgs::HeadRequest :datatype-) "robot_msgs/HeadRequest")
(setf (get robot_msgs::HeadRequest :definition-)
      "uint8 id
uint8 type
uint32 x
uint32 y
---
uint32 x
uint32 y
uint8 error

")

(setf (get robot_msgs::HeadResponse :md5sum-) "692066f040717cc8522264699ee67211")
(setf (get robot_msgs::HeadResponse :datatype-) "robot_msgs/HeadResponse")
(setf (get robot_msgs::HeadResponse :definition-)
      "uint8 id
uint8 type
uint32 x
uint32 y
---
uint32 x
uint32 y
uint8 error

")



(provide :robot_msgs/Head "692066f040717cc8522264699ee67211")


