#pragma once

#include "imgui.h"

namespace prism {

enum class GruvboxPalette {
    Bg = 0,
    Bg0_h,
    Bg0_s,
    Bg0,
    Bg1,
    Bg2,
    Bg3,
    Bg4,
    Fg,
    Fg0,
    Fg1,
    Fg2,
    Fg3,
    Fg4,
    Red,
    Green,
    Yellow,
    Blue,
    Purple,
    Aqua,
    Orange,
    Gray,
    DarkGray,
    LightRed,
    LightGreen,
    LightYellow,
    LightBlue,
    LightPurple,
    LightAqua,
    LightOrange,
};

static ImVec4 GruvboxTheme(GruvboxPalette color) {
    switch (color) {
        case GruvboxPalette::Bg:            return ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
        case GruvboxPalette::Bg0_h:         return ImVec4{ 0x1d/255.0f, 0x20/255.0f, 0x21/255.0f, 1.0f };
        case GruvboxPalette::Bg0_s:         return ImVec4{ 0x32/255.0f, 0x30/255.0f, 0x2f/255.0f, 1.0f };
        case GruvboxPalette::Bg0:           return ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
        case GruvboxPalette::Bg1:           return ImVec4{ 0x3c/255.0f, 0x38/255.0f, 0x36/255.0f, 1.0f };
        case GruvboxPalette::Bg2:           return ImVec4{ 0x50/255.0f, 0x49/255.0f, 0x45/255.0f, 1.0f };
        case GruvboxPalette::Bg3:           return ImVec4{ 0x66/255.0f, 0x5c/255.0f, 0x54/255.0f, 1.0f };
        case GruvboxPalette::Bg4:           return ImVec4{ 0x7c/255.0f, 0x6f/255.0f, 0x64/255.0f, 1.0f };
        case GruvboxPalette::Fg:            return ImVec4{ 0xeb/255.0f, 0xdb/255.0f, 0xb2/255.0f, 1.0f };
        case GruvboxPalette::Fg0:           return ImVec4{ 0xfb/255.0f, 0xf1/255.0f, 0xc7/255.0f, 1.0f };
        case GruvboxPalette::Fg1:           return ImVec4{ 0xeb/255.0f, 0xdb/255.0f, 0xb2/255.0f, 1.0f };
        case GruvboxPalette::Fg2:           return ImVec4{ 0xd5/255.0f, 0xc4/255.0f, 0xa1/255.0f, 1.0f };
        case GruvboxPalette::Fg3:           return ImVec4{ 0xbd/255.0f, 0xae/255.0f, 0x93/255.0f, 1.0f };
        case GruvboxPalette::Fg4:           return ImVec4{ 0xa8/255.0f, 0x99/255.0f, 0x84/255.0f, 1.0f };
        case GruvboxPalette::Red:           return ImVec4{ 0xcc/255.0f, 0x24/255.0f, 0x1d/255.0f, 1.0f };
        case GruvboxPalette::Green:         return ImVec4{ 0x98/255.0f, 0x97/255.0f, 0x1a/255.0f, 1.0f };
        case GruvboxPalette::Yellow:        return ImVec4{ 0xd7/255.0f, 0x99/255.0f, 0x21/255.0f, 1.0f };
        case GruvboxPalette::Blue:          return ImVec4{ 0x45/255.0f, 0x85/255.0f, 0x88/255.0f, 1.0f };
        case GruvboxPalette::Purple:        return ImVec4{ 0xb1/255.0f, 0x62/255.0f, 0x86/255.0f, 1.0f };
        case GruvboxPalette::Aqua:          return ImVec4{ 0x68/255.0f, 0x9d/255.0f, 0x6a/255.0f, 1.0f };
        case GruvboxPalette::Orange:        return ImVec4{ 0xd6/255.0f, 0x5d/255.0f, 0x0e/255.0f, 1.0f };
        case GruvboxPalette::Gray:          return ImVec4{ 0xa8/255.0f, 0x99/255.0f, 0x84/255.0f, 1.0f };
        case GruvboxPalette::DarkGray:      return ImVec4{ 0x92/255.0f, 0x83/255.0f, 0x74/255.0f, 1.0f };
        case GruvboxPalette::LightRed:      return ImVec4{ 0xfb/255.0f, 0x49/255.0f, 0x34/255.0f, 1.0f };
        case GruvboxPalette::LightGreen:    return ImVec4{ 0xb8/255.0f, 0xbb/255.0f, 0x26/255.0f, 1.0f };
        case GruvboxPalette::LightYellow:   return ImVec4{ 0xfa/255.0f, 0xbd/255.0f, 0x2f/255.0f, 1.0f };
        case GruvboxPalette::LightBlue:     return ImVec4{ 0x83/255.0f, 0xa5/255.0f, 0x98/255.0f, 1.0f };
        case GruvboxPalette::LightPurple:   return ImVec4{ 0xd3/255.0f, 0x86/255.0f, 0x9b/255.0f, 1.0f };
        case GruvboxPalette::LightAqua:     return ImVec4{ 0x8e/255.0f, 0xc0/255.0f, 0x7c/255.0f, 1.0f };
        case GruvboxPalette::LightOrange:   return ImVec4{ 0xfe/255.0f, 0x80/255.0f, 0x19/255.0f, 1.0f };
        default:                            return ImVec4(1, 0, 1, 1);
    }
}

} // namespace prism