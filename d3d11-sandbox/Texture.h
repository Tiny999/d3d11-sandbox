#pragma once
#include "Bindable.h"
#include "BindableCodex.h"

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, UINT slot = 0);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Bindable> Resolve(Graphics& gfx, const std::string& path, UINT slot);
		static std::string GenerateUID(const std::string& path, UINT slot);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	private:
		unsigned int slot;
		std::string path;
	};
}