#pragma once

/*
Color definitions in ImGui are a good starting point,
but do not cover all the intricacies of Spectrum's possible colors
in controls and widgets.

One big difference is that ImGui communicates widget activity
(hover, pressed) with their background, while spectrum uses a mix
of background and border, with border being the most common choice.

Because of this, we reference extra colors in spectrum from
imgui_widgets.cpp directly. Theme-dependent colors are accessed at
runtime via Spectrum::Colors (a pointer swapped by SetTheme()).
*/

namespace ImGui {
    namespace Spectrum {
        // Widget metric constants
        const float CHECKBOX_BORDER_SIZE = 2.0f;
        const float CHECKBOX_ROUNDING = 2.0f;

        // Load SourceSansProRegular and sets it as a default font.
        // You may want to call ImGui::GetIO().Fonts->Clear() before this
        void LoadFont(float size = 16.0f);

        // Sets the ImGui style to Spectrum. Pass dark=true for dark theme.
        // To switch themes at runtime, call again with the new value.
        void StyleColorsSpectrum(bool dark = false);

        namespace { // Unnamed namespace, since we only use this here.
            unsigned int Color(unsigned int c) {
                // ImGui ImU32 format is 0xAABBGGRR; standard RGB literals are 0xRRGGBB.
                // Swap R and B channels and add full alpha.
                const short a = 0xFF;
                const short r = (c >> 16) & 0xFF;
                const short g = (c >> 8) & 0xFF;
                const short b = (c >> 0) & 0xFF;
                return (a << 24) | (r << 0) | (g << 8) | (b << 16);
            }
        }

        inline unsigned int color_alpha(unsigned int alpha, unsigned int c) {
            return ((alpha & 0xFF) << 24) | (c & 0x00FFFFFF);
        }

        namespace Static { // static colors (same in both light and dark themes)
            const unsigned int NONE = 0x00000000; // transparent
            const unsigned int WHITE = Color(0xFFFFFF);
            const unsigned int BLACK = Color(0x000000);
            const unsigned int GRAY200 = Color(0xF4F4F4);
            const unsigned int GRAY300 = Color(0xEAEAEA);
            const unsigned int GRAY400 = Color(0xD3D3D3);
            const unsigned int GRAY500 = Color(0xBCBCBC);
            const unsigned int GRAY600 = Color(0x959595);
            const unsigned int GRAY700 = Color(0x767676);
            const unsigned int GRAY800 = Color(0x505050);
            const unsigned int GRAY900 = Color(0x323232);
            const unsigned int BLUE400 = Color(0x378EF0);
            const unsigned int BLUE500 = Color(0x2680EB);
            const unsigned int BLUE600 = Color(0x1473E6);
            const unsigned int BLUE700 = Color(0x0D66D0);
            const unsigned int RED400 = Color(0xEC5B62);
            const unsigned int RED500 = Color(0xE34850);
            const unsigned int RED600 = Color(0xD7373F);
            const unsigned int RED700 = Color(0xC9252D);
            const unsigned int ORANGE400 = Color(0xF29423);
            const unsigned int ORANGE500 = Color(0xE68619);
            const unsigned int ORANGE600 = Color(0xDA7B11);
            const unsigned int ORANGE700 = Color(0xCB6F10);
            const unsigned int GREEN400 = Color(0x33AB84);
            const unsigned int GREEN500 = Color(0x2D9D78);
            const unsigned int GREEN600 = Color(0x268E6C);
            const unsigned int GREEN700 = Color(0x12805C);
        }

        // Theme-dependent color palette. Access via Spectrum::Colors->.
        struct SpectrumPalette {
            unsigned int GRAY50, GRAY75, GRAY100, GRAY200, GRAY300, GRAY400, GRAY500, GRAY600, GRAY700, GRAY800, GRAY900;
            unsigned int BLUE400, BLUE500, BLUE600, BLUE700;
            unsigned int RED400, RED500, RED600, RED700;
            unsigned int ORANGE400, ORANGE500, ORANGE600, ORANGE700;
            unsigned int GREEN400, GREEN500, GREEN600, GREEN700;
            unsigned int INDIGO400, INDIGO500, INDIGO600, INDIGO700;
            unsigned int CELERY400, CELERY500, CELERY600, CELERY700;
            unsigned int MAGENTA400, MAGENTA500, MAGENTA600, MAGENTA700;
            unsigned int YELLOW400, YELLOW500, YELLOW600, YELLOW700;
            unsigned int FUCHSIA400, FUCHSIA500, FUCHSIA600, FUCHSIA700;
            unsigned int SEAFOAM400, SEAFOAM500, SEAFOAM600, SEAFOAM700;
            unsigned int CHARTREUSE400, CHARTREUSE500, CHARTREUSE600, CHARTREUSE700;
            unsigned int PURPLE400, PURPLE500, PURPLE600, PURPLE700;
        };

        extern const SpectrumPalette* Colors;
    }
}
