
(cl:in-package :asdf)

(defsystem "robot_msgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Head" :depends-on ("_package_Head"))
    (:file "_package_Head" :depends-on ("_package"))
    (:file "Speak" :depends-on ("_package_Speak"))
    (:file "_package_Speak" :depends-on ("_package"))
  ))