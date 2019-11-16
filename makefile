all:tracker.cpp client.cpp generateSHA.cpp generateSHA.h generateMtorrent.cpp generateMtorrent.h
	g++ client.cpp generateSHA.cpp generateMtorrent.cpp -o client -lssl -lcrypto -pthread
	g++ tracker.cpp -o tracker -pthread  
