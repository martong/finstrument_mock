#!/bin/sh

cpucount=`cat /proc/cpuinfo|grep processor|wc -l`

FLROOT=/sys/devices/system/cpu

newSpeedTop=`awk '{print $1}' $FLROOT/cpu$i/cpufreq/scaling_available_frequencies`

i=0
while [ $i -ne $cpucount ]
do
	newSpeedTop=`awk '{print $1}' $FLROOT/cpu$i/cpufreq/scaling_available_frequencies`
	newSpeedLow=`awk '{print $NF}' $FLROOT/cpu$i/cpufreq/scaling_available_frequencies`
	FLNMAX="$FLROOT/cpu"$i"/cpufreq/scaling_max_freq"
	FLNMIN="$FLROOT/cpu"$i"/cpufreq/scaling_min_freq"
	echo "$newSpeedTop > $FLNMAX"
	echo $newSpeedTop > $FLNMAX
	echo "$newSpeedLow > $FLNMIN"
	echo $newSpeedLow > $FLNMIN

	i=`expr $i + 1`
done

echo check cpu freq in /proc/cpuinfo
cat /proc/cpuinfo |grep -i mhz

