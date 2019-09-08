/*
 * Copyright (C) 2019 Xinyu Ma
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3.0. See the file LICENSE in the top level
 * directory for more details.
 *
 * See AUTHORS.md for complete list of NDN IOT PKG authors and contributors.
 */
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ndn-lite.h>
#include "ndn-lite/encode/name.h"
#include "ndn-lite/encode/data.h"
#include "ndn-lite/encode/interest.h"

ndn_name_t name_prefix;
uint8_t buf[4096];
ndn_udp_face_t *face;
bool running;

int parseArgs(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "ERROR: wrong arguments.\n");
    printf("Usage: <name-prefix>\n");
    return 1;
  }
  if(ndn_name_from_string(&name_prefix, argv[1], strlen(argv[1])) != NDN_SUCCESS){
    fprintf(stderr, "ERROR: wrong name.\n");
    return 4;
  }

  return 0;
}

int on_interest(const uint8_t* interest, uint32_t interest_size, void* userdata){
  ndn_data_t data;
  ndn_encoder_t encoder;
  char * str = "I'm a Data packet.";

  printf("On interest\n");
  data.name = name_prefix;

  ndn_data_set_content(&data, (uint8_t*)str, strlen(str) + 1);
  ndn_metainfo_init(&data.metainfo);
  
  ndn_metainfo_set_freshness_period(&data.metainfo, 1000);

  ndn_metainfo_set_content_type(&data.metainfo, NDN_CONTENT_TYPE_BLOB);
  encoder_init(&encoder, buf, 4096);
  ndn_data_tlv_encode_digest_sign(&encoder, &data);
  ndn_forwarder_put_data(encoder.output_value, encoder.offset);

  return 0;
}

int main(int argc, char *argv[]){
  int ret;
  ndn_encoder_t encoder;

  if((ret = parseArgs(argc, argv)) != 0){
    return ret;
  }

  ndn_lite_startup();
  in_port_t multicast_port =  htons((uint16_t) 56363);
  in_addr_t multicast_ip = inet_addr("224.0.23.170");
  face = ndn_udp_multicast_face_construct(INADDR_ANY, multicast_ip, multicast_port);

  // face = ndn_unix_face_construct(NDN_NFD_DEFAULT_ADDR, true);

  running = true;
  encoder_init(&encoder, buf, sizeof(buf));
  ndn_name_tlv_encode(&encoder, &name_prefix);

  ndn_forwarder_register_prefix(encoder.output_value, encoder.offset, on_interest, NULL);
  while(running){
    ndn_forwarder_process();
    usleep(10000);
  }

  ndn_face_destroy(&face->intf);

  return 0;
}