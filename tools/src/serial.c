#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>

int set_opt(int fd, int nspeed, int nbits, char nevent, int nstop)
{
    struct termios newtio, oldtio;
    int res_code;
    res_code = 0;
    if(tcgetattr(fd, &oldtio)!=0)
    {
        printf("setup serial 1");
        res_code = -1;
        goto usr_exit;
    }
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag |= CLOCAL|CREAD;
    newtio.c_cflag &=~CSIZE;
    switch(nbits){
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    default:
        printf("data bits error %d", nbits);
        res_code = -1;
        goto usr_exit;
    }
    switch(nevent){
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK|ISTRIP);
        break;
    case 'E':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &=~PARODD;
        newtio.c_iflag |=(INPCK|ISTRIP);
        break;
    case 'N':
        newtio.c_cflag &=~PARENB;
        break;
    default:
        printf("nevent error %d", nevent);
        res_code = -1;
        goto usr_exit;

    }
    switch(nspeed){
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        printf("speed error %d", nspeed);
        res_code = -1;
        goto usr_exit;

    }

    if(nstop == 1)
        newtio.c_cflag &=~CSTOPB;
    else if(nstop == 2)
        newtio.c_cflag |=CSTOPB;
    else
    {
        printf("stop bit error %d", nstop);
        res_code = -1;
        goto usr_exit;
    }

    newtio.c_cc[VTIME]= 0;
    newtio.c_cc[VMIN] = 1;

    tcflush(fd, TCIFLUSH);
    if(tcsetattr(fd, TCSANOW, &newtio) != 0)
    {
        printf("set com error tcsetattr");
        res_code = -1;
        goto usr_exit;
    }

    printf("set done!\n");
usr_exit:
    return res_code;
}


int write_port(char buf[], int write_bytes)
{
    int actual_bytes, len, retval;
    int error_cout;
    fd_set fs_write;
    struct timeval tv_timeout;
    actual_bytes = -1;
    retval = 0;
    if(fd == NULL)
    {
        printf("write port fd = %d", fd);
        goto usr_exit;
    }
    //设置等待时间
    FD_ZERO (&fs_write);
    FD_SET (fd, &fs_write);
    tv_timeout.tv_sec = TIMEOUT_SEC (write_bytes, port_baud);
    tv_timeout.tv_usec = TIMEOUT_USEC;
    // pthread_mutex_lock(&objMutex);
    actual_bytes = 0;
    error_cout = 0;
    //开始写
    while (actual_bytes < write_bytes)
    {
        retval = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);
        if(retval)
        {
            error_cout = 0;
            len = write(fd, buf + actual_bytes, write_bytes - actual_bytes);
            actual_bytes += len;
        }
        else
        {
            //错误连续10次，跳出
            error_cout++;
            if(error_cout > 100)
                break;
        }
    }
    // pthread_mutex_unlock(&objMutex);
usr_exit:
    return actual_bytes;
}

int open_port(char *str_port, int baud)
{
    int res_code;
    res_code = 0;
    fd = open(str_port, O_RDWR|O_NOCTTY);
    if(fd<0)
    {
        printf("open port error fd = %d\n", fd);
        res_code = -1;
        goto usr_exit;
    }
    //perror("open");
    res_code = set_opt(fd, baud, 8, 'N', 1);
    if(res_code != 0)
    {
        close(fd);
        fd = NULL;
        goto usr_exit;
    }
    res_code = pthread_create(&thread_rcv, NULL, serial_read_thread, (void *)this);
    if(res_code != 0)
    {
        printf("cread thread error %d", res_code);
        close(fd);
        fd = NULL;
        goto usr_exit;
    }
    port_baud = baud;
usr_exit:
    return res_code;
}