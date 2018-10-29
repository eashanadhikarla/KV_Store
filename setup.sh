g++ kv_client.cc \
	-std=c++17 \
	-lboost_system \
	-lboost_regex \
	-lboost_serialization \
	-lv8 \
	-lpthread \
	-lstdc++ \
	-o client
	
gcc kv_server.cc kv_struct.cc\
    -std=c++11 \
    -lboost_system \
    -lboost_regex \
    -lboost_serialization \
    -lv8 \
    -lpthread \
    -lstdc++ \
    -o server