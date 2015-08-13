#include "SvnController.h"
#include <sstream>
#include <windows.h>

DLL_API ISvnController* getSvnController()
{
	//ISvnController* pSvnController = new CSvnController;
	//return pSvnController;
	return CSvnController::instance();
}

std::string CSvnController::log(const std::string& strURL)
{
	std::string _log("svn log ");
	//_log += "\"" + strURL + "\"";
	_log += "\"";
	_log += strURL;
	_log += "\"";

	return _log;
	//return dispatchToString(_log);
}

std::string CSvnController::log(const std::string& strURL, bool bXML, bool bVerbose, int nStartRev, int nEndRev)
{
	std::string _log;
	_log += log(strURL);
	_log += _revision(nStartRev, nEndRev);

	if (bXML)
		_log += _xml();

	if (bVerbose)
		_log += _verbose();

	return _log;
	//return dispatchToString(_log);
}

std::string CSvnController::log(const std::string& strURL, bool bXML, bool bVerbose, const std::vector<int>& vecRevisions)
{
	std::string _log;
	_log += log(strURL);

	for (unsigned int i = 0; i < vecRevisions.size(); ++i)
		_log += _revision(vecRevisions[i]);

	if (bXML)
		_log += _xml();

	if (bVerbose)
		_log += _verbose();

	return _log;
}

std::string CSvnController::log(const std::string& strURL, int nLimit, bool bXML, bool bVerbose)
{
	std::string _log;
	_log += log(strURL);
	_log += _limit(nLimit);

	if (bXML)
		_log += _xml();

	if (bVerbose)
		_log += _verbose();

	return _log;
	//return dispatchToString(_log);
}

std::string CSvnController::dispatchToString(const std::string& strLog)
{
	HANDLE hWrite, hRead;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;

	// 결과값 받을 버퍼 생성
	CreatePipe(&hRead, &hWrite, &sa, 0);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hWrite;
	si.hStdError = hWrite;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	// CreateProcess 2번째 인자는 비상수문자열이므로 c_str()로 전달할수없음
	char* MutableLog = new char[strLog.length() + 1];
	MutableLog = strcpy(MutableLog, strLog.c_str());

	// svn log 명령어 실행
	//CreateProcess(NULL, MutableLog, NULL, NULL, true, DETACHED_PROCESS, NULL, NULL, &si, &pi);
	if (!CreateProcess(NULL, MutableLog, NULL, NULL, true, DETACHED_PROCESS, NULL, NULL, &si, &pi))
	{
		CloseHandle(hWrite);
		CloseHandle(hRead);
		delete[] MutableLog;

		return std::string("");
	}

	//WaitForSingleObject(pi.hProcess, INFINITE);

	delete[] MutableLog;

	CloseHandle(hWrite);

	char strBuffer[1024 + 1] = "";
	std::string strResult;
	DWORD bytesRead;

	// 버퍼에서 결과값 읽어옴
	while (ReadFile(hRead, strBuffer, 1024, &bytesRead, NULL) && bytesRead)
	{
		strBuffer[bytesRead] = '\0';
		strResult += strBuffer;
	}

	CloseHandle(hRead);

	return strResult;
}

std::string CSvnController::_revision(int nStartRev, int nEndRev)
{
	std::stringstream _log;
	_log << " -r " << nStartRev;

	if (nEndRev != -1)
		_log << ":" << nEndRev;

	return _log.str();
}

std::string CSvnController::_limit(int nLimit)
{
	std::stringstream _log;
	_log << " -l " << nLimit;

	return _log.str();
}

std::string CSvnController::_xml()
{
	std::string _log(" --xml");

	return _log;
}

std::string CSvnController::_verbose()
{
	return std::string(" -v");
}