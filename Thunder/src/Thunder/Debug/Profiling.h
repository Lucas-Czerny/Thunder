#pragma once

namespace Thunder
{
	struct ProfileResult
	{
		ProfileResult(const char* name, long long start, long long end, float microseconds, std::thread::id threadID)
			: Name(name), Start(start), End(end), Microseconds(microseconds), ThreadID(threadID) {}

		const char* Name;
		long long Start, End;
		float Microseconds;
		std::thread::id ThreadID;
	};

	class Timer
	{
	private:
		const char* m_Name;
		long long m_StartTime;
		bool m_Stopped;

		std::function<void(ProfileResult)> m_Callback;

	public:
		Timer(const char* name, std::function<void(ProfileResult)> callback = NULL);
		~Timer();

		ProfileResult Stop();
	};

	struct InstrumentationSession
	{
		char* Name;
		char* Filepath;
		bool Empty = true;
	};

	class Instrumentor
	{
	private:
		static InstrumentationSession* s_CurrentSession;
		static std::ofstream s_OutputStream;

	public:
		static void BeginSession(char* name, char* filepath);
		static void EndSession();
		static void WriteProfile(const ProfileResult& result);

	private:
		static void WriteHeader();
		static void WriteFooter();
	};

	class InstrumentationTimer
	{
	private:
		Timer m_Timer;

	public:
		InstrumentationTimer(const char* name)
			: m_Timer(Timer(name, InstrumentationTimer::WriteProfile)) {}

	private:
		static void WriteProfile(const ProfileResult& result)
		{
			Instrumentor::WriteProfile(result);
		}
	};
}

#if TD_ENABLE_PROFILING
	#define TD_BEGIN_PROFILING_SESSION(name) Thunder::Instrumentor::BeginSession(name, name ".json")
	#define TD_END_PROFILING_SESSION() Thunder::Instrumentor::EndSession()
	#define TD_PROFILE_SCOPE(timerName) Thunder::InstrumentationTimer timer##__LINE__(timerName)
	#define TD_PROFILE_FUNCTION() Thunder::InstrumentationTimer timer##__LINE__(__FUNCTION__)
#else
	#define TD_BEGIN_PROFILING_SESSION(name)
	#define TD_END_PROFILING_SESSION()
	#define TD_PROFILE_SCOPE(timerName)
	#define TD_PROFILE_FUNCTION()
#endif
