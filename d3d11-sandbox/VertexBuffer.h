#pragma once
#include "Bindable.h"
#include "BindableCodex.h"
#include <vector>
#include "Vertex.h"
#include "GraphicsThrowMacros.h"

namespace Bind 
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf)
			:
			VertexBuffer(gfx, "?", vbuf)
		{}
		VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf)
			:
			tag(tag),
			stride((UINT)vbuf.GetLayout().Size())
		{
			HRESULT hr;

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = UINT(vbuf.SizeBytes());
			bd.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vbuf.GetData();
			GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
		}
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Bindable> Resolve(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		UINT stride;
		std::string tag;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}