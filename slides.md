# Benchmarking Libs RDRS and Other Stuff 

 - Benchmarking Webersers
   - Go, Drogon, libhttp, Drogon (fastest accoring to techempower.com)
 - Benchmarking Json Libs 
   - Serialization & Deserialization (Hetrogeneus json arrays)
   - *Go* json, simdjson 
   - *CPP* simdjson, glaze
   - *C* yyjson
 - Other Stuff 
   - Unit testing, Prometheus, C++ Standard 

  [https://github.com/smkniazi/webserver-benchmarks]

---

# Testing Env 

 CPU: 12th Gen Intel(R) Core(TM) i9-12900HK

 Number of threads for server 16, pinned to CPUs 0-15 (12 p-cores, 4 e-cores)

 Client wrk.  40 threads using  80 connections pinned to CPUs 16-19 (4 e-cores)


---

# Sample Request

```lua
wrk.method = "POST"
wrk.body   = [[
{
  "operations": [
    {
      "method": "POST", "relative-url": "my_database_1/my_table_1/pk-read",
      "body": {
        "filters": [
          { "column": "id0", "value": "0" },
          { "column": "id1", "value": "0" }
        ],
        "readColumns": [
          { "column": "col0", "dataReturnType": "default" },
          { "column": "col1", "dataReturnType": "default" }
        ],
        "operationId": "1"
      }
    }
  ]
}
]]
wrk.headers["Content-Type"] = "application/json"
```

```bash
wrk -s req.lua -c 80 -t 40 -d 15s http://localhost:4046/ping

```

---

# Max Throughput 

                *GO*                                                              *C++*

```bash
Running 15s test @ http://localhost:4046/ping                   |   Running 15s test @ http://localhost:4046/ping
  40 threads and 80 connections                                 |     40 threads and 80 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev             |     Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   247.78us  349.98us   7.12ms   88.03%              |       Latency   162.98us  270.68us  20.21ms   98.77%
    Req/Sec    13.89k     1.98k   32.45k    74.16%              |       Req/Sec    13.36k     3.57k   31.56k    79.12%
  Latency Distribution                                          |     Latency Distribution
     50%  105.00us                                              |        50%  140.00us
     75%  286.00us                                              |        75%  183.00us
     90%  673.00us                                              |        90%  247.00us
     99%    1.71ms                                              |        99%  463.00us
  8319208 requests in 15.10s, 0.91GB read                       |     8027674 requests in 15.10s, 1.13GB read
Req/sec: 550965.05                                              |   Req/sec: 531620.80
Transfer/sec:     62.00MB                                       |   Transfer/sec:     76.56MB

```

---

# Libhttp, boost beast

Slow, no need to further test them

---

# Batch Size 1  (Deserialization)

|            | GO, (go-gin + json)   | C++ (Drogon + simdjson)   | GO (go-gin + gosimdjson)   | C++ (Drogon + glaze)   | c++ (Drogon + yyjson)   |
| ---------- | --------------------- | ------------------------- | -------------------------- | ---------------------- | ----------------------- |
| 50%        | 278.00us              | 195.00us                  | 550.00us                   | 179.00us               | 223.00us                |
| 75%        | 1.39ms                | 288.00us                  | 2.54ms                     | 322.00us               | 345.00us                |
| 90%        | 2.94ms                | 397.00us                  | 3.95ms                     | 594.00us               | 482.00us                |
| 99%        | 5.88ms                | 764.00us                  | 8.40ms                     | 1.67ms                 | 0.94ms                  |
| Req/sec:   | Req/sec: 190965.27    | Req/sec: 363035.15        | Req/sec:  91288.70         | Req/sec: 354139.56     | Req/sec: 311419.29      |

---

# Batch Size 10  (Deserialization)


|            | GO, (go-gin + json)   | C++ (Drogon + simdjson)   | GO (go-gin + gosimdjson)   | C++ (Drogon + glaze)   | c++ (Drogon + yyjson)   |
| ---------- | --------------------- | ------------------------- | -------------------------- | ---------------------- | ----------------------- |
| 50%        | 809.00us              | 225.00us                  | 849.00us                   | 272.00us               | 603.00us                |
| 75%        | 4.33ms                | 289.00us                  | 3.30ms                     | 380.00us               | 0.94ms                  |
| 90%        | 7.16ms                | 369.00us                  | 4.88ms                     | 502.00us               | 1.55ms                  |
| 99%        | 14.79ms               | 652.00us                  | 10.09ms                    | 4.45ms                 | 6.91ms                  |
| Req/sec:   | Req/sec:  62428.45    | Req/sec: 335603.99        | Req/sec:  60657.29         | Req/sec: 262935.10     | Req/sec: 114909.33      |


---

# Batch Size 10  (Deserialization and Serialization)

|              | Go (json)          | c++ simd + simple serializer | c++ glaze for serialization and deserialization |
| ------------ | ------------------ | ---------------------------- | ----------------------------------------------- |
| 50%          | 1.21ms             | 218.00us                     | 314.00us                                        |
| 75%          | 5.17ms             | 302.00us                     | 433.00us                                        |
| 90%          | 10.27ms            | 394.00us                     | 673.00us                                        |
| 99%          | 22.89ms            | 710.00us                     | 1.82ms                                          |
| Req/sec:     | Req/sec:  46139.38 | Req/sec: 330787.47           | Req/sec: 232537.81                              |

---

# More ...  

Further investigations needed

 - Serialization & Deserialization (Hetrogeneus json arrays)
   - how to deal with *nil*
 - Other Stuff 
   - Unit testing, Prometheus, C++ Standard 
