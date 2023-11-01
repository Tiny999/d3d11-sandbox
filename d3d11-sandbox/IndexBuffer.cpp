#include "IndexBuffer.h"

namespace Bind
{

	IndexBuffer::IndexBuffer(Graphics& gfx, std::vector<unsigned short> indices)
		:
		count((UINT)indices.size())
	{
		HREFTYPE hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(unsigned short) * count);
		bd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = indices.data();

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pIndexBuffer));
	}

	void IndexBuffer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}