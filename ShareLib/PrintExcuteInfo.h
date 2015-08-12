#ifndef __PRINT_EXCUTE_INFO_H__
#define __PRINT_EXCUTE_INFO_H__

class PrintExcuteInfo
{
public:
	PrintExcuteInfo(void);
	~PrintExcuteInfo(void) {}

	static PrintExcuteInfo* Instance();

	void PrintStart(const std::string server_name, const std::string national);
	void PrintEnd();
	void SetSignalNo(int signo)
	{
		sig_no_ = signo;
	}
	void SetStopMessage(std::string msg)
	{
		stop_message_ = msg;
	}

private:
	void PrintEndBySignal();

private:
	std::string server_name_;
	std::string national_;
	std::string debug_str_;
	std::string test_str_;

	std::string stop_message_;

	int sig_no_;
};

#endif
