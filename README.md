# Testing web server performance written in Go / C++ 

The aim of these tests is to find which language and libraries will deliver heighet throughput and lowest latency. 

 CPU: 12th Gen Intel(R) Core(TM) i9-12900HK

 Number of threads for server 16

 server threads were pinned to CPUs 0-15

 wrk threads (40 threads, that shared 80 connections) were pinned to CPUs 16-19




```
#,              GO, (go-gin + json)                                                  C++ (Drogon + simdjson)                                        GO (go-gin + gosimdjson)                                        C++ (Drogon + glaze)                          c++ (Drogon + yyjson) 
                                                      PING

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
Requests/sec: 550965.05                                         |   Requests/sec: 531620.80
Transfer/sec:     62.00MB                                       |   Transfer/sec:     76.56MB



                                                      DB Operation
                                                      Body Size. Number of sub-operations: 1

Running 15s test @ http://localhost:4046/dbop/some_db/some_table |  Running 15s test @ http://localhost:4046/dbop/some_db/some_table  |   Running 15s test @ http://localhost:4046/dbopsimd/some_db/some_table |  Running 15s test @ http://localhost:4046/dbopglaze/some_db/some_table   |   Running 15s test @ http://localhost:4046/dbopyyjson/some_db/some_table
  40 threads and 80 connections                                  |    40 threads and 80 connections                                   |     40 threads and 80 connections                                      |    40 threads and 80 connections                                         |     40 threads and 80 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev              |    Thread Stats   Avg      Stdev     Max   +/- Stdev               |     Thread Stats   Avg      Stdev     Max   +/- Stdev                  |    Thread Stats   Avg      Stdev     Max   +/- Stdev                     |     Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     0.97ms    1.39ms  33.72ms   84.90%               |      Latency   238.18us  292.12us  21.99ms   96.07%                |       Latency     1.54ms    1.96ms  49.31ms   86.40%                   |      Latency   314.80us  709.12us  33.56ms   97.80%                      |       Latency   273.66us  187.40us   6.11ms   83.06%
    Req/Sec     4.80k   516.32     6.94k    70.40%               |      Req/Sec     9.12k     3.47k   29.55k    76.57%                |       Req/Sec     2.31k   329.47     5.72k    74.73%                   |      Req/Sec     8.93k     3.61k   28.34k    71.60%                      |       Req/Sec     7.82k     1.83k   17.81k    75.98%
  Latency Distribution                                           |    Latency Distribution                                            |     Latency Distribution                                               |    Latency Distribution                                                  |     Latency Distribution
     50%  278.00us                                               |       50%  195.00us                                                |        50%  550.00us                                                   |       50%  179.00us                                                      |        50%  223.00us
     75%    1.39ms                                               |       75%  288.00us                                                |        75%    2.54ms                                                   |       75%  322.00us                                                      |        75%  345.00us
     90%    2.94ms                                               |       90%  397.00us                                                |        90%    3.95ms                                                   |       90%  594.00us                                                      |        90%  482.00us
     99%    5.88ms                                               |       99%  764.00us                                                |        99%    8.40ms                                                   |       99%    1.67ms                                                      |        99%    0.94ms
  2883520 requests in 15.10s, 324.49MB read                      |    5481610 requests in 15.10s, 726.65MB read                       |     1378464 requests in 15.10s, 155.12MB read                          |    5347445 requests in 15.10s, 708.86MB read                             |     4702388 requests in 15.10s, 623.35MB read
Requests/sec: 190965.27                                          |  Requests/sec: 363035.15                                           |   Requests/sec:  91288.70                                              |  Requests/sec: 354139.56                                                 |   Requests/sec: 311419.29
Transfer/sec:     21.49MB                                        |  Transfer/sec:     48.12MB                                         |   Transfer/sec:     10.27MB                                            |  Transfer/sec:     46.94MB                                               |   Transfer/sec:     41.28MB



                                                      DB Operation
                                                      Body Size. Number of sub-operations: 10

Running 15s test @ http://localhost:4046/dbop/some_db/some_table  |  Running 15s test @ http://localhost:4046/dbop/some_db/some_table | Running 15s test @ http://localhost:4046/dbopsimd/some_db/some_table   |     Running 15s test @ http://localhost:4046/dbopglaze/some_db/some_table | Running 15s test @ http://localhost:4046/dbopyyjson/some_db/some_table
  40 threads and 80 connections                                   |    40 threads and 80 connections                                  |   40 threads and 80 connections                                        |       40 threads and 80 connections                                       |   40 threads and 80 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev               |    Thread Stats   Avg      Stdev     Max   +/- Stdev              |     Thread Stats   Avg      Stdev     Max   +/- Stdev                  |       Thread Stats   Avg      Stdev     Max   +/- Stdev                   |   Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.61ms    3.40ms  49.95ms   84.27%                |      Latency   257.59us  389.75us  23.08ms   98.97%               |     Latency     2.00ms    2.34ms  36.48ms   86.73%                     |         Latency   400.20us    0.91ms  25.36ms   98.34%                    |     Latency     0.88ms    1.14ms  27.06ms   94.65%
    Req/Sec     1.57k   228.18     2.57k    68.64%                |      Req/Sec     8.43k     1.95k   20.65k    73.77%               |     Req/Sec     1.53k   215.54     2.49k    70.39%                     |         Req/Sec     6.63k     2.08k   26.55k    73.69%                    |     Req/Sec     2.89k     1.26k    8.56k    71.03%
  Latency Distribution                                            |    Latency Distribution                                           |   Latency Distribution                                                 |       Latency Distribution                                                |   Latency Distribution
     50%  809.00us                                                |       50%  225.00us                                               |      50%  849.00us                                                     |          50%  272.00us                                                    |      50%  603.00us
     75%    4.33ms                                                |       75%  289.00us                                               |      75%    3.30ms                                                     |          75%  380.00us                                                    |      75%    0.94ms
     90%    7.16ms                                                |       90%  369.00us                                               |      90%    4.88ms                                                     |          90%  502.00us                                                    |      90%    1.55ms
     99%   14.79ms                                                |       99%  652.00us                                               |      99%   10.09ms                                                     |          99%    4.45ms                                                    |      99%    6.91ms
  942597 requests in 15.10s, 106.07MB read                        |    5067327 requests in 15.10s, 671.73MB read                      |   915886 requests in 15.10s, 103.07MB read                             |       3970250 requests in 15.10s, 526.30MB read                           |   1735069 requests in 15.10s, 230.00MB read
Requests/sec:  62428.45                                           |  Requests/sec: 335603.99                                          | Requests/sec:  60657.29                                                |     Requests/sec: 262935.10                                               | Requests/sec: 114909.33
Transfer/sec:      7.03MB                                         |  Transfer/sec:     44.49MB                                        | Transfer/sec:      6.83MB                                              |     Transfer/sec:     34.85MB                                             | Transfer/sec:     15.23MB
```



























