make

gcc main.c -o main

insmod ioctl_basic.ko

DEV=/dev/temp
if [ -c $DEV ]; then
    echo "Device node: $DEV exists"
    rm $DEV
    echo "firstly remove device node: $DEV success"
fi

mknod $DEV c $(dmesg | grep "The major number for device is" | tail -n 1 | cut  -d ' ' -f 10) 0
echo "Make device node: $DEV success"

./main
dmesg

rmmod ioctl_basic
