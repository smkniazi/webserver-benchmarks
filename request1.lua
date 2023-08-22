-- Post requestr 

wrk.method = "POST"
wrk.body   = [[
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
    }
  ]
}
]]

wrk.headers["Content-Type"] = "application/json"

