#ifndef	_NETCARD_H_
#define _NETCARD_H_
int netIpIsValid(char *ip);

int netMaskIsValid(char* subnet);

int netGwIsValid(char *ip, char *mask, char* gw);

#endif

