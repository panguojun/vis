//
//  server.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//
#include <chrono>
#include <cstdio>
#include "../.thirdpart/httplib.hpp"
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace httplib;
std::string GetLocalIPAddress()
{
    std::string localIP;
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        std::cerr << "Error allocating memory needed to call GetAdaptersinfo" << std::endl;
        return "";
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            std::cerr << "Error allocating memory needed to call GetAdaptersinfo" << std::endl;
            return "";
        }
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            if (pAdapter->Type == 71 // MIB_IF_TYPE_ETHERNET 
                && pAdapter->IpAddressList.IpAddress.String[0] != '0') {
                localIP = pAdapter->IpAddressList.IpAddress.String;
                break;
            }
            pAdapter = pAdapter->Next;
        }
    }

    if (pAdapterInfo)
        free(pAdapterInfo);

    return localIP;
}

std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

std::string log(const Request &req, const Response &res) {
   // return ""; // πÿ±’»’÷æ£°
  std::string s;
  char buf[BUFSIZ];

  s += "\n================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) { s += res.body; }

  s += "\n";

  return s;
}
extern std::string   HOSTIP;
extern void onrequest(const std::string& msg, const std::string& body, std::string& out); 
extern void onrequest_get(const std::string& msg, const std::multimap<std::string, std::string>& params, std::string& out);

int servermain(int num) {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
  Server svr;
#endif

  if (!svr.is_valid()) {
    printf("server has an error...\n");
    return -1;
  }

  svr.Get("/", [=](const Request & /*req*/, Response &res) {
    res.set_redirect("/hi");
  });

  svr.Get("/hi", [](const Request & req, Response &res) {
      std::string response;
      onrequest("hi", req.body, response);
      res.set_content(response, "text/plain");
  });
  svr.Get("/get", [](const Request& req, Response& res) {
      std::string response;
      onrequest_get("get", req.params, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/cmd", [](const Request& req, Response& res) {
      std::string response;
      
      onrequest("cmd", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/cmd2d", [](const Request& req, Response& res) {
      std::string response;

      onrequest("cmd2d", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/cmd3d", [](const Request& req, Response& res) {
      std::string response;

      onrequest("cmd3d", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/ui", [](const Request& req, Response& res) {
      std::string response;

      onrequest("ui", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/lua", [](const Request& req, Response& res) {
      std::string response;

      onrequest("lua", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/phg", [](const Request& req, Response& res) {
      std::string response;

      onrequest("phg", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/hlr", [](const Request& req, Response& res) {
      std::string response;

      onrequest("hlr", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/sm", [](const Request& req, Response& res) {
      std::string response;

      onrequest("sm", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Get("/slow", [](const Request & /*req*/, Response &res) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Slow...\n", "text/plain");
  });

  svr.Get("/dump", [](const Request &req, Response &res) {
    res.set_content(dump_headers(req.headers), "text/plain");
  });

  svr.Get("/stop",
          [&](const Request & /*req*/, Response & /*res*/) { svr.stop(); });

  svr.set_error_handler([](const Request & /*req*/, Response &res) {
    const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });

  svr.set_logger([](const Request &req, const Response &res) {
    printf("%s", log(req, res).c_str());
  });

  svr.listen(HOSTIP.c_str(), 5088);
  //svr.listen(GetLocalIPAddress().c_str(), 5088);
  //svr.listen("127.0.0.1", 5088);
  return 0;
}
