/**
 ******************************************************************************
 * @file	netcard.c
 * @brief	Linux 双网卡平台自动切换网卡，网卡配置信息写入 /etc/ethX-setting，根据
 此配置为基础配置防火墙，路由


- 2015-09-30,Menglong Woo, MenglongWoo@aliyun.com
 	- brief

 * @attention
 *
 * ATTENTION
 *
 * <h2><center>&copy; COPYRIGHT </center></h2>
*/

#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "string.h"
#include "stdlib.h"

#define ETH0 (char*)"/etc/eth0-setting"
#define ETH1 (char*)"/etc/eth1-setting"

int netIpIsValid(char *ip)
{
	int i;

	i = inet_addr(ip);

	if((i == 0) || (i == 0xffffffff)) {
		return 0;
	}
	else {
		return 1;
	}
}

int netMaskIsValid(char *subnet)
{
	unsigned int b = 0, i, n[4];
	sscanf(subnet, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
	for(i = 0; i < 4; ++i) { //将子网掩码存入32位无符号整型
		b += n[i] << (i * 8);
	}
	b = ~b + 1;
	if((b & (b - 1)) == 0) {  //判断是否为2^n
		return 1;
	}
	return 0;
}

int netGwIsValid(char *ip, char *mask, char *gw)
{
	unsigned long iip, imask, igw;
	int ret;

	iip   = inet_addr(ip);
	imask = inet_addr(mask);
	igw   = inet_addr(gw);

	ret = netIpIsValid(gw);
	ret += netMaskIsValid(mask);

	if ( (imask & iip) == (imask & igw) &&
	     ret == 2) {
		return 1;
	}
	return 0;
}

int FormatCfg(
    char *file,
    char *ip,
    char *mask,
    char *gw,
    char *dns)
{
	FILE *fp;

	fp = fopen(file, "wb");
	if (fp == NULL) {
		printf("errr\n");
		return -1;
	}

	fprintf(fp, "IP=%s\n", ip);
	fprintf(fp, "Mask=%s\n", mask);
	fprintf(fp, "Gateway=%s\n", gw);
	fprintf(fp, "DNS=%s\n\n", dns);

	fclose(fp);
	return 0;
}

/**
 * @brief	检验是否需要切换网段\n
 			默认外网IP为 192.168.1.0 网段
 * @param	ip 外网IP
 * @param	mask 外网mask
 * @retval	1 外网 eth0：192.168.0.0/24 、内网 eth：192.168.1.0/24
 * @remarks
 * @see
 */
int CheckIPSwitch(char *ip, char *mask)
{
	unsigned long outside;
	unsigned long inside;
	unsigned long imask;

	outside = inet_addr(ip);
	inside  = inet_addr("192.168.0.0");
	imask   = inet_addr("255.255.255.0");

	outside = outside & imask;
	if (outside == inside) {
		return 1;
	}
	return 0;
}

#if 0
void Usage()
{
	printf(
	    "Usage:\n"
	    "	netcard [ip] [mask] [gw]\n");
}


//
// netcard ip mask gw
int main(int argc, char **argv)
{
	char ip[16];
	char mask[16];
	char gw[16];
	char dns[16];

	if (argc != 4) {
		Usage();
		return -1;
	}
	strcpy(ip,   argv[1]);
	strcpy(mask, argv[2]);
	strcpy(gw,   argv[3]);
	strcpy(dns,  argv[3]);

	if (0 == netIpIsValid(ip)) {
		printf("\tError IP\n");
		return 1;
	}
	if (0 == netMaskIsValid(mask)) {
		printf("\tError Mask\n");
		return 2;
	}
	if (0 == netIpIsValid(gw)) {
		printf("\tError Gateway\n");
		return 3;
	}
	if (0 == netGwIsValid(ip, mask, gw)) {
		printf("\tError Gateway and IP not a net\n");
		return 4;
	}


	if (CheckIPSwitch(ip, mask)) {
		printf("switch net\n");
		FormatCfg(ETH0, ip, mask, gw, dns);
		FormatCfg(ETH1, (char *)"192.168.1.254", mask, (char *)"192.168.1.254", dns);
	}
	else {
		printf("un switch net\n");
		FormatCfg(ETH0, ip, mask, gw, dns);
		FormatCfg(ETH1, (char *)"192.168.0.254", mask, (char *)"192.168.0.254", dns);
	}
	printf("ok\n");
	system("cat /etc/eth0-setting");
	system("cat /etc/eth1-setting");
	return 0;
}


#endif
