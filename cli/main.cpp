#include <exception>
#include <format>
#include <iostream>
#include <sstream>
#include <vector>

#include <CLI/CLI.hpp>
#include <termcolor/termcolor.hpp>

#include "config.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
// #include <ftxui/component/screen_interactive.hpp>
// #include <ftxui/screen/screen.hpp>
// #include <ftxui/screen/string.hpp>
#include <b64/cencode.h>
#include <qrcodegen/qrcodegen.hpp>
// #include <b64/encode.h>

using namespace std;
namespace tc = termcolor;
using namespace ftxui;

// std::string base64Encode(const std::vector<char>& data)
// {
//     vector<char> out(data.size() * 2);
//     base64_encodestate state;
//     base64_init_encodestate(&state);
//     char* p = out.data();
//     auto res = base64_encode_block(data.data(), data.size(), p, &state);
//     p += res;
//     res = base64_encode_blockend(p, &state);
//     p += res;
//     auto size = p - out.data();
//     return string(out.data(), size);
// }

int searchDataSizeForTargetQrSize(int leftSize, int rightSize, int targetQrSize)
{
    auto size = (leftSize + rightSize) / 2;
    if (leftSize >= rightSize)
        return size;
    vector<uint8_t> v(size, (char)255);
    try {
        auto code = qrcodegen::QrCode::encodeBinary(v, qrcodegen::QrCode::Ecc::LOW);
        auto qrSize = code.getSize();
        if (qrSize < targetQrSize) {
            return searchDataSizeForTargetQrSize(size + 1, rightSize, targetQrSize);
        } else if (qrSize > targetQrSize) {
            return searchDataSizeForTargetQrSize(leftSize, size - 1, targetQrSize);
        } else {
            return size;
        }
    } catch (const exception& e) {
        return searchDataSizeForTargetQrSize(leftSize, size - 1, targetQrSize);
    }
}

int getDataSizeForQrSize(int qrSize) { return searchDataSizeForTargetQrSize(0, 1 * 1024, qrSize); }

int main(int argc, char** argv)
{
    try {
        CLI::App app { format("QR Tcp Tunnel CLI (v{})", APP_VERSION) };

        CLI11_PARSE(app, argc, argv);

        auto summary = [&] {
            auto content = vbox({
                hbox({ text(L"- done:   "), text(L"3") | bold }) | color(Color::Green),
                hbox({ text(L"- active: "), text(L"2") | bold }) | color(Color::RedLight),
                hbox({ text(L"- queue:  "), text(L"9") | bold }) | color(Color::Red),
            });
            return window(text(L" Summary "), content);
        };

        auto document = //
            vbox({
                hbox({
                    summary(),
                    summary(),
                    summary() | flex,
                }),
                summary(),
                summary(),
            });

        // Limit the size of the document to 80 char.
        // document = document | size(WIDTH, GREATER_THAN, 10) | size(HEIGHT, GREATER_THAN, 10);

        // auto code = qrcodegen::QrCode::encodeText("c8B7yOVFVcgjJDUgBYBYTmEqw1Nt3xJIchAVFSLR+EylT21zbt6zZv/zUXp6BNal"
        //                                           "g6sU4CdrnhNGwFRt4qNpRfqEx6vkH5r1+PtSsR5PnqtyzWZc5TLeJ8HjqbzpF06U"
        //                                           "pZiXh9DYuO1jgS5DeVjYp2+5AnYESrsSwGwQ7RcY6x0=",
        //     qrcodegen::QrCode::Ecc::MEDIUM);

        int oldSize = 0;
        int dataSize = 0;

        char ch = 'A';

        auto qrCodeRenderer = Renderer([&] {
            auto termSize = Terminal::Size();

            auto size = min(termSize.dimx, (termSize.dimy - 4) * 2);
            if (oldSize != size) {
                // size = 1;
                dataSize = getDataSizeForQrSize(size);
                if (dataSize < 16)
                    dataSize = 16;
                oldSize = size;
            }

            // cout << dataSize << endl;

            vector<uint8_t> vec(dataSize, ch);
            auto code = qrcodegen::QrCode::encodeBinary(vec, qrcodegen::QrCode::Ecc::LOW);

            auto qrSize = code.getSize();

            auto c = Canvas(qrSize * 2 + 2, qrSize * 2 + 2);
            for (int y = 0; y < qrSize; y += 2) {
                for (int x = 0; x < qrSize; x++) {
                    auto v1 = code.getModule(x, y);
                    auto v2 = code.getModule(x, y + 1);

                    const char* ch = 0;
                    if (v1 && !v2)
                        ch = "▀";
                    else if (!v1 && v2)
                        ch = "▄";
                    else if (v1 && v2)
                        ch = "█";
                    if (ch)
                        c.DrawText(x * 2, y * 2, ch);
                }
            }
            return canvas(std::move(c));
        });

        auto page = Renderer(qrCodeRenderer, [&] {
            return vbox({
                text(format("QR TCP Tunnel {}", dataSize)),
                separator(),
                dbox({ qrCodeRenderer->Render() | center }) | flex /* | bgcolor(Color::Red)*/,
            });
        });

        auto pageWithKeys = CatchEvent(page, [&](Event e) {
            if (e.is_character()) {
                ch = e.character()[0];
            }
            return false;
        });

        auto screen = ScreenInteractive::Fullscreen();
        screen.SetCursor({ .shape = Screen::Cursor::Hidden });
        screen.Loop(pageWithKeys);

        // std::cout << screen.ToString() << '\0' << std::endl;

        return 0;
    } catch (const exception& e) {
        cerr << tc::red << "Error: " << e.what() << tc::reset << endl;
        return 1;
    }
}
