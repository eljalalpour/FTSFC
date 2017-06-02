sudo ovs-vsctl set-controller br0 tcp:10.4.4.3:6633
sudo apt-get install python-software-properties
sudo apt-add-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer
sudo apt-get install libboost-all-dev
sudo apt-get install autoconf
sudo apt-get install maven
git clone https://github.com/kohler/click
cd click 
sudo ./configure
sudo make
sudo make install
cd ..
git clone https://github.com/eljalalpour/FTSFC
git clone https://github.com/eljalalpour/Orchestrator-app
cd FTSFC
git pull origin master
sudo sh run.sh
cd ..
cd Orchestrator-app
sudo mvn package
