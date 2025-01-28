#include "Netzxz.h"


Netzxz::Netzxz(const char* ip, const uint16_t cport,const uint16_t rport)
{
    strcpy(this->uip,ip);
    this->uport = cport;
    this->refport = rport;
}


Netzxz::~Netzxz()
{

}


