#ifndef NETZXZ_H_
#define NETZXZ_H_

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Netzxz
{
public:
  Netzxz(const char* ip, const uint16_t cport,const uint16_t rport);
  ~Netzxz();

  char uip[20];
  uint16_t uport;
  uint16_t refport;

};


#endif

