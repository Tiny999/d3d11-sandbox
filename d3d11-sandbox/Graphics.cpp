#include "Graphics.h"
#include "dxerr.h"
#include <sstream>

namespace wrl = Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}

Graphics::Graphics(HWND hWnd)
{

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	// for checking results of d3d functions
	HRESULT hr;

	// Create device, front and back buffers and swap chain and device context.
	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));


	// access swapchain subresource (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_THROW_FAILED(hr);
		}
	}
	
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::DrawTestTriangle()
{

	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] =
	{
		{ 0.0f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};

	sd.pSysMem = vertices;

	HRESULT hr;

	GFX_THROW_FAILED(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	GFX_THROW_INFO_ONLY(pContext->Draw(3u, 0u));
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	GfxException(line, file),
	hr(hr)
{
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << "(" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error string] " << GetErrorString() << std::endl
		<< "[Description]" << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Custom Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[513];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "CUSTOM GRAPHICS EXCEPTION [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	GfxException(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Custom Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
