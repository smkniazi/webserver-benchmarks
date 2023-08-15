#include "httplib/httplib.h"

#define CPPHTTPLIB_THREAD_POOL_COUNT 10

using namespace httplib;

void work(int x) { std::this_thread::sleep_for(std::chrono::milliseconds(x)); }

int main(int argc, char** argv) {
  setbuf(stdout, NULL);

  int num_threads = 1;
  if (argc > 1) {
    num_threads = std::stoi(argv[1]);
  }

  Server svr;
  svr.Get("/ping", [](const Request& req, Response& res) {
    res.set_content("OK", "text/plain");
  });

  svr.Post("/ping", [](const Request& req, Response& res) {
    res.set_content("OK", "text/plain");
  });

  svr.Get(R"(/ping/delay/([0-9]*))", [&](const Request& req, Response& res) {
    std::string delay_str = req.matches[1];
    int time = 0;
    time = std::stoi(delay_str);
    work(time);
    res.set_content("OK", "text/plain");
  });

  svr.Get(R"(/ping/rand_delay/([0-9]*))",
          [&](const Request& req, Response& res) {
            std::string delayStr = req.matches[1];
            int time = 0;
            time = std::stoi(delayStr);
            if (time != 0) {
              time = rand() % time;
            }
            work(time);
            res.set_content("OK", "text/plain");
          });
  svr.listen("0.0.0.0", 4046);
}
