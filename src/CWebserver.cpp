#include "CWebserver.hpp"
//---
CWebserver *CWebserver::Instance = NULL;
//--- Adding the GetServerCFGOption into SAMPGDK since it was made by Zeex.
namespace sampgdk {
	std::string GetServerCFGOption(const std::string &option);
}
//---

void CWebserver::Load() {
	CWebserver::Instance = new CWebserver();
}

void CWebserver::Unload() {
	CWebserver::Instance->~CWebserver();
}

CWebserver::CWebserver() {
	WebThread = std::thread((std::bind(&CWebserver::FeedPoll, this)));
}

CWebserver::~CWebserver() {
	KeepExecution = false;
	WebThread.join();
}

void CWebserver::FeedPoll() {
	WServer = mg_create_server(NULL, &CWebserver::HandleRequests);
	// Set the document root
	mg_set_option(WServer, "document_root", "Webserver/");

	// Set the Web server port
	std::string http_port;
	http_port = sampgdk::GetServerCFGOption("http_port");
	if (http_port.length() > 0)
		mg_set_option(WServer, "listening_port", http_port.c_str());
	else
		mg_set_option(WServer, "listening_port", sampgdk::GetServerCFGOption("port").c_str());

	// Output the message that the server is alive... will be removed in stable and official release.
	sampgdk::logprintf(" **** WEB SERVER INITIALIZED IN THE AUXILIAR THREAD ****");
	sampgdk::logprintf(" **** LISTENING FOR CONNECTIONS ON PORT: %s ****", mg_get_option(WServer, "listening_port"));
	// Keep the server alive, until we get a signal, or the user simply shutdown the server with exit.
	for (; this->KeepExecution == true;)
		mg_poll_server(WServer, 1000);

	mg_destroy_server(&WServer);
}

int CWebserver::HandleRequests(struct mg_connection *conn, enum mg_event ev) {
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

//----
// Adding the GetServerCFGOption into SAMPGDK since it was made by Zeex.
//----
namespace sampgdk {
	std::string GetServerCFGOption(const std::string &option)
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
}