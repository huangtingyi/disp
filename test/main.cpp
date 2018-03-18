#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/inotify.h>  
#include <unistd.h>  
  
#define EVENT_NUM 12  
  
char *event_str[EVENT_NUM] =   
{  
    "IN_ACCESS",  
    "IN_MODIFY",  
    "IN_ATTRIB",  
    "IN_CLOSE_WRITE",  
    "IN_CLOSE_NOWRITE",  
    "IN_OPEN",  
    "IN_MOVED_FROM",  
    "IN_MOVED_TO",  
    "IN_CREATE",  
    "IN_DELETE",  
    "IN_DELETE_SELF",  
    "IN_MOVE_SELF"  
};  

int WatchFileCloseWrite(char sFileName[]);

int main(int argc, char *argv[])  
{  
    int fd;  
    int wd;  
    int len;  
    int nread;  
    char buf[BUFSIZ];  
    struct inotify_event *event;  
    int i;  
    
    
    if((i=WatchFileCloseWrite(argv[1]))<0) return -1;
    
    
    if(i==true)
    	printf("file %s %s.\n",argv[1],"IN_CLOSE_WRITE");
    	
    
    if(argc < 2)  
    {  
        fprintf(stderr, "%s path\n", argv[0]);  
        return -1;  
    }  
      
    fd = inotify_init();  
    if( fd < 0 )  
    {  
        fprintf(stderr, "inotify_init failed\n");  
        return -1;  
    }  
      
    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);  
    if(wd < 0)  
    {  
        fprintf(stderr, "inotify_add_watch %s failed\n", argv[1]);  
        return -1;  
    }  
      
    buf[sizeof(buf) - 1] = 0;  
    while( (len = read(fd, buf, sizeof(buf) - 1)) > 0 )  
    {  
        nread = 0;  
        while( len > 0 )  
        {  
            event = (struct inotify_event *)&buf[nread];  
            for(i=0; i<EVENT_NUM; i++)  
            {  
                if((event->mask >> i) & 1)  
                {  
                    if(event->len > 0)  
                        fprintf(stdout, "%s --- %s\ti=%d,m=%d,l=%d\n", event->name, event_str[i],i,event->mask,len);  
                    else  
                        fprintf(stdout, "%s --- %s\ti=%d,m=%d,l=%d\n", " ", event_str[i],i,event->mask,len);  
                }  
            }  
            nread = nread + sizeof(struct inotify_event) + event->len;  
            len = len - sizeof(struct inotify_event) - event->len;  
        }  
    }  
      
    return 0;  
}  
