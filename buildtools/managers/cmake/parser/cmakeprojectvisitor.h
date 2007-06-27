/* KDevelop CMake Support
 *
 * Copyright 2007 Aleix Pol <aleixpol@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef CMAKEPROJECTVISITOR_H
#define CMAKEPROJECTVISITOR_H

#include "cmakeastvisitor.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QHash>

class CMakeProjectVisitor : public CMakeAstVisitor
{
    public:
        CMakeProjectVisitor(QHash<QString, QStringList> *vars);
        virtual ~CMakeProjectVisitor() {}
        
        virtual void visit( const CustomCommandAst * ) { notImplemented(); }
        virtual void visit( const CustomTargetAst * ) { notImplemented(); }
        virtual void visit( const AddDefinitionsAst * ) { notImplemented(); }
        virtual void visit( const AddDependenciesAst * ) { notImplemented(); }
        virtual void visit( const AddExecutableAst * );
        virtual void visit( const AddLibraryAst * );
        virtual void visit( const AddSubdirectoryAst * );
        virtual void visit( const IncludeDirectoriesAst * );
        virtual void visit( const AddTestAst * ) { notImplemented(); }
        virtual void visit( const AuxSourceDirectoryAst * ) { notImplemented(); }
        virtual void visit( const BuildCommandAst * ) { notImplemented(); }
        virtual void visit( const BuildNameAst * ) { notImplemented(); }
        virtual void visit( const CMakeMinimumRequiredAst * ) { notImplemented(); }
        virtual void visit( const ConfigureFileAst * ) { notImplemented(); }
        virtual void visit( const IncludeAst * ) { notImplemented(); }
        
        virtual void visit( const SetAst * );
        virtual void visit( const ProjectAst * );
        
        virtual void visit( const CMakeAst * );
        
        QString projectName() const { return m_projectName; }
        QStringList subdirectories() const { return m_subdirectories; }
        QStringList targets() const { return m_filesPerTarget.keys(); }
        QStringList files(const QString &target) const { return m_filesPerTarget[target]; }
        QStringList includeDirectories() const { return m_includeDirectories; }
    private:
        void notImplemented() const;
        
        QString m_projectName;
        QStringList m_subdirectories;
        QStringList m_includeDirectories;
        QMap<QString, QStringList> m_filesPerTarget;
        QString m_root;
        QHash<QString, QStringList> *m_vars;
};

#endif
