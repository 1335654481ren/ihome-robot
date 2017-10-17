; Auto-generated. Do not edit!


(cl:in-package robot_msgs-srv)


;//! \htmlinclude Speak-request.msg.html

(cl:defclass <Speak-request> (roslisp-msg-protocol:ros-message)
  ((id
    :reader id
    :initarg :id
    :type cl:fixnum
    :initform 0)
   (type
    :reader type
    :initarg :type
    :type cl:fixnum
    :initform 0)
   (arg1
    :reader arg1
    :initarg :arg1
    :type cl:string
    :initform "")
   (arg2
    :reader arg2
    :initarg :arg2
    :type cl:string
    :initform ""))
)

(cl:defclass Speak-request (<Speak-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Speak-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Speak-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot_msgs-srv:<Speak-request> is deprecated: use robot_msgs-srv:Speak-request instead.")))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <Speak-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:id-val is deprecated.  Use robot_msgs-srv:id instead.")
  (id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <Speak-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:type-val is deprecated.  Use robot_msgs-srv:type instead.")
  (type m))

(cl:ensure-generic-function 'arg1-val :lambda-list '(m))
(cl:defmethod arg1-val ((m <Speak-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:arg1-val is deprecated.  Use robot_msgs-srv:arg1 instead.")
  (arg1 m))

(cl:ensure-generic-function 'arg2-val :lambda-list '(m))
(cl:defmethod arg2-val ((m <Speak-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:arg2-val is deprecated.  Use robot_msgs-srv:arg2 instead.")
  (arg2 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Speak-request>) ostream)
  "Serializes a message object of type '<Speak-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'arg1))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'arg1))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'arg2))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'arg2))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Speak-request>) istream)
  "Deserializes a message object of type '<Speak-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'arg1) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'arg1) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'arg2) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'arg2) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Speak-request>)))
  "Returns string type for a service object of type '<Speak-request>"
  "robot_msgs/SpeakRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Speak-request)))
  "Returns string type for a service object of type 'Speak-request"
  "robot_msgs/SpeakRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Speak-request>)))
  "Returns md5sum for a message object of type '<Speak-request>"
  "ffe2ed0131a0c7d14cc080adec6a4104")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Speak-request)))
  "Returns md5sum for a message object of type 'Speak-request"
  "ffe2ed0131a0c7d14cc080adec6a4104")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Speak-request>)))
  "Returns full string definition for message of type '<Speak-request>"
  (cl:format cl:nil "uint8 id~%uint8 type~%string arg1~%string arg2~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Speak-request)))
  "Returns full string definition for message of type 'Speak-request"
  (cl:format cl:nil "uint8 id~%uint8 type~%string arg1~%string arg2~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Speak-request>))
  (cl:+ 0
     1
     1
     4 (cl:length (cl:slot-value msg 'arg1))
     4 (cl:length (cl:slot-value msg 'arg2))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Speak-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Speak-request
    (cl:cons ':id (id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':arg1 (arg1 msg))
    (cl:cons ':arg2 (arg2 msg))
))
;//! \htmlinclude Speak-response.msg.html

(cl:defclass <Speak-response> (roslisp-msg-protocol:ros-message)
  ((code
    :reader code
    :initarg :code
    :type cl:integer
    :initform 0))
)

(cl:defclass Speak-response (<Speak-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Speak-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Speak-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot_msgs-srv:<Speak-response> is deprecated: use robot_msgs-srv:Speak-response instead.")))

(cl:ensure-generic-function 'code-val :lambda-list '(m))
(cl:defmethod code-val ((m <Speak-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:code-val is deprecated.  Use robot_msgs-srv:code instead.")
  (code m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Speak-response>) ostream)
  "Serializes a message object of type '<Speak-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'code)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'code)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'code)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'code)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Speak-response>) istream)
  "Deserializes a message object of type '<Speak-response>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'code)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'code)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'code)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'code)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Speak-response>)))
  "Returns string type for a service object of type '<Speak-response>"
  "robot_msgs/SpeakResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Speak-response)))
  "Returns string type for a service object of type 'Speak-response"
  "robot_msgs/SpeakResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Speak-response>)))
  "Returns md5sum for a message object of type '<Speak-response>"
  "ffe2ed0131a0c7d14cc080adec6a4104")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Speak-response)))
  "Returns md5sum for a message object of type 'Speak-response"
  "ffe2ed0131a0c7d14cc080adec6a4104")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Speak-response>)))
  "Returns full string definition for message of type '<Speak-response>"
  (cl:format cl:nil "uint32 code~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Speak-response)))
  "Returns full string definition for message of type 'Speak-response"
  (cl:format cl:nil "uint32 code~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Speak-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Speak-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Speak-response
    (cl:cons ':code (code msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Speak)))
  'Speak-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Speak)))
  'Speak-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Speak)))
  "Returns string type for a service object of type '<Speak>"
  "robot_msgs/Speak")