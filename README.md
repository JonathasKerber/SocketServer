# To compile the server
g++ src/*.cpp -o example -I inc/ -I /usr/local/include/boost_1_71_0/ -L/usr/include/lib/ -lpthread -lboost_filesystem

# To compile the test client
g++ TestClient.cpp -o TestClient -I /usr/local/include/boost_1_71_0/ -L /usr/include/lib/ -lpthread -lboost_filesystem
