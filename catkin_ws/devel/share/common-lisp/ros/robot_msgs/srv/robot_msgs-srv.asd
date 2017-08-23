
(cl:in-package :asdf)

(defsystem "robot_msgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Speek" :depends-on ("_package_Speek"))
    (:file "_package_Speek" :depends-on ("_package"))
  ))