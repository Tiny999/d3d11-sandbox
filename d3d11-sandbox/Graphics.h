#pragma once
#include "Win.h"
#include "Exception.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"
#include "GraphicsThrowMacros.h"
#include <DirectXMath.h>

class Graphics
{
	friend class Bindable;
public:
	class GfxException : public Exception
	{
		using Exception::Exception;
	};
	class HrException : public GfxException
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};
	class InfoException : public GfxException
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;
	void DrawTestTriangle(const float angle, float x, float y);
	void DrawIndexed(const UINT count);
	void SetProjection(DirectX::XMMATRIX proj);
	DirectX::XMMATRIX GetProjection() const noexcept;
	void EnableGUI() noexcept;
	void DisableGUI() noexcept;
	bool IsGuiEnabled() const noexcept;
private:
	DirectX::XMMATRIX projection;
private:
	bool isGuiEnabled = true;
	#ifndef NDEBUG
		DxgiInfoManager infoManager;
	#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

