/*
 * This file is part of KDevelop
 *
 * Copyright 2014 Sergey Kalinichev <kalinichev.so.0@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "gcclikecompiler.h"

#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QMap>

#include "../debugarea.h"

#ifdef _WIN32
#define NULL_DEVICE "NUL"
#else
#define NULL_DEVICE "/dev/null"
#endif

using namespace KDevelop;

namespace
{
QStringList languageOptions(const QString& arguments)
{
    const QRegularExpression regexp("-std=(c|c\\+\\+)[0-9]{2}");

    auto result = regexp.match(arguments);
    if(result.hasMatch()){
        auto standard = result.captured(0);
        auto language = result.captured(1) == QStringLiteral("c++") ? QStringLiteral("-xc++") : QStringLiteral("-xc");
        return {standard, language};
    }

    return {QStringLiteral("--std=c++11"), QStringLiteral("-xc++")};
}

}

Defines GccLikeCompiler::defines(const QString& arguments) const
{
    if (!m_definesIncludes.value(arguments).definedMacros.isEmpty() ) {
        return m_definesIncludes.value(arguments).definedMacros;
    }

    // #define a 1
    // #define a
    QRegExp defineExpression( "#define\\s+(\\S+)(?:\\s+(.*)\\s*)?");

    QProcess proc;
    proc.setProcessChannelMode( QProcess::MergedChannels );

    auto compilerArguments = languageOptions(arguments);
    compilerArguments.append("-dM");
    compilerArguments.append("-E");
    compilerArguments.append(NULL_DEVICE);

    proc.start(path(), compilerArguments);

    if ( !proc.waitForStarted( 1000 ) || !proc.waitForFinished( 1000 ) ) {
        definesAndIncludesDebug() <<  "Unable to read standard macro definitions from "<< path();
        return {};
    }

    while ( proc.canReadLine() ) {
        auto line = proc.readLine();

        if ( defineExpression.indexIn( line ) != -1 ) {
            m_definesIncludes[arguments].definedMacros[defineExpression.cap( 1 )] = defineExpression.cap( 2 ).trimmed();
        }
    }

    return m_definesIncludes[arguments].definedMacros;
}

Path::List GccLikeCompiler::includes(const QString& arguments) const
{
    if ( !m_definesIncludes.value(arguments).includePaths.isEmpty() ) {
        return m_definesIncludes.value(arguments).includePaths;
    }

    QProcess proc;
    proc.setProcessChannelMode( QProcess::MergedChannels );

    // The following command will spit out a bunch of information we don't care
    // about before spitting out the include paths.  The parts we care about
    // look like this:
    // #include "..." search starts here:
    // #include <...> search starts here:
    //  /usr/lib/gcc/i486-linux-gnu/4.1.2/../../../../include/c++/4.1.2
    //  /usr/lib/gcc/i486-linux-gnu/4.1.2/../../../../include/c++/4.1.2/i486-linux-gnu
    //  /usr/lib/gcc/i486-linux-gnu/4.1.2/../../../../include/c++/4.1.2/backward
    //  /usr/local/include
    //  /usr/lib/gcc/i486-linux-gnu/4.1.2/include
    //  /usr/include
    // End of search list.

    auto compilerArguments = languageOptions(arguments);
    compilerArguments.append("-E");
    compilerArguments.append("-v");
    compilerArguments.append(NULL_DEVICE);

    proc.start(path(), compilerArguments);

    if ( !proc.waitForStarted( 1000 ) || !proc.waitForFinished( 1000 ) ) {
        definesAndIncludesDebug() <<  "Unable to read standard include paths from " << path();
        return {};
    }

    // We'll use the following constants to know what we're currently parsing.
    enum Status {
        Initial,
        FirstSearch,
        Includes,
        Finished
    };
    Status mode = Initial;

    foreach( const QString &line, QString::fromLocal8Bit( proc.readAllStandardOutput() ).split( '\n' ) ) {
        switch ( mode ) {
            case Initial:
                if ( line.indexOf( "#include \"...\"" ) != -1 ) {
                    mode = FirstSearch;
                }
                break;
            case FirstSearch:
                if ( line.indexOf( "#include <...>" ) != -1 ) {
                    mode = Includes;
                    break;
                }
            case Includes:
                //Detect the include-paths by the first space that is prepended. Reason: The list may contain relative paths like "."
                if ( !line.startsWith( ' ' ) ) {
                    // We've reached the end of the list.
                    mode = Finished;
                } else {
                    // This is an include path, add it to the list.
                    m_definesIncludes[arguments].includePaths << Path( QDir::cleanPath( line.trimmed() ) );
                }
                break;
            default:
                break;
        }
        if ( mode == Finished ) {
            break;
        }
    }

    return m_definesIncludes[arguments].includePaths;
}

GccLikeCompiler::GccLikeCompiler(const QString& name, const QString& path, bool editable, const QString& factoryName):
    ICompiler(name, path, factoryName, editable)
{}
