#bin/sh
#This sh script is a hook for system start
	export PATH=/bin:/sbin:/usr/bin:/usr/sbin
	export LD_LIBRARY_PATH=/lib:/usr/lib

	export PATH=$PATH:/mnt/mmcblk1p2/usr/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/utils_lib:/usr/lib/qt_lib

	cd /mnt/mmcblk1p2/usr/codesysctrl/
    	./codesyscontrol &
	sleep 3
	
	/sbin/ifconfig eth0 192.168.1.253
	/sbin/ifconfig eth0 netmask 255.255.255.0
	
	./IoTaskMIC7001 &
	
	ifconfig can0 down	
	canconfig can0 bitrate 250000
	canconfig can0 ctrlmode loopback off
	canconfig can0 start
	
	#wifi used for station
	#wpa_supplicant -d -Dwext -iwlan0 -B -c /etc/wpa_supplicant/wpa_supplicant.conf
	#ifconfig wlan0 192.168.43.10 up

	#wifi used for AP
	hostapd -B /etc/hostapd.conf
	ifconfig wlan0 192.168.2.253 up
	udhcpd /etc/udhcpd.conf



        

