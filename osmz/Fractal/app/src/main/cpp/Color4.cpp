#import "Color4.h"

Color4::Color4(uint32_t px) {
    a = static_cast<uint32_t>((px & 0xff000000) >> 24);
    r = static_cast<uint32_t>((px & 0x00ff0000) >> 16);
    g = static_cast<uint32_t>((px & 0x0000ff00) >> 8);
    b = static_cast<uint32_t>((px & 0x000000ff));
}

uint32_t Color4::abgr() {
    return (uint32_t) (a << 24 | r << 16 | g << 8 | b);
}

Color4 Color4::grayscale() {
    uint32_t c = static_cast<uint32_t>((r + g + b) / 3);
    return Color4(c, c, c, a);
}

Color4 Color4::inverse() {
    return Color4(255 - r, 255 - b, 255 - g, a);
}

Color4 operator+(const Color4 &c1, const Color4 &c2) {
    return Color4();
}

Color4 operator-(const Color4 &c1, const Color4 &c2) {
    return Color4(c1.r - c2.r, c1.b - c2.b, c1.g - c2.g, 255);
}

Color4 operator/(const Color4 &c1, uint32_t k) {
    return Color4(c1.r / k, c1.b / k, c1.g / k, 255);
}

Color4 operator*(const Color4 &c1, uint32_t k) {
    return Color4(c1.r * k, c1.b * k, c1.g * k, 255);
}