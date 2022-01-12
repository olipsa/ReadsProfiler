all:
	g++ -g -Wall -Wextra -O2 Server/main.cpp Server/Database.cpp Server/Server.cpp Server/ServerNetwork.cpp Server/Commands/Command.cpp Server/Commands/Download.cpp Server/Commands/Login.cpp Server/Commands/Register.cpp Server/Commands/Search.cpp Server/Commands/Upload.cpp Server/Commands/View.cpp -o server -l sqlite3 -pthread
	g++ -g -Wall -Wextra -O2 Client/main.cpp Client/Client.cpp Client/ClientNetwork.cpp -o client
clean:
	rm -f *~ client server