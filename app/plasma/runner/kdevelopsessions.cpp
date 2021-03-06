/*
 *   Copyright 2008,2011 Sebastian Kügler <sebas@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "kdevelopsessions.h"

#include <QtCore/QCollator>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <KLocalizedString>


#include <QDebug>
#include <QIcon>
#include <QFile>
#include <KDirWatch>
#include <KToolInvocation>
#include <KConfig>
#include <KConfigGroup>
#include <QUrl>
#include <KStringHandler>
#include <QStandardPaths>

K_EXPORT_PLASMA_RUNNER(kdevelopsessions, KDevelopSessions)

bool kdevelopsessions_runner_compare_sessions(const Session &s1, const Session &s2) {
    QCollator c;
    return c.compare(s1.name, s2.name) < 0;
}

KDevelopSessions::KDevelopSessions(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent, args)
{
    setObjectName(QLatin1String("KDevelop Sessions"));
    setIgnoredTypes(Plasma::RunnerContext::File | Plasma::RunnerContext::Directory | Plasma::RunnerContext::NetworkLocation);
    m_icon = QIcon::fromTheme("kdevelop");

    loadSessions();

    // listen for changes to the list of kdevelop sessions
    KDirWatch *historyWatch = new KDirWatch(this);
    const QStringList sessiondirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QLatin1String("kdevelop/sessions"));
    foreach (const QString &dir, sessiondirs) {
        historyWatch->addDir(dir);
    }
    connect(historyWatch, &KDirWatch::dirty, this, &KDevelopSessions::loadSessions);
    connect(historyWatch, &KDirWatch::created, this, &KDevelopSessions::loadSessions);
    connect(historyWatch, &KDirWatch::deleted, this, &KDevelopSessions::loadSessions);

    Plasma::RunnerSyntax s(QLatin1String(":q:"), i18n("Finds KDevelop sessions matching :q:."));
    s.addExampleQuery(QLatin1String("kdevelop :q:"));
    addSyntax(s);

    setDefaultSyntax(Plasma::RunnerSyntax(QLatin1String("kdevelop"), i18n("Lists all the KDevelop editor sessions in your account.")));
}

KDevelopSessions::~KDevelopSessions()
{
}

QStringList findSessions()
{
    QStringList sessionDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "kdevelop/sessions", QStandardPaths::LocateDirectory);
    QStringList sessionrcs;
    Q_FOREACH(const QString& dir, sessionDirs) {
        QDir d(dir);
        Q_FOREACH(const QString& sessionDir, d.entryList(QDir::Dirs)) {
            QDir sd(d.absoluteFilePath(sessionDir));
            QString path(sd.filePath("sessionrc"));
            if(QFile::exists(path)) {
                sessionrcs += path;
            }
        }
    }
    return sessionrcs;
}

void KDevelopSessions::loadSessions()
{
    m_sessions.clear();
    // Switch kdevelop session: -u
    // Should we add a match for this option or would that clutter the matches too much?
    const QStringList list = findSessions();
    foreach (const QString &sessionfile, list)
    {
        Session session;
        session.id = sessionfile.section('/', -2, -2);
        KConfig cfg(sessionfile, KConfig::SimpleConfig);
        KConfigGroup group = cfg.group(QString());
        session.name = group.readEntry("SessionPrettyContents");;
        m_sessions << session;
    }
    std::sort(m_sessions.begin(), m_sessions.end(), kdevelopsessions_runner_compare_sessions);
}

void KDevelopSessions::match(Plasma::RunnerContext &context)
{
    if (m_sessions.isEmpty()) {
        return;
    }

    QString term = context.query();
    if (term.length() < 3) {
        return;
    }

    bool listAll = false;

    if (term.startsWith(QLatin1String("kdevelop"), Qt::CaseInsensitive)) {
        if (term.trimmed().compare(QLatin1String("kdevelop"), Qt::CaseInsensitive) == 0) {
            listAll = true;
            term.clear();
        } else if (term.at(8) == QLatin1Char(' ') ) {
            term.remove(QLatin1String("kdevelop"), Qt::CaseInsensitive);
            term = term.trimmed();
        } else {
            term.clear();
        }
    }

    if (term.isEmpty() && !listAll) {
        return;
    }

    foreach (const Session &session, m_sessions) {
        if (!context.isValid()) {
            return;
        }

        if (listAll || (!term.isEmpty() && session.name.contains(term, Qt::CaseInsensitive))) {
            Plasma::QueryMatch match(this);
            if (listAll) {
                // All sessions listed, but with a low priority
                match.setType(Plasma::QueryMatch::ExactMatch);
                match.setRelevance(0.8);
            } else {
                if (session.name.compare(term, Qt::CaseInsensitive) == 0) {
                    // parameter to kdevelop matches session exactly, bump it up!
                    match.setType(Plasma::QueryMatch::ExactMatch);
                    match.setRelevance(1.0);
                } else {
                    // fuzzy match of the session in "kdevelop $session"
                    match.setType(Plasma::QueryMatch::PossibleMatch);
                    match.setRelevance(0.8);
                }
            }
            match.setIcon(m_icon);
            match.setData(session.id);
            match.setText(session.name);
            match.setSubtext(i18n("Open KDevelop Session"));
            context.addMatch(match);
        }
    }
}

void KDevelopSessions::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context)
    QString sessionId = match.data().toString();
    if (sessionId.isEmpty()) {
        qWarning() << "No KDevelop session id in match!";
        return;
    }
    qDebug() << "Open KDevelop session" << sessionId;
    QStringList args;
    args << QLatin1String("--open-session") << sessionId;
    KToolInvocation::kdeinitExec(QLatin1String("kdevelop"), args);
}

#include "kdevelopsessions.moc"
