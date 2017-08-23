
(cl:in-package :asdf)

(defsystem "robot_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Ear" :depends-on ("_package_Ear"))
    (:file "_package_Ear" :depends-on ("_package"))
  ))