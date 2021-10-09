#pragma once

namespace Thunder
{
	struct GUID
	{
	private:
		uint64_t m_GUID;

	public:
		GUID() = default;
		GUID(uint64_t guid)
			: m_GUID(guid) {}

		static GUID Generate();

		operator uint64_t() const { return m_GUID; }
	};
}

namespace std
{
	template<>
	struct hash<Thunder::GUID>
	{
		std::size_t operator()(const Thunder::GUID& guid) const
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}