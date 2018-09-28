$ mkdir arm_jsoncpp
$ cp include/ arm_jsoncpp/ -r
$ cp src/lib_json/* arm_jsoncpp/
$ cd arm_jsoncpp/

# 编译静态库
$ arm-linux-gnueabihf-g++ -c *.cpp -I./include -fPIC
$ ar cr libjsoncpp.a *.o

# 编译动态库
$ arm-linux-gnueabihf-g++ -shared -fPIC *.cpp -I./include -o libjsoncpp.so
