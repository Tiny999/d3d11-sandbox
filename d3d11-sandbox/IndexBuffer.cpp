#include "IndexBuffer.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short> indices)
		:
		IndexBuffer(gfx, "?", indices)
	{}
	IndexBuffer::IndexBuffer(Graphics& gfx, std::string tag, std::vector<unsigned short> indices)
		:
		count((UINT)indices.size()),
		tag(tag)
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
	std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& gfx, const std::string& tag, const std::vector<unsigned short>& indices)
	{
		assert(tag != "?");
		return Codex::Resolve<IndexBuffer>(gfx, tag, indices);
	}
	std::string IndexBuffer::GetUID() const noexcept
	{
		return GenerateUID_(tag);
	}
	std::string IndexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
}