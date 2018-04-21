git status
git pull
git add --all
git commit -m "add imu src"
git config --global user.email "xiaoliang.ren@hobot.cc"
git config --global user.name "xiaoliang.ren"
git commit -m "add imu src"
git push origin master

git clone
git init
git config user.name .....
git config user.email ......

多账号登陆
参考：
1.https://www.jianshu.com/p/12badb7e6c10
2.http://1024chan.com/2018/01/20/git-remote-permision/
3.https://www.zhihu.com/question/46432334/answer/300371167

renxl@ubuntu:~/.ssh$ cat config 
Host github.com
HostName github.com
User git
IdentityFile ~/.ssh/id_rsa
Host github-1335654481ren
HostName github.com
User git
IdentityFile ~/.ssh/id_rsa_mygit
renxl@ubuntu:~/.ssh$ 

cd ~/ihome-robot/
git remote set-url origin github-1335654481ren:1335654481ren/web-socket.git
