
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>

/* key_test
*/
int main(int argc,char **argv)
{
	int fd;
	int cnt = 0;
  char key_val;
  
	int ret;
  struct pollfd fds[1];
  
	fd = open("/dev/buttons", O_RDWR);
	if(fd < 0){
		printf("can't open!\n\r");
	}
	/* �ж�������� */
	if(argc != 1){
		printf("Usage : \n");
		printf("%s \n",argv[0]);
		return -1;
	}
	fds[0].fd = fd;
	fds[0].events = POLLIN;//�ڴ����¼�:�����ݴ���ȡ
	
	while(1)
	{
		ret = poll(fds, 1, 5000);//5000ms
		if(0 == ret){
			printf("buttons timeout\n");
		}
		else{
			read(fd, &key_val, 1);
			printf("key_val = 0x%x\n", key_val);
		}
	}
	return 0;
}
