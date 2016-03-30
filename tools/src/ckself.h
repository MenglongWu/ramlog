#ifndef _CHECK_SELF_H_
#define _CHECK_SELF_H_

#include "producetest.h"

int ti_ck_self(void *arg);
int ti_check_smscat(void *arg);
int ti_ck_sms_param(void *arg);
int ti_check_simcard(void *arg);
int ti_ck_mutil_strstr(struct ck_self *ptiem, int len);
int ti_ck_mutil(struct ck_self *ptiem, int len);
int part_configx(char *str, char *nval, char *val, int maxlen);
int read_rawconfig(char *file, char *nval, char *val, int maxlen);
int read_shval(char *file, char *nval, char *val, int maxlen);
int ti_ck_self(void *arg);
#endif