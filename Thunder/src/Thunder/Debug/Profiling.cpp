#include "td_pch.h"
#include "Profiling.h"

#include "Thunder/Core/Log.h"

namespace Thunder
{
	Timer::~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	Timer::Timer(const char* name, std::function<void(ProfileResult)> callback /* = NULL */)
		: m_Name(name), m_Stopped(false), m_Callback(callback)
	{
		m_StartTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}

	ProfileResult Timer::Stop()
	{
		m_Stopped = true;

		long long endTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();

		float microseconds = endTime - m_StartTime;

		ProfileResult result(m_Name, m_StartTime, endTime, microseconds, std::this_thread::get_id());

		if (m_Callback)
			m_Callback(result);

		return result;
	}

	InstrumentationSession* Instrumentor::s_CurrentSession = NULL;
	std::ofstream Instrumentor::s_OutputStream;

	void Instrumentor::BeginSession(char* name, char* filepath)
	{
		TD_CORE_ASSERT(s_CurrentSession == NULL, "Can't create a new Instrumentation session as there is another session running!");
		s_CurrentSession = new InstrumentationSession();
		s_CurrentSession->Name = name;
		s_CurrentSession->Filepath = filepath;

		s_OutputStream.open(filepath);

		WriteHeader();
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();

		s_CurrentSession = NULL;
		s_OutputStream.close();
	}

	void Instrumentor::WriteHeader()
	{
		s_OutputStream << "{ \"traceEvents\": [ ";
		s_OutputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		s_OutputStream << " ] }";
		s_OutputStream.flush();
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		if (s_CurrentSession == NULL)
			return;

		if (!s_CurrentSession->Empty)
			s_OutputStream << ", ";
		else
			s_CurrentSession->Empty = false;

		s_OutputStream << "{ \"name\": \"" << result.Name;
		s_OutputStream << "\", \"cat\": \"function\",";
		s_OutputStream << "\"dur\":" << result.Microseconds << ',';
		s_OutputStream << "\"ph\":\"X\",";
		s_OutputStream << "\"pid\":0,";
		s_OutputStream << "\"tid\":" << result.ThreadID << ",";
		s_OutputStream << "\"ts\":" << result.Start;
		s_OutputStream << "}";

		s_OutputStream.flush();
	}
}
