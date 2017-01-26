#!/usr/bin/env bash
sudo make clean
sudo autoconf
sudo ./configure
sudo make elemlist
sudo make
sudo make install
