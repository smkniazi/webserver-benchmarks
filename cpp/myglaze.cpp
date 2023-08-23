#include "myglaze.h"
#include <cstdio>
#include <iostream>
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
    printf("Body [0].Fileter[0].Column %s\n",
           batchOpRequest.Operations[0].Body.Filters[0].Column.c_str());
    printf("Body [0].Fileter[0].Column %s\n",
           batchOpRequest.Operations[0].Body.Filters[0].Value.c_str());
    printf("Body [0].ReadColumns[0].Column %s\n",
           batchOpRequest.Operations[0].Body.ReadColumns[0].Column.c_str());
    printf("Body [0].ReadColumns[0].DataReturnType %s\n",
           batchOpRequest.Operations[0]
               .Body.ReadColumns[0]
               .DataReturnType.c_str());
    printf("Body [0].OperationID %s\n",
           batchOpRequest.Operations[0].Body.OperationID.c_str());
  }
}

void glaze_to_string(BatchOpRequest *req) {
  std::string buffer = glz::write_json(req);
  // printf("output %s\n", buffer.c_str());
}

void create_dummy_batch_req(BatchOpRequest *req, int num_ops) {
  for (int i = 0; i < num_ops; i++) {
    BatchSubOp subOp{};
    subOp.Method = "POST";
    subOp.RelativeURL = "/testing/test";
    subOp.Body.OperationID = "opid";
    Filter filter;
    filter.Column = "filter_col";
    filter.Value = "filter_val";
    ReadColumn read_col;
    read_col.Column = "read_col";
    read_col.DataReturnType = "default";
    subOp.Body.Filters.push_back(filter);
    subOp.Body.ReadColumns.push_back(read_col);
    req->Operations.push_back(subOp);
  }
}

void simple_to_string(BatchOpRequest *req) {
  std::stringstream ss;
  ss << "{'operations':[ ";
  for (int i = 0; i < req->Operations.size(); i++) {
    BatchSubOp op = req->Operations[i];
    ss << "{ 'method': '" << op.Method<< "', ";
    ss << "'relative_url': '" << op.RelativeURL<<"', ";
    ss << "'body': {";

    ss << "'filters': [";
    for (int f = 0; f < op.Body.Filters.size(); f++) {
      Filter filter = op.Body.Filters[f];

      ss << "{'column': '" << filter.Column << "', 'value': '" << filter.Value << "'" << "},";
    }
    ss << "],";

    ss << "'readCoumns': [";
    for (int f = 0; f < op.Body.Filters.size(); f++) {
      ReadColumn rc = op.Body.ReadColumns[f];

      ss << "{'column': '" << rc.Column << "', 'dataType': " << rc.DataReturnType << "'  },";
    }
    ss << "],";

    ss<< "'operationid': '"<< op.Body.OperationID << "'";


    ss << "}}, ";
  }
  ss << "]} ";

  // std::cout<<ss.str()<<std::endl;
}
