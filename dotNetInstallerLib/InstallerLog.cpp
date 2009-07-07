#include "StdAfx.h"
#include "InstallerLog.h"

InstallerLog ApplicationLog;

InstallerLog::InstallerLog(void)
    : m_enabled(false)
    , m_hFile(INVALID_HANDLE_VALUE)
{

}

InstallerLog::~InstallerLog(void)
{
    if (m_hFile != NULL)
    {
        ::CloseHandle(m_hFile);
    }
}

void InstallerLog::Write(const std::wstring& message)
{
	if (IsEnableLog() == false || message.empty())
		return;

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        if (m_logfile.empty()) 
        {
			m_logfile = DVLib::DirectoryCombine(DVLib::GetTemporaryDirectoryW(), L"dotNetInstallerLog.txt").c_str();
        }

		std::wstring path = DVLib::GetFileDirectoryW(m_logfile);
		DVLib::DirectoryCreate(path);

		m_hFile = ::CreateFile(m_logfile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CHECK_WIN32_BOOL(m_hFile != INVALID_HANDLE_VALUE,
			L"Error creating or opening \"" << m_logfile << L"\"");

	    SetFilePointer(m_hFile, 0, 0, FILE_END);
    }

	DWORD l_BytesWritten = 0;

	std::wstringstream message_s;
	message_s << DVLib::FormatCurrentDateTimeW() << L"\t" << message << std::endl;

	CHECK_WIN32_BOOL(WriteFile(m_hFile, message_s.str().c_str(), message_s.str().size(),  & l_BytesWritten, NULL),
		L"Error writing to \"" << m_logfile << L"\"");
}

void InstallerLog::Write(const std::wstring& message, const std::wstring& appendMessage)
{
	Write(message + appendMessage);
}