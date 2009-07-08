#include "portfwd.h"
int main(int argc, char** argv)
{
    if(argc!=3)
    {
        printf("Usage: %s <lan ip> <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[2]);
    std::string lanip = std::string(argv[1]);
    printf("Using %s:%d\n", lanip.c_str(), port);
    Portfwd pf;
    if(!pf.init())
    {
        printf("Portfwd.init() failed.\n");
        return 2;
    }
    printf("External IP: %s\n", pf.external_ip().c_str());
    printf("%s\n", ((pf.add( lanip, port ))?"Added":"Failed to add") );

    printf("Any key to exit...\n");
    char foo;
    scanf("%c",&foo);

    printf("%s\n",  ((pf.remove( port ))?"Removed.":"Failed to remove") );
    return 0;
}

