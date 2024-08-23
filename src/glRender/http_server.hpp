#pragma once
#ifdef WIN

// --------------------------------------------
// server
// --------------------------------------------
extern int servermain(int num);
namespace CMD {
	extern void onmsg(const std::string& msg, const std::string& body);
	extern void onmsg(const std::string& msg, const std::multimap<std::string, std::string>& params, std::string& out);
}
std::vector<std::pair<std::string, std::string> > msgbuffer;
std::mutex msgbuffer_mutex;
void onrequest(const std::string& msg, const std::string& body, std::string& out)
{
	PRINT("onrequest: " << msg << "\n");
	{
		std::lock_guard<std::mutex> lock(msgbuffer_mutex); // ¼ÓËø
		msgbuffer.push_back({ msg, body });

		out = "hello!";
	}
}
void onrequest_get(const std::string& msg, const std::multimap<std::string, std::string>& params, std::string& out)
{
	if (msg == "get")
	{
		CMD::onmsg(msg, params, out);
	}
}
#endif

#define SEND_MSG(ip, port, cmd, body)	{httplib::Client client(ip, port);client.Post(cmd, body, "text/plain");}