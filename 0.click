require(package "FTSFC");

ControlSocket(TCP, 10001, VERBOSE true);
se::FTStateElement(ID 1, FAILURE_COUNT 1);