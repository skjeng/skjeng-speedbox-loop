#!/bin/bash

echo "setup.sh"
pacman -Syu
pacman -Sy iperf3
cp speedbox.service /etc/systemd/system/speedbox.service
cd /etc/systemd/system/
systemctl enable speedbox
