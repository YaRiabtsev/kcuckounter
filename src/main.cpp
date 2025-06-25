/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Yaroslav Riabtsev <yaroslav.riabtsev@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Qt
#include <QApplication>
#include <QCommandLineParser>
// std
#include <memory>
// KF
#include <KAboutData>
#include <KLocalizedString>
// own
#include "mainwindow.hpp"

int main(int argc, char* argv[]) {
    const QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kcuckounter");

    KAboutData about_data(
        QStringLiteral("kcuckounter"), i18n("kcuckounter"),
        QStringLiteral("1.0"),
        i18n(
            "A tool for improving arithmetic skills and memory retention "
            "through counting cards in different table-slots with different "
            "strategies. "
        ),
        KAboutLicense::MIT, i18n("(c) 2025, Yaroslav Riabtsev"), QString(),
        QStringLiteral("https://github.com/YaRiabtsev/kcuckounter"),
        QStringLiteral("yaroslav.riabtsev@rwth-aachen.de")
    );

    about_data.addAuthor(
        i18n("Yaroslav Riabtsev"), i18n("Original author"),
        QStringLiteral("yaroslav.riabtsev@rwth-aachen.de"),
        QStringLiteral("https://github.com/YaRiabtsev"),
        QStringLiteral("yariabtsev")
    );

    KAboutData::setApplicationData(about_data);

    QCommandLineParser parser;
    about_data.setupCommandLine(&parser);
    parser.process(app);
    about_data.processCommandLine(&parser);

    auto window = std::make_unique<MainWindow>();
    window->show();

    const int ret = QApplication::exec();
    window.reset();
    return ret;
}
