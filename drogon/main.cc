#include <drogon/drogon.h>
using namespace drogon;
using namespace drogon::orm;

void work(int x) { std::this_thread::sleep_for(std::chrono::milliseconds(x)); }

int main() {
  // `registerHandler()` adds a handler to the desired path. The handler is
  // responsible for generating a HTTP response upon an HTTP request being
  // sent to Drogon
  app().registerHandler(
      "/ping",
      [](const HttpRequestPtr &,
         std::function<void(const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("Hello, World!");
        callback(resp);
      },
      {Get});

  app().registerHandler(
      "/ping/delay/{time}",
      [](const HttpRequestPtr &,
         std::function<void(const HttpResponsePtr &)> &&callback,
         const std::string &time) {
        int t = std::stoi(time);
        work(t);
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("Hello, World!. With fixed delay");
        callback(resp);
      },
      {Get});

  app().registerHandler(
      "/ping/rand_delay/{time}",
      [](const HttpRequestPtr &,
         std::function<void(const HttpResponsePtr &)> &&callback,
         const std::string &time) {
        int t = std::stoi(time);
        if (t != 0) {
          t = rand() % t;
        }
        work(t);

        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("Hello, World!. With random delay");
        callback(resp);
      },
      {Get});

  printf("Server running on 0.0.0.0:4046\n");
  app().addListener("0.0.0.0", 4046);
  app().setThreadNum(16);
  app().disableSession();
  app().run();
}
