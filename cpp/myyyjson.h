#ifndef MYYYJSON
#define MYYYJSON

#include "data-structs.h"
void test_yyjson();
void parse_batch_yyjson(const char* body, BatchOpRequest* req );
void to_json_yyjson(const BatchOpRequest* req);
#endif
