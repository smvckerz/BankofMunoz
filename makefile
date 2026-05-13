CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
MYSQL_FLAGS = $(shell /opt/homebrew/opt/mysql-client/bin/mysql_config --cflags --libs)

db: src/main.cpp src/banner.cpp src/database.cpp src/menu.cpp include/banner.h include/database.h include/menu.h
	$(CXX) $(CXXFLAGS) -o db src/main.cpp src/banner.cpp src/database.cpp src/menu.cpp $(MYSQL_FLAGS)

clean:
	rm -f db