#include "myglaze.h"
#include <cstdio>
#include <glaze/glaze.hpp>
#include <glaze/json/read.hpp>
#include "data-structs.h"


void glazeParse(BatchOpRequest *req) {
  string test_data = R"(
{
  "operations": [
    {
      "method": "POST",
      "relative-url": "my_database_1/my_table_1/pk-read",
      "body": {
        "filters": [
          {
            "column": "id0",
            "value": "0"
          },
          {
            "column": "id1",
            "value": "0"
          }
        ],
        "readColumns": [
          {
            "column": "col0",
            "dataReturnType": "default"
          },
          {
            "column": "col1",
            "dataReturnType": "default"
          }
        ],
        "operationId": "1"
      }
    },
    {
      "method": "POST",
      "relative-url": "my_database_1/my_table_1/pk-read",
      "body": {
        "filters": [
          {
            "column": "id0",
            "value": "0"
          },
          {
            "column": "id1",
            "value": "0"
          }
        ],
        "readColumns": [
          {
            "column": "col0",
            "dataReturnType": "default"
          },
          {
            "column": "col1",
            "dataReturnType": "default"
          }
        ],
        "operationId": "1"
      }
    }
  ]
}
  )";

  BatchOpRequest batchOpRequest{};
  auto s = glz::read_json<BatchOpRequest>(test_data);
  if (s) {
    BatchOpRequest batchOpRequest = s.value();
    printf("Size is %ld\n", batchOpRequest.Operations.size());
    printf("Method [0] %s\n", batchOpRequest.Operations[0].Method.c_str());
    printf("RURL [0] %s\n", batchOpRequest.Operations[0].RelativeURL.c_str());
    printf("Body [0].Fileter[0].Column %s\n", batchOpRequest.Operations[0].Body.Filters[0].Column.c_str());
    printf("Body [0].Fileter[0].Column %s\n", batchOpRequest.Operations[0].Body.Filters[0].Value.c_str());
    printf("Body [0].ReadColumns[0].Column %s\n", batchOpRequest.Operations[0].Body.ReadColumns[0].Column.c_str());
    printf("Body [0].ReadColumns[0].DataReturnType %s\n", batchOpRequest.Operations[0].Body.ReadColumns[0].DataReturnType.c_str());
    printf("Body [0].OperationID %s\n", batchOpRequest.Operations[0].Body.OperationID.c_str());
  }
}

