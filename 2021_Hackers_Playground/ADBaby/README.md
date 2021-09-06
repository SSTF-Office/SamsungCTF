# ADBaby
## Requirement
- Network

tcp:6666

- KVM must be available

AWS generally does not support virtualization based on hardware. Therefore, you must use AWS ec2 bare metal instances that have a high price.
Alternatively, it is recommended to setup on the local PC.


## Description (in public)

adb connect SERVER_IP:6666

## Run
### auto
```
cd src/docker
sudo ./run.sh
```

### manual
 - build
```
cd src/docker
docker build -t sctf/adb .
```
 - run
`docker run -p 6666:6666 -e ADBKEY="$(cat adbkey)" --name adb --device /dev/kvm -it sctf/adb`

## Flag
 - `SCTF{Do_U_th1nk_th1s_1s_adb}`

