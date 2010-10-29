/*  This file is part of KDevelop
    Copyright 2009 Aleix Pol <aleixpol@kde.org>
    Copyright 2009 David Nolden <david.nolden.kdevelop@art-master.de>
    Copyright 2010 Benjamin Port <port.benjamin@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "qthelpprovider.h"
#include <QtCore/QObject>

#include <KDebug>
#include <QHelpIndexModel>
#include <QTemporaryFile>
#include <QHelpContentModel>

#include <KProcess>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <KAboutData>
#include <KStandardDirs>

#include <language/duchain/duchain.h>
#include <language/duchain/declaration.h>
#include <language/duchain/duchainlock.h>

#include <KIcon>
#include "qthelpdocumentation.h"

QtHelpProvider::QtHelpProvider(QObject *parent, const KComponentData &componentData, const QString &fileName, const QString &name, const QString &iconName, const QVariantList &args)
    : QtHelpProviderAbstract(parent, componentData, QHelpEngineCore::namespaceName(fileName) + ".qhc", args)
    , m_fileName(fileName)
    , m_name(name)
    , m_icon(iconName)
{
    m_engine.registerDocumentation(m_fileName);
}

QIcon QtHelpProvider::icon() const
{
    return KIcon(m_icon);
}

QString QtHelpProvider::name() const
{
    return m_name;
}

QString QtHelpProvider::fileName() const
{
    return m_fileName;
}

QString QtHelpProvider::iconName() const
{
    return m_icon;
}
