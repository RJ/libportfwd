#include "portfwd/portfwd.h"

#include "miniwget.h"
#include "miniupnpc.h"
#include "upnpcommands.h"

Portfwd::Portfwd()
 : urls(0), data(0)
{
}

Portfwd::~Portfwd()
{
    if(urls) free(urls);
    if(data) free(data);
}

bool
Portfwd::init()
{
   struct UPNPDev * devlist;
   struct UPNPDev * dev;
   char * descXML;
   int descXMLsize = 0;
   printf("TB : init_upnp()\n");
   urls = (UPNPUrls*)malloc(sizeof(struct UPNPUrls));
   data = (IGDdatas*)malloc(sizeof(struct IGDdatas));
   memset(urls, 0, sizeof(struct UPNPUrls));
   memset(data, 0, sizeof(struct IGDdatas));
   devlist = upnpDiscover(2000, NULL, NULL, 0);
   if (devlist)
   {
       dev = devlist;
       while (dev)
       {
           if (strstr (dev->st, "InternetGatewayDevice"))
               break;
           dev = dev->pNext;
       }
       if (!dev)
           dev = devlist; /* defaulting to first device */

       printf("UPnP device :\n"
              " desc: %s\n st: %s\n",
              dev->descURL, dev->st);

       descXML = (char*)miniwget(dev->descURL, &descXMLsize);
       if (descXML)
       {
           parserootdesc (descXML, descXMLsize, data);
           free (descXML); descXML = 0;
           GetUPNPUrls (urls, data, dev->descURL);
       }
       freeUPNPDevlist(devlist);
       return true;
   }
   return false;
}

bool
Portfwd::add( const std::string& ip, unsigned short port )
{
   char port_str[16];
   int r;
   printf("TB : upnp_add_redir (%s, %d)\n", ip.c_str(), port);
   if(urls->controlURL[0] == '\0')
   {
       printf("TB : the init was not done !\n");
       return false;
   }
   sprintf(port_str, "%d", port);
   r = UPNP_AddPortMapping(urls->controlURL, data->servicetype,
                           port_str, port_str, ip.c_str(), 0, "TCP", NULL);
   if(r!=0)
   {
    printf("AddPortMapping(%s, %s, %s) failed, code %d\n", port_str, port_str, ip.c_str(), r);
    return false;
   }
   return true;
}

bool
Portfwd::remove( unsigned short port )
{
   char port_str[16];
   printf("TB : upnp_rem_redir (%d)\n", port);
   if(urls->controlURL[0] == '\0')
   {
       printf("TB : the init was not done !\n");
       return false;
   }
   sprintf(port_str, "%d", port);
   int r = UPNP_DeletePortMapping(urls->controlURL, data->servicetype, port_str, "TCP", NULL);
   return r == 0;
}

std::string
Portfwd::external_ip()
{
    char ip[16];
    int r = UPNP_GetExternalIPAddress( urls->controlURL, data->servicetype, (char*)&ip );
    if( r != 0 ) return "";
    return std::string(ip);
}

