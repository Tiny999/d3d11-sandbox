#pragma once
#include "Bindable.h"

template <typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts)
	{
		HRESULT hr;

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_FAILED(GetContext(gfx)->Map(
			pConstantBuffer.Get(), 
			0u, D3D11_MAP_WRITE_DISCARD, 
			0u, &msr
		));

		memccpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);

	}
	ConstantBuffer(Graphics& gfx, const C& consts)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = size(consts);
		bd.StructureByteStride = 0u;


		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &consts;

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfc)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		GFX_THROW_FAILED(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};


template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C> ConstantBuffer;
	void Bind(Grapics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C> ConstantBuffer;
	void Bind(Grapics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};