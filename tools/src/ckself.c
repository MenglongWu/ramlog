#include <producetest.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <readline/readline.h>

int SetOpt(int m_fd,int nSpeed/* =9600 */, int nBits/* =8 */, char nEvent/* ='N' */, int nStop/* =1 */){
	struct termios newtio,oldtio;
	if(tcgetattr(m_fd,&oldtio)!=0){
		perror("SetupSerial");
		return -1;
	}
	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= (nBits==7) ? CS7 : CS8;

	switch(nEvent){
	case 'N':
	case 'n':
		newtio.c_cflag &= ~PARENB;
		newtio.c_iflag &= ~INPCK;
		break;
	case 'O':
	case 'o':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
	case 'e':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'S':
	case 's':
		newtio.c_cflag &= ~PARENB;
		newtio.c_cflag &= ~CSTOPB;
		break;
	default:
		return -2;
	}

	switch(nSpeed){
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}

	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |=  CSTOPB;

	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(m_fd,TCIFLUSH);

	if((tcsetattr(m_fd,TCSANOW,&newtio))!=0){
		perror("com set error.");
		return 1;
	}
	return 0;
}

// 检测短信猫是否存在
int ti_check_smscat(void *arg)
{
	int fd;
	char buffer[1024];
	int isfail = 1;

	fd=open(SMS_CAT, O_RDWR);
	if(fd<=0){
		perror("open failed.");
		return 1;
	}
	SetOpt(fd,9600,8,'N',1);
	
	for (int i = 0; i < 8;i++) {
		int len;
		len = write(fd,"AT\r\n",4);
		printf("write len = %d\n", len);
		if((len=read(fd,buffer,sizeof(buffer) - 1))<0) {
			perror("read failed.");
		}
		else if(len>0){
			buffer[len] = '\0';
			if (strstr(buffer, "OK") != NULL) {
				printf("buf %s\n", buffer);
				isfail = 0;
				break;
			}
		}
		usleep(1000*100);
	}
	close(fd);
	return isfail;
}

int ti_ck_sms_param(void *arg)
{
	char *pstr = (char*)arg;

	int fd;
	char buffer[1024];
	int isfail = 1;

	fd=open(SMS_CAT, O_RDWR);
	if(fd<=0){
		perror("open failed.");
		return 1;
	}
	SetOpt(fd,9600,8,'N',1);
	
	for (int i = 0; i < 8;i++) {
		int len;
		len = write(fd, pstr, strlen(pstr));
		// printf("write len = %d\n", len);
		if((len=read(fd,buffer,sizeof(buffer) - 1))<0) {
			perror("read failed.");
		}
		else if(len>0){
			buffer[len] = '\0';
			if (strstr(buffer, "OK") != NULL) {
				isfail = 0;
				break;
			}
		}
		usleep(1000*100);
	}
	close(fd);
	return isfail;

}


int ti_check_simcard(void *arg)
{
	int fd;
	char buffer[1024];
	// int isfail = 1;

	fd=open(SMS_CAT, O_RDWR);
	if(fd<=0){
		perror("open failed.");
		return 1;
	}
	SetOpt(fd,9600,8,'N',1);
	
	// for (int i = 0; i < 8;i++) 
	char *input = (char*)NULL;
	
	while(1)
	{
		input = readline((char*)"输出IO测试>");
		
		int len;
		len = write(fd,input,strlen(input));
		printf("len = %d\n", len);
		// len = write(fd,"AT+CSCA?\r\n",4);
		write(fd,"\r\n",2);
		usleep(1000*1000);
		// printf("write len = %d\n", len);
		if((len=read(fd,buffer,sizeof(buffer) - 1))<0) {
			
			
		}
		else {
			buffer[len] = '\0';
			printf("%s",buffer);
		}
		if (!input) {
			free(input);
		}
		
	}
	close(fd);
}

// int 
// {
// 	{
// 		(char*)"Checking driver /dev/watchdog",
// 		(char*)"/usr/MenglongWu/bin/bin/gnokii --getsmsc",
// 		(char*)"SMS center number is",
// 	}
// }

int ti_ck_mutil_strstr(struct ck_self *ptiem, int len)
{
	FILE *stream;
	char strout[256];
	int ret;
	int failcount = 0;

	for (int i = 0; i < len; i++) {
		stream = popen(ptiem[i].dir, "r");
		
		// 读出内容，并在末尾添加字符串终结符号
		ret = fread( strout, sizeof(char), sizeof(strout) - 1, stream);
		strout[ret] = '\0';
		pclose(stream);
		printf("%s", ptiem[i].notice);
		if (ptiem[i].condition == COND_EQ) {
			if (0 == strstr(strout, ptiem[i].cmp )) {
				failcount++;
			}
		}
		else if (ptiem[i].condition == COND_NOTEQ) {
			if (0 != strstr(strout, ptiem[i].cmp )) {
				failcount++;
			}
		}
		
	}
	return failcount;
}

int ti_ck_mutil(struct ck_self *ptiem, int len)
{
	FILE *stream;
	char strout[256];
	int ret;
	int failcount = 0;
	
	for (int i = 0; i < len; i++) {
		stream = popen(ptiem[i].dir, "r");
		// printf("popen ret %d ", (int)stream);
		// perror("popen()");
		if (NULL == stream) {
			continue;
		}
		printf("%s():%d\n", __FUNCTION__, __LINE__);
		//存在溢出，应该用 sizeof(strout) - 1
		// 读出内容，并在末尾添加字符串终结符号
		ret = fread( strout, sizeof(char), sizeof(strout) - 1, stream);
		strout[ret] = '\0';
		ret = pclose(stream);
		// printf("pclose ret %d ", ret);
		perror("pclose()");

		printf("%s():%d\n", __FUNCTION__, __LINE__);
		// printf("%s\n%s\n", strout, POS_WLAN0);
		printf("%s", ptiem[i].notice);

		printf("%s():%d\n", __FUNCTION__, __LINE__);
		if (ptiem[i].condition == COND_EQ) {
			if (0 != memcmp(strout, ptiem[i].cmp , strlen(ptiem[i].cmp) )) {
				failcount++;
			}
		}
		else if (ptiem[i].condition == COND_NOTEQ) {
			if (0 == memcmp(strout, ptiem[i].cmp , strlen(ptiem[i].cmp) )) {
				failcount++;
			}
		}
		
	}
	return failcount;
}

int part_configx(char *str, char *nval, char *val, int maxlen)
{
	char stroption[256];
	char strval[256];

	// 正则表达式分离配置信息字符串
	sscanf(str, "%[^= , +]",  stroption);
	sscanf(str, "%*[^=]= %[^@]",     strval);

	if ( strcmp(stroption, nval) == 0) {
		memcpy(val, strval, maxlen);
		return 1;
	}
	return 0;	
}

// ret 0 没有找到
// ret 1 找到
// ret -1 文件打开失败
int read_rawconfig(char *file, char *nval, char *val, int maxlen)
{
	// FILE *fp = NULL;
	FILE *fin = NULL;
	char *pret = (char*)1;
	char strinput[2048];
	int ret = 0;
	// 读取打开配置文件
	fin = fopen( file, "r");
	if (NULL == fin) {
		printf("Error: can't open config file :%s.\nCheck path\n", file);
		return -1;
	}

	/*                                                                                                                              
	 * Automatically generated C config: don't edit
	 * Linux/arm 3.0.15Hello-KuGo Kernel Configuration
	 */
	
	// 逐行读取配置信息
	while(NULL != pret) {
		pret = fgets(strinput, 2048, fin);
		ret = part_configx(strinput, nval, val, maxlen);
		if (ret) {
			break;
		}
	}

	fclose(fin);

	return ret;
}

int read_shval(char *file, char *nval, char *val, int maxlen)
{
	// FILE *stream;
	// char strout[256];
	// int ret;


	


	return 0;
}
int ti_ck_self(void *arg)
{
	char strwlan0[24]= POS_WLAN0;
	char strlan0[24]= POS_LAN0;
	struct ck_self ckitem[] = {
		{
			(char*)"Check battery and date",
			(char*)"date \"+%Y-%m-%d %H:%M:%S\"",
			(char*)"2012",
			COND_NOTEQ,
		},
		{
			(char*)"Checking Wlan0 position",
			(char*)"ls /sys/class/net/wlan0/device -l | awk -F '../../../' \"{print \\$2}\"",
			// POS_WLAN0,
			strwlan0,
		},
		{
			(char*)"Checking Lan0 position",
			(char*)"ls /sys/class/net/lan0/device -l | awk -F '../../../' \"{print \\$2}\"",
			strlan0,
			COND_EQ,
		},
		{
			(char*)"Checking Lan0 status",
			(char*)"cat /sys/class/net/lan0/carrier",
			(char*)"1",
			COND_EQ,
		},
		{
			(char*)"Checking driver /dev/tms_gpio",
			(char*)"ls /dev/tms_gpio0",
			(char*)"/dev/tms_gpio0",
			COND_EQ,
		},
		{
			(char*)"Checking driver /dev/input/event2",
			(char*)"ls /dev/input/event2",
			(char*)"/dev/input/event2",
			COND_EQ,
		},
		{
			(char*)"Checking driver /dev/watchdog",
			(char*)"ls /dev/watchdog",
			(char*)"/dev/watchdog",
			COND_EQ,
		},
	};
	struct ck_self sim[] = {
		{
			(char*)"Checking SIM Card",
			(char*)"/usr/4412/install/gnokii/bin/gnokii --getsmsc",
			(char*)"SMS center number is",
		}
	};
	struct ck_self dir_driver[] = {
		{
			(char*)"Checking Exynos4412_wdt.ko\n",
			(char*)"ls /usr/MenglongWu/driver/Exynos4412_wdt.ko",
			(char*)"/",
		},
		{
			(char*)"Checking all_load.sh\n",
			(char*)"ls /usr/MenglongWu/driver/all_load.sh",
			(char*)"/",
		},
		{
			(char*)"Checking gpio_load.sh\n",
			(char*)"ls /usr/MenglongWu/driver/gpio_load.sh",
			(char*)"/",
		},
		{
			(char*)"Checking gpio_unload.sh\n",
			(char*)"ls /usr/MenglongWu/driver/gpio_unload.sh",
			(char*)"/",
		},
		{
			(char*)"Checking tms_gpio.ko\n",
			(char*)"ls /usr/MenglongWu/driver/tms_gpio.ko",
			(char*)"/",
		},
	};
	struct ck_self dir_bin[] = {
		{
			(char*)"Checking initgpio.elf\n",
			(char*)"ls /usr/MenglongWu/bin/initgpio.elf",
			(char*)"/",
		},
		{
			(char*)"Checking netcard.elf\n",
			(char*)"ls /usr/MenglongWu/bin/netcard.elf",
			(char*)"/",
		},
		{
			(char*)"Checking targetExe-r\n",
			(char*)"ls /usr/MenglongWu/bin/targetExe-r",
			(char*)"/",
		},
		{
			(char*)"Checking test_gpio.elf\n",
			(char*)"ls /usr/MenglongWu/bin/test_gpio.elf",
			(char*)"/",
		},
		{
			(char*)"Checking tms4412.elf\n",
			(char*)"ls /usr/MenglongWu/bin/tms4412.elf",
			(char*)"/",
		},
		{
			(char*)"Checking udiskupdate.elf\n",
			(char*)"ls /usr/MenglongWu/bin/udiskupdate.elf",
			(char*)"/",
		},



		{
			(char*)"Checking udev-114/udevd\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevd",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevinfo\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevinfo",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevmonitor\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevmonitor",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevsettle\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevsettle",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevstart\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevstart",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevtest\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevtest",
			(char*)"/",
		},
		{
			(char*)"Checking udev-114/udevtrigger\n",
			(char*)"ls /usr/MenglongWu/bin/udev-114/udevtrigger",
			(char*)"/",
		},
	};

	int ret;
	int count = 0;
	char val[24]= {};
	
	// 读取配置文件
	ret = read_rawconfig((char*)"/usr/MenglongWu/bin/ptrc", (char*)"wlan0", val,24);
	if (1 == ret) {
		memcpy(strwlan0, val, 24);
	}

	ret = read_rawconfig((char*)"/usr/MenglongWu/bin/ptrc", (char*)"lan0", val,24);
	if (1 == ret) {
		memcpy(strlan0, val, 24);
	}


	// 自动测试逐个项目
	ret = ti_ck_mutil(&dir_driver[0], sizeof(dir_driver) / sizeof(struct ck_self));
	if (0 != ret) {
		RESOULT_NO_EN();
	}
	else {
		RESOULT_OK_EN();
	}
	count += ret;
	
	ret = ti_ck_mutil(&dir_bin[0], sizeof(dir_bin) / sizeof(struct ck_self));
	if (0 != ret) {
		RESOULT_NO_EN();
	}
	else {
		RESOULT_OK_EN();
	}
	count += ret;

	for (int i = 0; i < sizeof(ckitem) / sizeof(struct ck_self); i++) {
		ret = ti_ck_mutil(&ckitem[i], 1);
		count += ret;
		if (1 == ret) {
			RESOULT_NO_EN();
		}
		else {
			RESOULT_OK_EN();
		}
	}

	printf("Checking SMS Cat");
	ret = ti_ck_sms_param((char*)"AT\r\n");
	count += ret;
	if (0 != ret) {
		RESOULT_NO_EN();
	}
	else {
		RESOULT_OK_EN();
	}

	if (ret == 0 && arg == NULL) {
		ret = ti_ck_mutil_strstr(&sim[0], 1);
		count += ret;
		if (0 != ret) {
			RESOULT_NO_EN();
		}
		else {
			RESOULT_OK_EN();
		}
	}
	printf("Fail :%d\n", count);
	// ti_check_smscat(NULL);
	
	// ti_ck_sms_param("AT\r\n");
	// ti_ck_sms_param("AT+CSCA?\r\n");
	// ti_check_simcard(NULL);
	



	return count;
}
