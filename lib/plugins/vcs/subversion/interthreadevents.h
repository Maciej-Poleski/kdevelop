#ifndef INTERTHREADEVENTS_H
#define INTERTHREADEVENTS_H

#include <QEvent>
#include <QString>

struct svn_opt_revision_t;
struct svn_auth_ssl_server_cert_info_t;
struct apr_array_header_t;

#define SVNACTION_PROGRESS           ( (QEvent::Type)15148 )
#define SVNACTION_NOTIFICATION       ( (QEvent::Type)15149 )
#define SVNLOGIN_IDPWDPROMPT         ( (QEvent::Type)15150 )
#define SVNLOGIN_SERVERTRUSTPROMPT   ( (QEvent::Type)15151 )
#define SVNCOMMIT_LOGMESSAGEPROMPT   ( (QEvent::Type)15160 )

class SvnNotificationEvent : public QEvent
{
public:
    SvnNotificationEvent( QString msg );
    virtual ~SvnNotificationEvent();
    // notification message
    QString m_msg;
};

class SvnInterThreadPromptEvent : public QEvent
{
public:
    SvnInterThreadPromptEvent(QEvent::Type type, void *data);
    virtual ~SvnInterThreadPromptEvent();
    void *m_data;
};

class SvnUserinputInfo
{
public:
    SvnUserinputInfo();
    
    bool receivedInfos();
    void setHasInfos();
    QObject *origSender;
private:
    bool hasInfo;
};

class SvnLoginInfo : public SvnUserinputInfo
{
public:
    SvnLoginInfo();
    
    void setData( QString userId, QString passWd, bool save );
    
    QString realm;
    QString userName;
    QString passWord;
    bool maySave;
};

class SvnServerCertInfo : public SvnUserinputInfo
{
public:
    SvnServerCertInfo();

    /// called by dialog box side
    void setData( int decision );
    /// certification informations to be displayed to user
    const svn_auth_ssl_server_cert_info_t *cert_info;
    /// failed reason reported by svn library
    unsigned int fails;
    /// -1 for rejection, 0 for accept once, 1 for accept permanently
    int m_decision;
};

class SvnCommitLogInfo : public SvnUserinputInfo
{
public:
    SvnCommitLogInfo();

    void setData( bool accept, QString msg );
    
    // from thread to dialogbox
    apr_array_header_t *m_commit_items;
    // from dialogbox to thread
    bool m_accept;
    QString m_message;
};

#endif
