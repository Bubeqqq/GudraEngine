#include "ErrorLogs.h"

void Logs::pushLog(ErrorLog log)
{
	logs.push_back(log);
}

void Logs::printLogs()
{
	for (auto log : logs) {
		switch (log.type) {
		case LERROR:
			cout << "\033[31m" << "ERROR: ";
			error = true;
			break;
		case WARNING:
			cout << "\033[33m" << "WARNING: ";
			break;
		case INFO:
			cout << "\033[34m" << "INFO: ";
			break;
		}

		cout << "[in line: " << log.line + 1 << "] - " << log.msg << "\033[0m" << endl;
	}
}

bool Logs::haveLogs()
{
	return logs.size() >= 1;
}

bool Logs::haveError()
{
	return error;
}

void Logs::reset()
{
	logs.clear();
	error = false;
}
