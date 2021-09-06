#!/bin/bash
ADB=/android/sdk/platform-tools/adb

$ADB wait-for-device
$ADB root
$ADB forward tcp:5558 tcp:5556
$ADB push /home/sctf/adbd_sctf /data/local/tmp/adbd_sctf
$ADB push /home/sctf/flag /data/local/tmp/flag
$ADB shell chown root /data/local/tmp
$ADB shell chmod 551 /data/local/tmp

for (( ; ; ))
do
    $ADB shell mount -o remount,ro /storage/emulated
    if [ "$?" -eq 0 ]; then
        break
    fi
done

$ADB shell LD_LIBRARY_PATH=/apex/com.android.adbd/lib64 /data/local/tmp/adbd_sctf --root_seclabel=u:r:su:s0
