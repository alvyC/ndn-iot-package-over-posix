#include "ndn-lite/psync-fconsumer/full-consumer.h"
#include "../ndn-lite/face/dummy-face.h"

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ndn-lite.h>
#include "ndn-lite/encode/name.h"
#include "ndn-lite/encode/data.h"
#include "ndn-lite/encode/interest.h"
#include <time.h>
#include <stdlib.h>

void onUpdate() {;}

int main() {
bool running = true;
// full_consumer* c = NULL;
ndn_lite_startup();
srandom(time(0));
char* arr = "/sync";
ndn_udp_face_t *udp_face = NULL;
// time_t time;
void (*ptr)() = &onUpdate; 

in_port_t multicast_port =  htons((uint16_t) 56363);
in_addr_t multicast_ip = inet_addr("224.0.23.170");
udp_face = ndn_udp_multicast_face_construct(INADDR_ANY, multicast_ip, multicast_port);

init_full_consumer(arr, udp_face, &onUpdate);

while(running){
    ndn_forwarder_process();
    usleep(10000);
}

}

    