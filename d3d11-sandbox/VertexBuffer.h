#pragma once
#include "Bindable.h"
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
	protected:
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}