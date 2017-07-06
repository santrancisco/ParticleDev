#!/bin/bash

echo "[+] Installing dependencies and gcc toolchain"
apt-get update --fix-missing
apt-get install -y python-software-properties
add-apt-repository ppa:terry.guo/gcc-arm-embedded
apt-get update
apt-get install -y build-essential
apt-get install -y --force-yes gcc-arm-none-eabi
apt-get install -y dfu-util
apt-get install -y git-core
apt-get install -y libarchive-zip-perl

echo "[+] Downloading particle firmware and tropicssl"

mkdir -p /home/vagrant/src
cd /home/vagrant/src
git clone https://github.com/spark/firmware.git 
git clone https://github.com/santrancisco/tropicssl.git
cp -rfv ./tropicssl/include/tropicssl ./firmware/communication/src

echo "[+] Check out stable version v0.6.2"
cd firmware
git checkout tags/v0.6.2
echo "[+] Copy entire firmware folder to /vagrant and softlink ~/src/firmware/usr/applications so we can use any IDE"
cp -r ~/src/firmware /vagrant/
rm -rf ~/src/firmware/user/applications
ln -s /vagrant/firmware/user/applications ~/src/firmware/user/applications

echo "[+] Installing nodejs for particle cli "
curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
sudo apt-get install -y nodejs
echo "[+] Install particle cli"
npm install -g particle-cli

echo "[+] Now you should have a working environment:"
echo " "
echo "[+] You can create a project in ~/src/firmware/user/applications/"
echo "[+] You can first try 'make' an hw_serial2 app:"
echo "cd ~/src/firmware & make APP=hw_serial2"
echo ""
echo "[+] Copy the binary to vagrant shared folder."
echo "cp build/target/main/platform-0-lto/hw_serial2.bin /vagrant/"
echo ""
echo "[+] Make sure your core is in DFU mode by holding Mode button + click and release reset until you see flashing green light"
echo "[+] Using particle-cli (Install through npm) to flash your core with the new app: "
echo "particle flash --force  --usb  hw_serial2.bin"
echo ""
echo "[+] Now you can use minicom to monitor Serial port for debug 'minicom -D /dev/cu.usbmodem1411'"
#git clone https://github.com/digitalwizards/SparkCore-LEDMatrix-MAX7219-MAX7221.git

chown vagrant:vagrant -R /home/vagrant


