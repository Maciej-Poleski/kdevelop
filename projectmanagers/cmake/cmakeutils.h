/* KDevelop CMake Support
 *
 * Copyright 2009 Andreas Pakulat <apaku@gmx.de>
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

#ifndef CMAKEUTILS_H
#define CMAKEUTILS_H

#include "cmakecommonexport.h"

#include <QStringList>
#include <util/path.h>
#include <QStandardPaths>

namespace KDevelop
{
    class ProjectBaseItem;
    class IProject;
}

class ICMakeDocumentation;
class CMakeCacheModel;

namespace CMake
{
    /**
     * Checks wether there's a need to run cmake for the given project item
     * This is the case if no builddir has been specified, in which case
     * it asks for one.
     *
     * @returns true if configure should be run, false otherwise
     */
    KDEVCMAKECOMMON_EXPORT bool checkForNeedingConfigure( KDevelop::IProject* project );

    /**
     * @returns the current builddir for the given project or an empty url if none
     * has been set by the user.
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path currentBuildDir( KDevelop::IProject* project );

    /**
     * @returns the path to the 'compile_commands.json' file in the current builddir for the given project
     * or an empty url if none has been set by the user.
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path commandsFile( KDevelop::IProject* project );
    /**
     * @returns the path to the 'CMakeFiles/TargetDirectories.txt' file in the current builddir for the given project
     * or an empty url if none has been set by the user.
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path targetDirectoriesFile( KDevelop::IProject* project );

    /**
     * @returns the current build type for the given project or "Release" as default value.
     */
    KDEVCMAKECOMMON_EXPORT QString currentBuildType( KDevelop::IProject* project );

    /**
     * @returns the current cmake binary for the given project or
     * QStandardPaths::findExecutable("cmake") as default value. 
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path currentCMakeBinary( KDevelop::IProject* project );

    /**
     * @returns the current install dir for the given project or "/usr/local" as default value.
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path currentInstallDir( KDevelop::IProject* project );
    
    /**
     * @returns the current extra arguments for the given project or "" as default value.
     */
    KDEVCMAKECOMMON_EXPORT QString currentExtraArguments( KDevelop::IProject* project );

    /**
     * @returns the current build dir for the given project.
     */
    KDEVCMAKECOMMON_EXPORT QString projectRootRelative( KDevelop::IProject* project );
    
    /**
     * @returns whether there's projectRootRelative defined
     */
    KDEVCMAKECOMMON_EXPORT bool hasProjectRootRelative( KDevelop::IProject* project );
    
    /**
     * Convenience function to get the project root.
     */
    KDEVCMAKECOMMON_EXPORT KDevelop::Path projectRoot( KDevelop::IProject* project );
    
    /**
     * @returns the environment configuration for a @p project
     */
    KDEVCMAKECOMMON_EXPORT QString currentEnvironment( KDevelop::IProject* project );

    /**
     * Sets the current install dir for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentInstallDir( KDevelop::IProject* project, const KDevelop::Path &path );

    /**
     * Sets the current build type for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentBuildType( KDevelop::IProject* project, const QString& type );

    /**
     * Sets the current cmake binary for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentCMakeBinary( KDevelop::IProject* project, const KDevelop::Path &path );

    /**
     * Sets the current build dir for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentBuildDir( KDevelop::IProject* project, const KDevelop::Path& path );
    
    /**
     * Sets the current build dir for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setProjectRootRelative( KDevelop::IProject* project, const QString& path);
    
    /**
     * Sets the current extra arguments for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentExtraArguments( KDevelop::IProject* project, const QString& args );

    /**
     * Obtains a cmake documentation instance if it exists
     */
    KDEVCMAKECOMMON_EXPORT ICMakeDocumentation* cmakeDocumentation();
    
    /**
     * Retrieves the configured build directories for @p project.
     */
    KDEVCMAKECOMMON_EXPORT QStringList allBuildDirs(KDevelop::IProject* project);

    /**
     * Attempts to migrate the CMake configuration to per-builddir format.
     * Silently returns if the migration has already been performed.
     */
    KDEVCMAKECOMMON_EXPORT void attemptMigrate( KDevelop::IProject* project );

    /**
     * Attempts to update CMake configuration keys from the cache data.
     *
     * @param model The CMake cache model to load data from. If NULL, the model is created based on build directory path for the given index.
     */
    KDEVCMAKECOMMON_EXPORT void updateConfig( KDevelop::IProject* project, int buildDirectory);

    /**
     * Returns the current build directory count.
     */
    KDEVCMAKECOMMON_EXPORT int buildDirCount( KDevelop::IProject* project );

    /**
     * Sets the build directory count (equivalent to adding a new build directory).
     */
    KDEVCMAKECOMMON_EXPORT void setBuildDirCount( KDevelop::IProject* project, int count );

    /**
     * @returns the current builddir index for the given project or -1 if none
     * has been set by the user.
     */
    KDEVCMAKECOMMON_EXPORT int currentBuildDirIndex( KDevelop::IProject *project );

    /**
     * Sets the current build dir index for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentBuildDirIndex( KDevelop::IProject* project, int buildDirIndex );

    /**
     * A hack to avoid adding an optional "build directory index" parameter to all functions here.
     * This function sets an alternate build directory index key that overrides regular build directory index.
     */
    KDEVCMAKECOMMON_EXPORT void setOverrideBuildDirIndex( KDevelop::IProject* project, int overrideBuildDirIndex );

    /**
     * This removes build directory override key (\ref setOverrideBuildDirIndex).
     * Silently returns if there is no override.
     *
     * @param writeToMainIndex Whether the overridden index should be saved to regular
     */
    KDEVCMAKECOMMON_EXPORT void removeOverrideBuildDirIndex( KDevelop::IProject* project, bool writeToMainIndex = false );

    /**
     * Sets the environment configuration for the given project.
     */
    KDEVCMAKECOMMON_EXPORT void setCurrentEnvironment( KDevelop::IProject* project, const QString& environment );

    /**
     * Removes current build directory (overridden or not) from the project configuration.
     * Override is then cleared and index set to -1.
     */
    KDEVCMAKECOMMON_EXPORT void removeBuildDirConfig( KDevelop::IProject* project );

    KDEVCMAKECOMMON_EXPORT KDevelop::Path::List resolveSystemDirs(KDevelop::IProject* project, const QStringList& dirs);

    /** Runs the process specified by @p execName with @p args */
    KDEVCMAKECOMMON_EXPORT QString executeProcess(const QString& execName, const QStringList& args=QStringList());
}

#endif


