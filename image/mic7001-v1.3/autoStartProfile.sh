#bin/sh
#This sh script is a hook for system start
echo "4 4 1 7" > /proc/sys/kernel/printk
cd

if [ -f "/userapp/sn.txt" ]; then
	while read sn
	do
		break
	done < /userapp/sn.txt
	hostname $sn
fi
	
