#pragma once
#include "sattp.hpp"

class CWebserver {
public:
	//----
	static CWebserver *Instance;
	static void Load();
	static void Unload();
	//----
	struct mg_server *WServer = NULL;
	std::thread WebThread;
	bool KeepExecution = true;
	//----
	CWebserver();
	~CWebserver();
	//----
	void InitThread();
	void StopThread();
	void FeedPoll();
private:
	static int HandleRequests(struct mg_connection *conn, enum mg_event ev);
};