#! /bin/sh auto run scirpt for linux, after linux start up, it does the emmc flash, partition, format and copy the ramdisk to the emmc.
function run_mmc_case()
{
	printf "create four partitions\n"

	fdisk /dev/mmcblk1  2>&1 1>/dev/null  << EOF
	p
	d
	1
	d
	2
	d
	3
	d
	n
	p
	1
	1
	800
	t
	b
	n
	p
	2
	801
	22000
	n
	p
	3
	22001
	28000
	n
	p	


	w
EOF
	sleep 4
	if [ "$?" = 0 ]; then
		if [ -e '/dev/mmcblk1p1' ] && [ -e '/dev/mmcblk1p2' ] && [ -e '/dev/mmcblk1p3' ] && [ -e '/dev/mmcblk1p4' ]; then
			printf "MMC part ok! \n\n"
			return
		fi
	fi
	printf "MMC test fails \n\n"
}


echo "auto make system begin"

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
	mkfs.ext4 /dev/mmcblk1p2
	mkfs.ext4 /dev/mmcblk1p3
	mkfs.ext4 /dev/mmcblk1p4
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
	
	#10. mysql database
	mount /dev/mmcblk1p4 /userdata
	cd /userdata
	mkdir -p /userdata/database/var/run/mysqld/
	mkdir -p /userdata/database/var/log/
	touch /userdata/database/var/run/mysqld/mysqld.pid
	cd
	echo "mysql database directory finish"
	umount /userdata
	
	#11. copy mysql to /mnt/mmcblk1p2/usr
	cd /mnt/mmcblk1p2/usr
	cp /media/sda1/mysql.tar.gz .
	tar zxvf mysql.tar.gz
	rm -f mysql.tar.gz
	echo "mysql copy finish"
	
	#12. copy gcc to /mnt/mmcblk1p2/usr
	cd /mnt/mmcblk1p2/usr
	cp /media/sda1/gcc.tar.gz .
	tar zxvf gcc.tar.gz
	rm -f gcc.tar.gz
	echo "gcc copy finish"
	
	#13. copy python to /mnt/mmcblk1p2/usr
	cd /mnt/mmcblk1p2/usr
	cp /media/sda1/python.tar.gz .
	tar zxvf python.tar.gz
	rm -f python.tar.gz
	echo "python lib copy finish"
	
	#14. copy python third lib to /mnt/mmcblk1p2/usr/python/lib/python3.7
	cd /mnt/mmcblk1p2/usr/python/lib/python3.7
	cp /media/sda1/site-packages.tar.gz .
	tar zxvf site-packages.tar.gz
	rm -f site-packages.tar.gz
	echo "python third lib copy finish"
	
	#15. umount mmcblk1p2
	umount /mnt/mmcblk1p2/
	cd

	#16.reboot 
	echo "auto make system end, the system will reboot"
	reboot

else
	umount /mnt/mmcblk1p2
	#2.fdisk
	run_mmc_case
	echo "fdisk finish, system will reboot"
	sleep 2
	reboot
fi
