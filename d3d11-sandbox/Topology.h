#pragma once
#include "Bindable.h"
#include "BindableCodex.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
			:
			type(type)
		{}
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const noexcept override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}