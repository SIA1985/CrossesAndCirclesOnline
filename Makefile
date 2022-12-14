all: CNCOnline

CNCOnline:
	
	g++ -g -fpermissive -Wall main.cpp Network/Network.cpp Interface/Interface.cpp Game/Game.cpp Player/Player.cpp -o CNConline

