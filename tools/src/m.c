#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stropts.h>

void fun(char *p)
{
	*p = NULL;

}
int main()
{
	int fd;
	// fd = open("/dev/pts/2", O_RDWR );
	// if (fd == -1) {
	// 	perror("open /dev/ptmx faild\n");
	// 	exit(0);
	// }
	// printf("O_RDWR %x\n", O_RDWR);
	// char buf[1024];
	// // write(fd, "ls", 2);
	// ioctl(fd, I_PUSH, "ls");

	// sleep(10);
	// read(fd, buf, 1024);
	// printf("buf : %s\n", buf);
	// sleep(10);
	// close(fd);


	FILE *stream = NULL;
	char strout[256];
	int ret;
	int i = 0;
	// while(1) {
	// 	i++;
	stream = popen("cat /sys/class/net/eth3/address", "r");
	if (stream == NULL) {
		printf("i %d\n", i);
		exit(0);
	}
	// }
	printf("[%d]\n", stream);
	// 读出内容，并在末尾添加字符串终结符号
	ret = fread( strout,  1,30, stream);
	strout[ret] = '\0';
	printf("ret %d %s\n", ret, strout);
	ret = pclose(stream);

	return 0;
}