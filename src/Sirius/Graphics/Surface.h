//
// Created by Lms on 05/02/2025.
//

#ifndef SURFACE_H
#define SURFACE_H
#define NOMINMAX


#include "Core/SiriusException.h"
#include <memory>


class Surface {
public:
    class Color {
    public:
        unsigned int dword;

        constexpr Color() noexcept : dword() {
        }

        constexpr Color(const Color &col) noexcept = default;

        constexpr explicit Color(unsigned int dw) noexcept
            : dword(dw) {
        }

        constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
            : dword((x << 24u) | (r << 16u) | (g << 8u) | b) {
        }

        constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
            : dword((r << 16u) | (g << 8u) | b) {
        }

        constexpr Color(const Color& col, unsigned char x) noexcept
            : Color((x << 24u) | col.dword) {
        }

        Color &operator =(const Color& color) noexcept = default;

        [[nodiscard]] constexpr unsigned char GetX() const noexcept {
            return dword >> 24u;
        }

        [[nodiscard]] constexpr unsigned char GetA() const noexcept {
            return GetX();
        }

        [[nodiscard]] constexpr unsigned char GetR() const noexcept {
            return (dword >> 16u) & 0xFFu;
        }

        [[nodiscard]] constexpr unsigned char GetG() const noexcept {
            return (dword >> 8u) & 0xFFu;
        }

        [[nodiscard]] constexpr unsigned char GetB() const noexcept {
            return dword & 0xFFu;
        }

        void SetX(unsigned char x) noexcept {
            dword = (dword & 0xFFFFFFu) | (x << 24u);
        }

        void SetA(unsigned char a) noexcept {
            SetX(a);
        }

        void SetR(unsigned char r) noexcept {
            dword = (dword & 0xFF00FFFFu) | (r << 16u);
        }

        void SetG(unsigned char g) noexcept {
            dword = (dword & 0xFFFF00FFu) | (g << 8u);
        }

        void SetB(unsigned char b) noexcept {
            dword = (dword & 0xFFFFFF00u) | b;
        }
    };

    class Exception : public SiriusException {
    public:
        Exception(int line, const char *file, std::string note) noexcept;

        const char *what() const noexcept override;

        const char *GetType() const noexcept override;

        const std::string &GetNote() const noexcept;

    private:
        std::string note;
    };

    Surface(unsigned int width, unsigned int height) noexcept;

    Surface(Surface &&source) noexcept;

    Surface(Surface &) = delete;

    Surface &operator=(Surface &&donor) noexcept;

    Surface &operator=(const Surface &) = delete;

    ~Surface();

    void Clear(Color fillValue) noexcept;

    void PutPixel(unsigned int x, unsigned int y, Color c) noexcept;

    [[nodiscard]] Color GetPixel(unsigned int x, unsigned int y) const noexcept;

    [[nodiscard]] unsigned int GetWidth() const noexcept;

    [[nodiscard]] unsigned int GetHeight() const noexcept;

    Color *GetBufferPtr() noexcept;

    const Color *GetBufferPtr() const noexcept;

    const Color *GetBufferPtrConst() const noexcept;

    static Surface FromFile(const std::string &name);

    void Save(const std::string &filename) const;

    void Copy(const Surface &src) noexcept;

private:
    Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;

    std::unique_ptr<Color[]> pBuffer;
    unsigned int width;
    unsigned int height;
};


#endif //SURFACE_H
