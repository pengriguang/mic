#! /bin/sh auto run scirpt for linux, after linux start up, it does the emmc flash, partition, format and copy the ramdisk to the emmc.

echo "auto make system begin"

if [ -e '/dev/mmcblk0p1' ] && [ -e '/dev/mmcblk0p2' ] && [ -e '/dev/mmcblk0p3' ] && [ -e '/dev/mmcblk1p4' ]; then
	echo "already mk system"
	return
else
	fuser -km /mnt/mmcblk1p2
	sleep 2
	umount /mnt/mmcblk1p2
	
	(echo d; echo 2; echo d; echo 1; echo w) | fdisk /dev/mmcblk1
	(echo n; echo p; echo 1; echo 1; echo 800; echo t; echo b; echo n; echo p; echo 2; echo 801; echo 22000; echo n; echo p; echo 3; echo 22001; echo 28000; echo n; echo p; echo 4; echo 28001; echo; echo w) | fdisk /dev/mmcblk1
	echo "fdisk emmc finished"
fi

echo "copy emmc partition file"
sleep 2

if [ -e '/dev/mmcblk1p1' ] && [ -e '/dev/mmcblk1p2' ] && [ -e '/dev/mmcblk1p3' ] && [ -e '/dev/mmcblk1p4' ]; then
	#1.umount partition
	umount /userdata
	umount /userapp
	umount /mnt/mmcblk1p2
	echo "umount finish"
	
	#3.format mmcblk1p1 as fat
	mkfs.vfat /dev/mmcblk1p1
	echo "format k1p1 finish"

	#4.copy boot image from usb flash to emmcblk1p1
	mount /dev/mmcblk1p1 /mnt/mmcblk1p1/
	cd /mnt/mmcblk1p1
	cp /media/sda1/zImage .
	cp /media/sda1/uEnv.txt .
	cp /media/sda1/ramdisk.gz .
	cp /media/sda1/mic7001.dtb .
	cd
	umount /mnt/mmcblk1p1/
	echo "copy boot image finish"


	#5.copy usr lib from usb flash to memory
	cd /dev/shm/
	cp /media/sda1/usr.tar.gz .
	tar zxvf usr.tar.gz
	export PATH=/dev/shm/usr/bin:$PATH
	export LD_LIBRARY_PATH=/dev/shm/usr/lib:/dev/shm/usr/lib/utils_lib:$LD_LIBRARY_PATH
	echo "copy usr lib to memory finish"

	#6.format mmcblk1p2/mmcblk1p3/mmcblk1p4 as ext4
	(echo y) | mkfs.ext4 /dev/mmcblk1p2
	(echo y) | mkfs.ext4 /dev/mmcblk1p3
	(echo y) | mkfs.ext4 /dev/mmcblk1p4
	echo "format k1p2 to k1p4 finish"

	#6.copy usr lib from memory to mmcblk1p2 
	mount /dev/mmcblk1p2 /mnt/mmcblk1p2/
	cp usr.tar.gz /mnt/mmcblk1p2/
	cd /mnt/mmcblk1p2/
	tar zxvf usr.tar.gz
	rm -f usr.tar.gz
	echo "copy usr lib to flash finish"


	#7.copy auto run script to mmcblk1p2
	cp /media/sda1/autoStartProfile.sh .
	cp /media/sda1/autoStartRcs.sh .
	cd
	echo "copy auto run script finish"

	#8.copy 3S license and codesyscontrol to mmcblk1p3
	mount /dev/mmcblk1p3 /userapp
	cd /userapp
	mv /mnt/mmcblk1p2/usr/config/ipaddr.txt .
	mv /mnt/mmcblk1p2/usr/config/r_mins.txt .
	mv /mnt/mmcblk1p2/usr/config/sn.txt .
	mv /mnt/mmcblk1p2/usr/config/wifi_passwd.txt .
	mv /mnt/mmcblk1p2/usr/codesysctrl/mic7000d/ .
	cp /mnt/mmcblk1p2/usr/codesysctrl/3S.dat .
	cp /mnt/mmcblk1p2/usr/codesysctrl/IoTaskMIC7001 .
	chmod +x IoTaskMIC7001
	cd
	umount /userapp
	echo "copy lincense and IoTaskMIC7001 finish"
	
	#9. remove IoTaskMIC7001, then umount mmcblk1p2
	cd /mnt/mmcblk1p2/usr/codesysctrl
	rm -f IoTaskMIC7001
	echo "remove IoTaskMIC7001 from blk2"
	cd
	
	#15. umount mmcblk1p2
	umount /mnt/mmcblk1p2/
	cd

	#16.reboot 
	echo "auto make system end, the system will reboot"
	reboot
else
	echo "emmc partition has already finished"
fi

