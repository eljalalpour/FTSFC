#!/usr/bin/env bash
cd /home/eghaznavi/Milad/MLNX_OFED_LINUX-4.0-2.0.0.1-ubuntu14.04-x86_64/DEBS

dpkg -i libibverbs1_1.2.1mlnx1-OFED.4.0.1.5.3.40200_amd64.deb
dpkg -i libibverbs1-dbg_1.2.1mlnx1-OFED.4.0.1.5.3.40200_amd64.deb
dpkg -i libibverbs-dev_1.2.1mlnx1-OFED.4.0.1.5.3.40200_amd64.deb

dpkg -i libmlx5-1_1.2.1mlnx1-OFED.4.0.1.6.3.40200_amd64.deb
dpkg -i libmlx5-dev_1.2.1mlnx1-OFED.4.0.1.6.3.40200_amd64.deb
dpkg -i libmlx5-1-dbg_1.2.1mlnx1-OFED.4.0.1.6.3.40200_amd64.deb

dpkg -i libmlx4-1_1.2.1mlnx1-OFED.4.0.1.6.2.40200_amd64.deb
dpkg -i libmlx4-dev_1.2.1mlnx1-OFED.4.0.1.6.2.40200_amd64.deb
dpkg -i libmlx4-1-dbg_1.2.1mlnx1-OFED.4.0.1.6.2.40200_amd64.deb

dpkg -i mlnx-ofed-kernel-utils_4.0-OFED.4.0.2.0.0.1.g595cb61_amd64.deb
dpkg -i mlnx-ofed-kernel-dkms_4.0-OFED.4.0.2.0.0.1.g595cb61_all.deb
dpkg -i mlnx-ofed-kernel-only_4.0-2.0.0.1_all.deb