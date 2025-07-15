#pragma once

#include <cstdint>

namespace Juniper {

	class UUID
	{
	public:
		UUID();

		operator uint64_t() const { return  m_UUID; }

	private:
		uint64_t m_UUID;
	};

}

// Define hash function for UUIDs
namespace std {
	
	template<>
	struct hash<Juniper::UUID>
	{
		std::size_t operator()(const Juniper::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
}	;

}
