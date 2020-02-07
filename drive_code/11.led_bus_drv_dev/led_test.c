
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/* led_test on
 * led_test off
*/
int main(int argc,char **argv)
{
	int fd;
	int val = 1;
	fd = open("/dev/led", O_RDWR);
	if(fd < 0){
		printf("can't open!\n\r");
	}
	/* �ж�������� */
	if(argc != 2){
input_err:	
		printf("Usage : \n");
		printf("%s <on|off>\n",argv[0]);//<>��ʾ��������ʡ��
		return -1;
	}
	
	if(!strcmp(argv[1],"on")){//����
		val = 1;
	}
	else if(!strcmp(argv[1],"off")){
		val = 0;
	}
	else{
		goto input_err;
	}
	write(fd, &val, 4);
	return 0;
}
