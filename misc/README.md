# Misc

## Disclaimer

- These files are just some samples for reference. Please do not adopt directly without any revision.

## Firmware compile

1. download [buildroot](https://git.buildroot.net/buildroot)
2. get into buildroot folder
3. run

```
# generate .config
make raspberrypi4_64_defconfig
# enable features/options
make menuconfig
# do the compile with n cores
make -j$(nproc)
```

---

- make bootloader, rootfs, userdata partition

```
# umount the sd card first, if it has been mount
sudo umount /dev/sdb*
# clean up data
sudo wipefs -a /dev/sdb
# build MBR table
sudo parted --script /dev/sdb mklabel msdos
# 1. make boot partition
sudo parted --script /dev/sdb mkpart primary fat32 1MiB 256MiB
# 2. make rootfs partition
sudo parted --script /dev/sdb mkpart primary ext4 256MiB 16.3GiB
# 3. make userdata partition
sudo parted --script /dev/sdb mkpart primary ext4 16.3GiB 100%
sudo mkfs.vfat -F 32 -n BOOT /dev/sdb1
sudo mkfs.ext4 -L ROOTFS /dev/sdb2
sudo mkfs.ext4 -L USERDATA /dev/sdb3
mkdir -p /tmp/mnt_boot /tmp/mnt_root
sudo mount /dev/sdb1 /tmp/mnt_boot
sudo mount /dev/sdb2 /tmp/mnt_root
sudo cp -r output/images/rpi-firmware/* /tmp/mnt_boot/
sudo cp output/images/Image output/images/bcm2711-rpi-4-b.dtb /tmp/mnt_boot/

# if you have compile u-boot, you can also copy it into boot partition
# sudo cp output/images/u-boot.bin /tmp/mnt_boot/

# revise config.txt
enable_uart=1
kernel=u-boot.bin

```
