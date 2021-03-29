target:server client
.PHONY:target
cxx=g++
cflags= -g -std=c++11
src=server.cpp reactor.cpp event_loop.cpp demultiplexer.cpp event_handler.cpp
src_client=client.cpp
server:$(src)
	$(cxx) -o server $(cflags) $(src)
client:$(src_client)
	$(cxx) -o client $(cflags) $(src_client)
.PHONY:clean
clean:
	rm server client
