#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return keystates[keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if (keybuffer.size() > 0u)
    {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }
    return {};
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return keybuffer.empty();
}

void Keyboard::Flushkey() noexcept
{
    keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
    if (charbuffer.size() > 0u)
    {
        unsigned char charcode = charbuffer.front();
        charbuffer.pop();
        return charcode;
    }
    
    return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
    return charbuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
    Flushkey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    autoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    autoRepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer( keybuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
    keystates.reset();
}


template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
