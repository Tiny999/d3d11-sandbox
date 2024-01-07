#include "PixelShader.h"
#include <d3dcompiler.h>

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::string& path)
		:
		path(path)
	{
		HRESULT hr;

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		GFX_THROW_FAILED(D3DReadFileToBlob(std::wstring{ path.begin(), path.end() }.c_str(), &pBlob));

		GFX_THROW_FAILED(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<PixelShader>(gfx, path);
	}
	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}