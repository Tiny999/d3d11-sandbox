#include "Topology.h"

namespace Bind
{
	void Topology::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetPrimitiveTopology(type);
	}
}