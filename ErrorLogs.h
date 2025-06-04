#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Logs {
public:
	enum LogsType {
		LERROR,
		WARNING,
		INFO
	};

	struct ErrorLog {
		ErrorLog(LogsType t, string s, int index) {
			type = t;
			msg = s;
			line = index;
		}

		LogsType type;
		string msg;
		int line;
	};

	void pushLog(ErrorLog log);

	void printLogs();

	bool haveLogs();

	bool haveError();

	void reset();

private:
	vector<ErrorLog> logs;

	bool error = false;
};