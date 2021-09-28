/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtCore/QCoreApplication>
#include <QTest>

#if __has_include(<valgrind/valgrind.h>)
#  include <valgrind/valgrind.h>
#  define HAVE_VALGRIND_H
#endif

class tst_BenchlibCallgrind: public QObject
{
    Q_OBJECT

private slots:
    void failInChildProcess();

    void twoHundredMillionInstructions();
};

void tst_BenchlibCallgrind::failInChildProcess()
{
#ifdef HAVE_VALGRIND_H
    static double f = 1.0;
    QBENCHMARK {
        for (int i = 0; i < 1000000; ++i) {
            f *= 1.1;
            if (RUNNING_ON_VALGRIND) QFAIL("Running under valgrind!");
        }
    }
#else
    QSKIP("Skipping test because I can't see <valgrind/valgrind.h> - is valgrind installed ?");
#endif
}

void tst_BenchlibCallgrind::twoHundredMillionInstructions()
{
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
    QBENCHMARK {
        __asm__ __volatile__(
            "mov $100000000,%%eax   \n"
            "1:               \n"
            "dec %%eax              \n"
            "jnz 1b            \n"
            : /* no output */
            : /* no input */
            : /* clobber */ "eax"
        );
    }
#else
    QSKIP("This test is only implemented for gcc on x86.");
#endif
}

int main(int argc, char *argv[])
{
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-callgrind");
    argc = args.size();
    argv = const_cast<char**>(&args[0]);

    QTEST_MAIN_IMPL(tst_BenchlibCallgrind)
}

#undef HAVE_VALGRIND_H
#include "tst_benchlibcallgrind.moc"
