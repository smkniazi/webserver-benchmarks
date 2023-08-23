#include "myyyjson.h"
#include <stdio.h>
#include <string>
#include "data-structs.h"
#include "yyjson.h"
using namespace std;
void test_yyjson() {
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
  printf("Testing yyjson\n");

  BatchOpRequest req{};
  parse_batch_yyjson(test_data.c_str(), &req);
}

int parse_filters(BatchSubOp *subOperation, size_t index, yyjson_val *filter) {
  Filter f = Filter{};
  yyjson_val *column = yyjson_obj_get(filter, "column");
  f.Column = string(yyjson_get_str(column));

  yyjson_val *value = yyjson_obj_get(filter, "value");
  f.Value = string(yyjson_get_str(value));

  subOperation->Body.Filters.push_back(f);
  return 0;
}

int parse_read_columns(BatchSubOp *subOperation, size_t index,
                       yyjson_val *read_column) {
  ReadColumn r = ReadColumn{};
  yyjson_val *column = yyjson_obj_get(read_column, "column");
  r.Column = string(yyjson_get_str(column));

  yyjson_val *data_return_type = yyjson_obj_get(read_column, "dataReturnType");
  r.DataReturnType = string(yyjson_get_str(data_return_type));

  subOperation->Body.ReadColumns.push_back(r);

  return 0;
}

int parse_operation(BatchOpRequest *batchOpRequest, size_t index,
                    yyjson_val *operation) {
  BatchSubOp batchSubOp{};

  yyjson_val *method = yyjson_obj_get(operation, "method");
  batchSubOp.Method = string(yyjson_get_str(method));

  yyjson_val *rurl = yyjson_obj_get(operation, "relative-url");
  batchSubOp.RelativeURL = string(yyjson_get_str(rurl));

  yyjson_val *body = yyjson_obj_get(operation, "body");

  yyjson_val *filters = yyjson_obj_get(body, "filters");
  size_t idx, max = 0;
  yyjson_val *filter;
  yyjson_arr_foreach(filters, idx, max, filter) {
    parse_filters(&batchSubOp, idx, filter);
  }

  yyjson_val *read_columns = yyjson_obj_get(body, "readColumns");
  idx = 0;
  max = 0;
  yyjson_val *read_column;
  yyjson_arr_foreach(read_columns, idx, max, read_column) {
    parse_read_columns(&batchSubOp, idx, read_column);
  }

  yyjson_val *operationId = yyjson_obj_get(body, "operationId");
  batchSubOp.Body.OperationID = string(yyjson_get_str(operationId));

  batchOpRequest->Operations.push_back(batchSubOp);

  return 0;
}

void print_batch_request(BatchOpRequest *req) {
  printf("Operations: %ld\n", req->Operations.size());
  for (int i = 0; i < req->Operations.size(); i++) {
    printf("\nOperation %d \n", i);
    printf("Method %s \n", req->Operations[i].Method.c_str());
    printf("RURL %s \n", req->Operations[i].RelativeURL.c_str());

    for (int j = 0; j < req->Operations[i].Body.Filters.size(); j++) {
      printf(" Filter Column %s \n",
             req->Operations[i].Body.Filters[j].Column.c_str());
      printf(" Filter Value %s \n",
             req->Operations[i].Body.Filters[j].Column.c_str());
    }

    printf("Read cols size %d\n", req->Operations[i].Body.ReadColumns.size());

    for (int j = 0; j < req->Operations[i].Body.ReadColumns.size(); j++) {
      printf(" Read Cols Column %s \n",
             req->Operations[i].Body.ReadColumns[j].Column.c_str());
      printf(" Read Cols Type %s \n",
             req->Operations[i].Body.ReadColumns[j].DataReturnType.c_str());
    }
    printf("OP ID %s \n", req->Operations[i].Body.OperationID.c_str());
  }
}

void parse_batch_yyjson(const char *body,BatchOpRequest  *batchOpRequest ) {
  yyjson_doc *doc = yyjson_read(body, strlen(body), 0);
  yyjson_val *root = yyjson_doc_get_root(doc);

  yyjson_val *operations = yyjson_obj_get(root, "operations");
  size_t idx, max;
  yyjson_val *operation;
  yyjson_arr_foreach(operations, idx, max, operation) {
    parse_operation(batchOpRequest, idx, operation);
  }
  // print_batch_request(&batchOpRequest);
}

void to_json_yyjson(const BatchOpRequest* req) {
yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
yyjson_mut_val *root = yyjson_mut_obj(doc);
yyjson_mut_doc_set_root(doc, root);
}
