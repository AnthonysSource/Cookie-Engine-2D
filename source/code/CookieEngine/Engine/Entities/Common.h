#pragma once

namespace Cookie {

	// We should use generational id's because this approach
	// could generate problems when destroying and creating a
	// vast amount of entitites in a short timespan
	using EntityID = u32;
	using ComponentSignatureIndex = u8;

	const EntityID MAX_ENTITIES = 100000;
	const ComponentSignatureIndex MAX_COMPONENTS = 128;

	using Signature = Bitset<MAX_COMPONENTS>;

} // namespace Cookie