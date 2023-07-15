#pragma once
#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public: 
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown, 
			Move,
			Enter,
			Leave,
			Invalid,
		};
	private:
		Type t;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event() noexcept
			:
			t(Type::Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			x(0),
			y(0)
		{};
		Event(Type t, const Mouse& parent) noexcept
			:
			t(t),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{};

		bool IsValid() const noexcept
		{
			return t != Type::Invalid;
		}


		Type GetType() const noexcept
		{
			return t;
		}

		std::pair<int, int> GetPos() const noexcept
		{
			return { x,y };
		}

		int GetPosX() const noexcept
		{
			return x;
		}

		int GetPosY() const noexcept
		{
			return y;
		}

		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}

		bool RightIsPressed() const noexcept
		{
			return rightIsPressed;
		}

	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Flush() noexcept;
private:
	void OnMouseMove(int newX, int newY) noexcept;
	void onMouseLeave() noexcept;
	void onMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void onWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

private:
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int x, y;
	int wheelDeltaCarry = 0;
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;
};

