; Auto-generated. Do not edit!


(cl:in-package robot_msgs-srv)


;//! \htmlinclude Head-request.msg.html

(cl:defclass <Head-request> (roslisp-msg-protocol:ros-message)
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
   (x
    :reader x
    :initarg :x
    :type cl:integer
    :initform 0)
   (y
    :reader y
    :initarg :y
    :type cl:integer
    :initform 0))
)

(cl:defclass Head-request (<Head-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Head-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Head-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot_msgs-srv:<Head-request> is deprecated: use robot_msgs-srv:Head-request instead.")))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <Head-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:id-val is deprecated.  Use robot_msgs-srv:id instead.")
  (id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <Head-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:type-val is deprecated.  Use robot_msgs-srv:type instead.")
  (type m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <Head-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:x-val is deprecated.  Use robot_msgs-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <Head-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:y-val is deprecated.  Use robot_msgs-srv:y instead.")
  (y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Head-request>) ostream)
  "Serializes a message object of type '<Head-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Head-request>) istream)
  "Deserializes a message object of type '<Head-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Head-request>)))
  "Returns string type for a service object of type '<Head-request>"
  "robot_msgs/HeadRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Head-request)))
  "Returns string type for a service object of type 'Head-request"
  "robot_msgs/HeadRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Head-request>)))
  "Returns md5sum for a message object of type '<Head-request>"
  "692066f040717cc8522264699ee67211")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Head-request)))
  "Returns md5sum for a message object of type 'Head-request"
  "692066f040717cc8522264699ee67211")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Head-request>)))
  "Returns full string definition for message of type '<Head-request>"
  (cl:format cl:nil "uint8 id~%uint8 type~%uint32 x~%uint32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Head-request)))
  "Returns full string definition for message of type 'Head-request"
  (cl:format cl:nil "uint8 id~%uint8 type~%uint32 x~%uint32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Head-request>))
  (cl:+ 0
     1
     1
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Head-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Head-request
    (cl:cons ':id (id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
))
;//! \htmlinclude Head-response.msg.html

(cl:defclass <Head-response> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:integer
    :initform 0)
   (y
    :reader y
    :initarg :y
    :type cl:integer
    :initform 0)
   (error
    :reader error
    :initarg :error
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Head-response (<Head-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Head-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Head-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot_msgs-srv:<Head-response> is deprecated: use robot_msgs-srv:Head-response instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <Head-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:x-val is deprecated.  Use robot_msgs-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <Head-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:y-val is deprecated.  Use robot_msgs-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'error-val :lambda-list '(m))
(cl:defmethod error-val ((m <Head-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot_msgs-srv:error-val is deprecated.  Use robot_msgs-srv:error instead.")
  (error m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Head-response>) ostream)
  "Serializes a message object of type '<Head-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'error)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Head-response>) istream)
  "Deserializes a message object of type '<Head-response>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'x)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'y)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'error)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Head-response>)))
  "Returns string type for a service object of type '<Head-response>"
  "robot_msgs/HeadResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Head-response)))
  "Returns string type for a service object of type 'Head-response"
  "robot_msgs/HeadResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Head-response>)))
  "Returns md5sum for a message object of type '<Head-response>"
  "692066f040717cc8522264699ee67211")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Head-response)))
  "Returns md5sum for a message object of type 'Head-response"
  "692066f040717cc8522264699ee67211")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Head-response>)))
  "Returns full string definition for message of type '<Head-response>"
  (cl:format cl:nil "uint32 x~%uint32 y~%uint8 error~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Head-response)))
  "Returns full string definition for message of type 'Head-response"
  (cl:format cl:nil "uint32 x~%uint32 y~%uint8 error~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Head-response>))
  (cl:+ 0
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Head-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Head-response
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':error (error msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Head)))
  'Head-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Head)))
  'Head-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Head)))
  "Returns string type for a service object of type '<Head>"
  "robot_msgs/Head")