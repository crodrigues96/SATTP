#include "sattp.hpp"
//----
std::string GetServerCfgOption(const std::string &option);

struct mg_server *WServer = NULL;
bool KillWebServer = false;
std::thread WebSvr;
std::ofstream debug;
//----
static int Event_Handler(struct mg_connection *conn, enum mg_event ev) {
	if (ev == MG_AUTH) {
		return MG_TRUE;   // Authorize all requests
	}
	else if (ev == MG_REQUEST) {
		std::ifstream WebFile;
		WebFile.open(std::string(conn->uri));

		if (WebFile.is_open() == false)
			return MG_FALSE; // The file wasn't found, show the directory listing(Can be configurated later)

		while (WebFile.eof() != true) {
			char buf[2];
			WebFile.read(buf, 2);
			mg_printf_data(conn, "%s", buf);
		}

		WebFile.close();
		return MG_TRUE;   // Mark as processed
	}
	else {
		return MG_FALSE;  // Rest of the events are not processed
	}
}

void FeedPoll() {
	WServer = mg_create_server(NULL, Event_Handler);
	// Set the document root
	mg_set_option(WServer, "document_root", "Webserver/");

	// Set the Web server port
	std::string http_port;
	http_port = GetServerCfgOption("http_port");
	if (http_port.length() > 0)
		mg_set_option(WServer, "listening_port", http_port.c_str());
	else
		mg_set_option(WServer, "listening_port", GetServerCfgOption("port").c_str());

	// Keep the server alive, until we get a signal, or the user simply shutdown the server with exit.
	for (; KillWebServer != true;)
		mg_poll_server(WServer, 1000);

	mg_destroy_server(&WServer);
}
//----
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
	sampgdk::Load(ppData);
	WebSvr = std::thread(FeedPoll); // Create our auxiliar thread for the web server
	return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supporst() {
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { // Called with exit command
	sampgdk::Unload();
	KillWebServer = true; // tell the loop that we want to kill the server.
	WebSvr.join(); // Wait for the thread to finish.
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	sampgdk::ProcessTick();
}

// Created by: https://github.com/Zeex/sampgdk/blob/master/plugins/unlimitedfs/unlimitedfs.cpp
std::string GetServerCfgOption(const std::string &option)
{
	std::string name, value;
	std::string line;
	std::fstream server_cfg("server.cfg");
	if (server_cfg)
	{
		while (std::getline(server_cfg, line))
		{
			std::stringstream ss(line);
			ss >> name;
			if (name == option)
			{
				ss >> value;
				break;
			}
		}
	}
	return value;
}