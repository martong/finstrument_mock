#!/bin/sh

if [ -z $1 ]
then
	echo "have to enter a frequency. Available frequencies are:"
	cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies
	exit
fi

echo "Available frequencies are:"
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies

# see http://www.thinkwiki.org/wiki/How_to_make_use_of_Dynamic_Frequency_Scaling

cpucount=`cat /proc/cpuinfo|grep processor|wc -l`

FLROOT=/sys/devices/system/cpu

i=0
while [ $i -ne $cpucount ]
do
	FLNMAX="$FLROOT/cpu"$i"/cpufreq/scaling_max_freq"
	FLNMIN="$FLROOT/cpu"$i"/cpufreq/scaling_min_freq"
	echo "$1 > $FLNMAX"
	echo $1 > $FLNMAX
	echo "$1 > $FLNMIN"
	echo $1 > $FLNMIN
	i=`expr $i + 1`
done

echo check cpu freq in /proc/cpuinfo
cat /proc/cpuinfo |grep -i mhz

