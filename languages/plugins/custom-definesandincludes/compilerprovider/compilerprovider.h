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

#ifndef COMPILERSPROVIDER_H
#define COMPILERSPROVIDER_H

#include "icompilerprovider.h"

#include <QVariantList>
#include <QVector>

#include <interfaces/iplugin.h>

#include <QString>
#include <QScopedPointer>

class CompilerProvider : public KDevelop::IPlugin, public ICompilerProvider
{
    Q_OBJECT
    Q_INTERFACES( ICompilerProvider )
public:
    explicit CompilerProvider(QObject* parent, const QVariantList& args = QVariantList() );

    /// TODO: support custom paths to compilers: /path/to/gcc
    virtual bool setCompiler( const QString& name ) override;

    class CompilerProviderPrivate;
    QScopedPointer<CompilerProviderPrivate> d;
};

#endif // COMPILERSPROVIDER_H
