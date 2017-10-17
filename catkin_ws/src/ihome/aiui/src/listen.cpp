#include "ros/ros.h"
#include "robot_msgs/Ear.h"

void get_skippattern(const char *pattern, int* next, int len)
{
    int pos = 2;
    int subStrIndex = 0; //valid prefix candidate substring index;
    next[0] = -1; // when 1st char mismatched, always move 1 (p=0, k=-1);
    next[1] = 0;  // when 2nd mismatched, always move 1(p=1, k=0);in fact, if the 2nd char is same as 1st char, we can move 2
    while(pos<len)
    {
        if(pattern[pos - 1] == pattern[subStrIndex]) //one char matched, then continue to match more, 
        {
            subStrIndex++;            //prefix substring move ahead;
            next[pos] = subStrIndex;//for current position, the k is got; 
            pos++;                    //current pos move ahead;
        }
        else if(subStrIndex>0)    //one substring found, but in the new pos, mismatched;
        {
            subStrIndex = next[subStrIndex]; //then we need fall back subStrIndex to value that still can be matched;
        }
        else
        {
            next[pos] = 0;
            pos++;
        }
    }
}

int KMP_search(const char *src, int slen, const char *pattern, int plen)
{
    int* next = (int *)malloc(sizeof(int)*slen);
    get_skippattern(pattern,next,plen);

    int indexInSrc = 0;
    int offset = 0;
    while((indexInSrc+offset)<slen)
    {
        if(pattern[offset] == src[indexInSrc+offset])
        {
            if(offset == (plen-1))
                return indexInSrc;
            offset++;
        }else
        {
            indexInSrc += offset-next[offset];
            if(next[offset]>-1)
                offset = next[offset];
            else
                offset = 0;
        }
    }
    return slen;
}

void chatterCallback(const robot_msgs::Ear::ConstPtr& msg)   //callback函数，收到msg时调用
{
  	printf("get data :%s\n",msg->data.c_str());
	const char *pat = msg->data.c_str();
	const char *src = "看看我是谁，你认识我吗，解锁";
	int index = KMP_search(src,strlen(src),pat,strlen(pat));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/aiui_ear", 10, chatterCallback);

  ros::spin();

  return 0;
}
