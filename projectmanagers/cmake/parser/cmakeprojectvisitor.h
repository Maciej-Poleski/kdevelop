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

#include <QString>
#include <QStringList>
#include <QMap>
#include <QHash>
#include <QStack>

#include "cmakeastvisitor.h"
#include "cmakelistsparser.h"
#include "cmaketypes.h"
#include <language/duchain/topducontext.h>
class CMakeFunctionDesc;

namespace KDevelop
{
    class TopDUContext;
    class Declaration;
}

class KDEVCMAKECOMMON_EXPORT CMakeProjectVisitor : CMakeAstVisitor
{
    public:
        enum TargetType { Library, Executable, Custom };
        explicit CMakeProjectVisitor(const QString& root, KDevelop::ReferencedTopDUContext parent);
        virtual ~CMakeProjectVisitor() {}
        
        virtual int visit( const CustomCommandAst * );
        virtual int visit( const CustomTargetAst * );
        virtual int visit( const AddDefinitionsAst * );
        virtual int visit( const AddTestAst * );
        virtual int visit( const AddExecutableAst * );
        virtual int visit( const AddLibraryAst * );
        virtual int visit( const AddSubdirectoryAst * );
        virtual int visit( const IncludeDirectoriesAst * );
        virtual int visit( const IncludeAst * );
        virtual int visit( const MacroCallAst * call);
        virtual int visit( const FindPackageAst * );
        virtual int visit( const MarkAsAdvancedAst * );
        virtual int visit( const FindProgramAst * );
        virtual int visit( const FunctionAst * );
        virtual int visit( const FindPathAst * );
        virtual int visit( const FindLibraryAst * );
        virtual int visit( const FindFileAst * );
        virtual int visit( const IfAst * );
        virtual int visit( const ExecProgramAst * );
        virtual int visit( const ExecuteProcessAst * );
        virtual int visit( const FileAst * );
        virtual int visit( const MessageAst * );
        virtual int visit( const MathAst * );
        virtual int visit( const MacroAst * );
        virtual int visit( const ListAst * );
        virtual int visit( const GetFilenameComponentAst * );
        virtual int visit( const OptionAst * );
        virtual int visit( const SetAst * );
        virtual int visit( const ForeachAst * );
        virtual int visit( const ProjectAst * );
        virtual int visit( const SetTargetPropsAst * );
        virtual int visit( const StringAst * );
        virtual int visit( const SubdirsAst * );
        virtual int visit( const TryCompileAst * );
        virtual int visit( const TargetLinkLibrariesAst * );
        virtual int visit( const GetCMakePropertyAst * );
        virtual int visit( const RemoveDefinitionsAst * );
        virtual int visit( const SeparateArgumentsAst * );
        virtual int visit( const WhileAst * );
        virtual int visit( const GetSourceFilePropAst * );
        virtual int visit( const CMakeAst * );
        
        void setCacheValues( CacheValues* cache);
        void setVariableMap( VariableMap* vars );
        void setMacroMap( MacroMap* macros ) { m_macros=macros; }
        void setModulePath(const QStringList& mp) { m_modulePath=mp; }
        void setDefinitions(const Definitions& defs) { m_defs=defs; }

        const VariableMap* variables() const { return m_vars; }
        const Definitions& definitions() const { return m_defs; }
        
        QString projectName() const { return m_projectName; }
        QStringList subdirectories() const { return m_subdirectories; }
        QStringList targets() const { return m_filesPerTarget.keys(); }
        QStringList files(const QString &target) const { return m_filesPerTarget[target]; }
        QStringList targetDependencies(const QString & target) const;
        QStringList includeDirectories() const { return m_includeDirectories; }
        QString targetProperty(const QString& targetName, const QString &propName) const
            { return m_targetProperties[targetName][propName]; }
        bool targetHasProperty(const QString& targetName, const QString &propName) const
            { return m_targetProperties[targetName].contains(propName); }
            
        CMakeFunctionDesc folderDeclarationDescriptor(const QString& name) const { return m_folderDesc[name]; }
        CMakeFunctionDesc targetDeclarationDescriptor(const QString& name) const { return m_targetDesc[name]; }
            
        int walk(const CMakeFileContent& fc, int line);
        
//         static VariableType hasVariable(const QString &name);
        static QStringList envVarDirectories(const QString &varName);
        
//         enum FileType { Location, File, Executable, Library };
        static QString findFile(const QString& files, const QStringList &folders,
                                    const QStringList& suffixes=QStringList(), bool location=false);
        
        QString findExecutable(const QString& filenames, const QStringList& dirs,
                                    const QStringList& pathSuffixes=QStringList()) const;
        
        KDevelop::ReferencedTopDUContext context() const { return m_topctx; }
        const QMap<QString, KDevelop::IndexedDeclaration>& declarationsPerTarget() { return m_declarationsPerTarget; }
        QStringList resolveVariable(const CMakeFunctionArgument &exp);

        bool hasMacro(const QString& name) const;
        TargetType targetType(const QString& targetName) { return m_targetsType[targetName]; }

        struct VisitorState
        {
            const CMakeFileContent* code;
            int line;
            KDevelop::ReferencedTopDUContext context;
        };
        
    protected:
        struct IntPair
        {
            IntPair(int f, int s, int l) : first(f), second(s), level(l) {}
            int first, second, level;
            QString print() const { return QString("(%1, %2 : %3)").arg(first).arg(second).arg(level); }
            static QStringList printList(const QList<IntPair>& l)
            {
                QStringList r;
                foreach(const IntPair& i, l)
                    r+= i.print();
                return r;
            }
        };
        
        static QList< IntPair > parseArgument(const QString &exp);
        
    private:
        static KDevelop::ReferencedTopDUContext
            createContext(const KUrl& path, KDevelop::ReferencedTopDUContext aux, int endl ,int endc);
        
        void macroDeclaration(const CMakeFunctionDesc& def, const CMakeFunctionDesc& end, const QStringList& args);
        CMakeFunctionDesc resolveVariables(const CMakeFunctionDesc &exp);
        QStringList value(const QString& exp, const QList<IntPair>& poss, int& desired) const;
        QStringList theValue(const QString& exp, const IntPair& p) const;

        typedef QMap<QString, QString> TargetProperties;
        
        void defineTarget(const QString& id, const QStringList& sources, TargetType t);
        int notImplemented(const QString& n) const;
        bool haveToFind(const QString &varName);
        void createDefinitions(const CMakeAst* ast);
        void createUses(const CMakeFunctionDesc& ast);
        unsigned int currentIdentity() const;
        void printBacktrace(const QStack<VisitorState> &backtrace);
        VisitorState stackTop() const;
        QStringList dependees(const QString& s) const;

        QStringList m_modulePath;
        QString m_projectName;
        QStringList m_subdirectories;
        QStringList m_includeDirectories;
        QMap<QString, QStringList> m_filesPerTarget;
        QMap<QString, QStringList> m_generatedFiles;
        QMap<QString, KDevelop::IndexedDeclaration> m_declarationsPerTarget;
        QMap<QString, TargetType> m_targetsType;
        QMap<QString, TargetProperties> m_targetProperties;
        QMap<QString, CMakeFunctionDesc> m_folderDesc;
        QMap<QString, CMakeFunctionDesc> m_targetDesc;
        
        QStack< VisitorState > m_backtrace;
        QString m_root;
        VariableMap *m_vars;
        MacroMap *m_macros;
        const CacheValues* m_cache;
        Definitions m_defs;
        QStringList m_filesRead;
        KDevelop::ReferencedTopDUContext m_topctx;
        KDevelop::ReferencedTopDUContext m_parentCtx;

};

#endif