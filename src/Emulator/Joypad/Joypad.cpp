#include <iostream>
#include "Joypad.hpp"

void Joypad::update(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)  key_down(e);
    if (e.type == SDL_KEYUP)    key_up(e);
}

void Joypad::write_byte(u8 value)
{
    m_column = (value >> 4) & 3;
}

// TODO: FIX A BUG??
u8 Joypad::read_byte()
{
    switch (m_column)
    {
        case 1: return m_rows[0];
        case 2: return m_rows[1];
		default: return (1);
    }
}

void Joypad::key_down(SDL_Event e)
{
    switch (e.key.keysym.sym)
    {
        case SDLK_s:
            m_rows[0] &= ~(0x1); // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] &= ~(0x2); // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] &= ~(0x4); // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] &= ~(0x8); // ENTER gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] &= ~(0x1);
            break;
        case SDLK_LEFT:
            m_rows[1] &= ~(0x2);
            break;
        case SDLK_UP:
            m_rows[1] &= ~(0x4);
            break;
        case SDLK_DOWN:
            m_rows[1] &= ~(0x8);
            break;
        default:
            break;
    }
}

void Joypad::key_up(SDL_Event e)
{
    switch (e.key.keysym.sym) {
        case SDLK_s:
            m_rows[0] |= 0x1; // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] |= 0x2; // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] |= 0x4; // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] |= 0x8; // ENTER gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] |= 0x1;
            break;
        case SDLK_LEFT:
            m_rows[1] |= 0x2;
            break;
        case SDLK_UP:
            m_rows[1] |= 0x4;
            break;
        case SDLK_DOWN:
            m_rows[1] |= 0x8;
            break;
        default:
            break;
    }
}