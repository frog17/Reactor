#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>

using namespace std;
int main()
{
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = connect(clientfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    if(ret == -1) {
        cout << strerror(errno) << endl;
        exit(-1);
    }
    char ss[10] = "hello";
    //write(ret,ss,6);
    cout << send(clientfd, ss, 10, 0) << endl;
    cout << "connect successfully!" << endl;
    char recv_buff[20];
    int len = recv(clientfd, recv_buff, sizeof(recv_buff), 0);
    cout << "recv :" << recv_buff << endl;
    //sleep(3);
    
    close(ret);
    //shutdown(ret,SHUT_RDWR);
}