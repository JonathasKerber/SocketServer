# To compile the server
g++ src/*.cpp -o example -I inc/ -I <path_to_boost>/boost_1_71_0/ -L <path_to_boost_binary_libs>/lib/ -lpthread -lboost_filesystem

# To compile the test client
g++ TestClient.cpp -o TestClient -I <path_to_boost>/boost_1_71_0/ -L <path_to_boost_binary_libs>/lib/ -lpthread -lboost_filesystem
