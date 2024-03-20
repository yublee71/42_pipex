#include <stdio.h>
#include <io.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main() {
    
    pid_t pid;
    
    int x;
    x = 0;
    
    pid = fork();
    
    if(pid > 0) {  // 부모 코드
        x = 1;
        printf("부모 PID : %d,  x : %d , return : %d\n",(int)getpid(), x, (int)pid);
    }
    else if(pid == 0){  // 자식 코드
        x = 2;
        printf("자식 PID : %d,  x : %d\n, return : %d\n",(int)getpid(), x, (int)pid);
    }
    else {  // fork 실패
        printf("fork Fail! \n");
        return -1;
    }
    return 0;
}