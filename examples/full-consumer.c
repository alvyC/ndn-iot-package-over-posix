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

full_consumer* c;
char const arr[] = "ABCD";
ndn_udp_face_t *udp_face;
time_t time;
// void (*ptr)() = &onUpdate; 

init_full_consumer(udp_face, arr, &onUpdate);

}

