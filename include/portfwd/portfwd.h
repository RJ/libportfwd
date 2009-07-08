#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <iostream>
//fwd:
struct UPNPUrls;
struct IGDdatas;

class Portfwd
{
    public:
        Portfwd();
        ~Portfwd();
        bool init();
        bool add(const std::string& ip, unsigned short port);
        bool remove(unsigned short port);
        std::string external_ip();

    protected:
        struct UPNPUrls* urls;
        struct IGDdatas* data;
};

