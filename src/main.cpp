#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

extern "C" {
#include <qrencode.h>
}

namespace qrcode {

struct QR {
    std::unique_ptr<QRcode, decltype(&QRcode_free)> code;

    QR(const std::string& text, QRecLevel level = QR_ECLEVEL_M)
        : code(QRcode_encodeString(text.c_str(), 0, level, QR_MODE_8, 1), &QRcode_free)
    {
        if (!code) {
            throw std::runtime_error("Failed to generate QR code for: " + text);
        }
    }

    int width() const { return code->width; }
    const unsigned char* data() const { return code->data; }
};

class TerminalRenderer {
public:
    TerminalRenderer(int margin = 4, const std::string& black = "██", const std::string& white = "  ")
        : margin_(margin), black_(black), white_(white) {}

    void render(const QR& qr) const {
        for (int y = -margin_; y < qr.width() + margin_; ++y) {
            for (int x = -margin_; x < qr.width() + margin_; ++x) {
                bool black = false;
                if (x >= 0 && y >= 0 && x < qr.width() && y < qr.width())
                    black = qr.data()[y * qr.width() + x] & 1;

                std::cout << (black ? black_ : white_);
            }
            std::cout << '\n';
        }
    }

private:
    int margin_;
    std::string black_;
    std::string white_;
};

class SVGRenderer {
public:
    SVGRenderer(int margin = 4, int scale = 10)
        : margin_(margin), scale_(scale) {}

    void render(const QR& qr, const std::string& path) const {
        std::ofstream out(path);
        if (!out) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        int size = (qr.width() + margin_ * 2) * scale_;
        out << "<svg xmlns='http://www.w3.org/2000/svg' width='" << size << "' height='" << size << "'>\n";
        out << "<rect width='100%' height='100%' fill='white'/>\n";
        for (int y = 0; y < qr.width(); ++y)
            for (int x = 0; x < qr.width(); ++x)
                if (qr.data()[y * qr.width() + x] & 1)
                    out << "<rect x='" << (x + margin_) * scale_ << "' y='" << (y + margin_) * scale_
                        << "' width='" << scale_ << "' height='" << scale_ << "'/>\n";
        out << "</svg>\n";
    }

private:
    int margin_;
    int scale_;
};

}

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <text-to-encode> [output.svg]\n";
            return 1;
        }

        qrcode::QR qr(argv[1], QR_ECLEVEL_M);

        if (argc >= 3) {
            qrcode::SVGRenderer renderer;
            renderer.render(qr, argv[2]);
        } else {
            qrcode::TerminalRenderer renderer;
            renderer.render(qr);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
