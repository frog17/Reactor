#include "include/reactor.h"

int main(){
    Reactor reactor;
    reactor.init(9999);
    reactor.run();
    return 0;
}