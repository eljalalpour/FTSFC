packet_drainer::PacketDrainer(
    PACKETS 1000,
    SIZE 1000,
    PATH /home/eghaznavi/Milad/evaluate/sig19/packet_drains.txt,
	SRCIP 0.0.0.0,
	DSTIP 10.70.0.8,
	SRCETH e4:1d:2d:13:9e:d0,
	DSTETH f4:52:14:69:a5:20);

td::ToDPDKDevice(1);

packet_drainer
-> td;
