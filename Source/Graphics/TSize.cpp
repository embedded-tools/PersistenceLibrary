#include "TSize.h"

TSize::TSize()
{
    Width = 0;
    Height = 0;
}

TSize::TSize(const TSize &src)
{
    Width = src.Width;
    Height = src.Height;
}

TSize::TSize(short width, short height)
{
    Width = width;
    Height = height;
}

TSize& TSize::operator = (const TSize &src)
{
    Width = src.Width;
    Height = src.Height;
    return *this;
}