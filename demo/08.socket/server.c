#include <sys/types.h>       
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define SERVER_PORT 8899 
#define BACKLOG     10   //���������
//#define SERVER_IP   "192.168.1.105"

/* socket ����
 * bind		��
 * listen ����
 * accept ����
 * send   ��������
 * recv   ��������
 */
int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	int iAddrLen;
	int iRet;
	
	struct sockaddr_in tSocketServerAddr;
	struct sockaddr_in tSocketClientAddr; //���ڱ���ͻ��˵ĵ�ַ��Ϣ

	int iClientNum = -1;
	int RecvLen;
	unsigned char RecvBuff[1000];

	/* 1.���� */
 	iSocketServer = socket(AF_INET, SOCK_STREAM, 0); //IPV4��TCP
	if(-1 == iSocketServer){
		printf("socket error\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT); //���˿ں�ת��Ϊ�����ֽ��� 16λ
	
#ifdef SERVER_IP
	tSocketServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);//�̶�ip
#else
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY; //��ʾ���������е�IP
#endif

	memset(tSocketServerAddr.sin_zero, 0, 8); //���0 �Ա�����struct sockaddrͬ����С

	/* 2.�� */
  iRet = bind(iSocketServer, (struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
  if(-1 == iRet){
		printf("bind error\n");
		return -1;
  }
  
	/* 3.�� */
	iRet = listen(iSocketServer, BACKLOG);
  if(iRet < 0){
		printf("listen error\n");
		return -1;
  }

  while(1)
  {
		/* 4.���� */
		iAddrLen = sizeof(struct sockaddr);
		iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		if(iSocketClient != -1){
			iClientNum++;
			printf("get connet from client %d : %s\n", iClientNum, inet_ntoa(tSocketClientAddr.sin_addr)); //������ipת��Ϊascii
			
			if(!fork()){
				/* �ӽ��� */
				
				while(1){
					/* 5.���տͻ��˵����� */
					RecvLen = recv(iSocketClient, RecvBuff , 999, 0);
					
					if(RecvLen <= 0){//���ճ���
					
						close(iSocketClient);
						return -1;
					}
					else{
						printf("get msg from clinet %d : %s\n", iClientNum, RecvBuff);
					}
				}
			}
		}
  }
	/* 6.�رշ����� */
  close(iSocketServer);
	return 0;
}


