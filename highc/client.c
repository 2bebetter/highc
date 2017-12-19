#include <unp.h>
#include <string.h>

#define STATE_INIT '0'


void sendWithCode(int sockfd, char* msg, char* code, char*buf){
    strcpy(buf, msg);
    strcat(buf, code);
    Writen(sockfd, buf, strlen(buf));
}

void UploadFile(int sockfd,FILE* file,char* fileName){
    ssize_t n;
    char buf[MAXLINE];
    while((n=read(handle,buf,n)!=0){
        sendWithCode(sockfd,buf,'1\n',buf);
    }
}
void fileDistribute(FILE* fp, int sockfd){
    int maxfdpl;
    fd_set rset;
    ssize_t n;

    char state = STATE_INIT;

    char sendline[MAXLINE], recvline[MAXLINE];

    char s[MAXLINE];

    FILE* f;

    FD_ZERO(&rset);

    for(; ;){
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        maxfdpl = max(fileno(fp), sockfd) + 1;

        Select(maxfdpl, &rset, NULL, NULL, NULL);

        //socket is readable
        if(FD_ISSET(sockfd, &rset)){
            if((n = Read(sockfd, recvline, MAXLINE)) == 0){
                err_quit("server socket closed!!");
            }
            state = recvline[n - 2];
            recvline[n - 2] = '\0';               
            Fputs(recvline, stdout);
        }

        //input is readable
        if(FD_ISSET(fileno(fp), &rset)){
            if(Fgets(sendline, MAXLINE, fp) == NULL)
                return;
            if(state == STATE_Update){    //上传文件
                scanf("%s",s);
                f=open(s,O_RDONLY);
                UploadFile(sockfd,f);
            } else if(state == STATE_Download){ //下载文件，
                sendline[strlen(sendline) - 1] = '\0';
                sendWithCode(sockfd, sendline, "1\n", sendline);
            } else if(state == STATE_Show_File){ //显示文件列表
                sendWithCode(sockfd, sendline, "2\n", sendline);
            }
        }
    }
}

int main(int argc, char **argv){
	//Socket 文件描述符
    int sockfd;
    //下面这个结构体用来存储一个IP地址
	struct sockaddr_in servaddr;

    /*
     * 对控制台输入参数的判断：调用格式如==> operator_cli 127.0.0.1
     */
	if(argc != 2)
		err_quit("usage: operator_cli <IPaddress>\n");
	
    //创建一个TCP Socket
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    //将servaddr置0， 可以认为置0以后，该结构体里面存储的ip地址为0.0.0.0
	bzero(&servaddr, sizeof(servaddr));

    //指定服务器地址的协议族为 AF_INET(TCP/IP协议族)
	servaddr.sin_family = AF_INET;
    //指定服务器地址的端口
	servaddr.sin_port = htons(9748);

    //下面这一行将控制台输入的第二个参数（即IP地址），存放到地址结构体当中
    //emm，里面可能涉及主机字节序转网络字节序什么的，但是不用关心
    //只要知道调用下面的函数可以把形如“127.0.0.1”这样的字符串存放到servaddr这个结构体当中
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    /**
     * 下面这一行是客户端请求连接servaddr所指向的服务器（根据结构体里面保存的地址和端口）
     */
	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    /**
     * 下面是成功连接到服务器以后，处理具体的逻辑，这是一个自定义的函数
     * 即输入一个表达式传送到服务器端，
     * 再从服务器端读取这个表达式的结果打印出啦
     */
	fileDistribute(stdin, sockfd);
	exit(0);
}
