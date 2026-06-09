/*
 * my_ioctl_codes.h - header file with declarations ioctl 
 * userspace and kernelspace proccesses both should know this codes
 */

#ifndef MY_IOCTL_CODES_H
#define MY_IOCTL_CODES_H

#include <linux/ioctl.h>

/* Use random example of magic number */
#define MAGIC_NUMBER '\x44'

#define IOCTL_SET_MSG _IOW(MAGIC_NUMBER, 0, char *)
#define IOCTL_GET_MSG _IOR(MAGIC_NUMBER, 1, char *)
/* Driver put a message into passed buffer */
#define IOCTL_GET_NTH_BYTE _IOWR(MAGIC_NUMBER, 2, int)
/* Gets number n and return message[n]
 */

/* Name file device */
#define DEVICE_FILE_NAME "char_dev"
#define DRIVER_NAME "char_dev"
#define DEVICE_PATH "/dev/device_0"

#endif /* MY_IOCTL_CODES_H */
