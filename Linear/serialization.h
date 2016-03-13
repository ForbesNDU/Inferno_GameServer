#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "net_defs.h"
#include <string.h>

void serialize_match_request(match_request*, char*);
void deserialize_match_request(match_request*, char*);

#endif
