#include "konsoleviewpart.h"

#include <qwhatsthis.h>

#include <kgenericfactory.h>
#include <kiconloader.h>
#include <klocale.h>

#include "kdevcore.h"
#include "kdevtoplevel.h"

#include "konsoleviewwidget.h"


K_EXPORT_COMPONENT_FACTORY( libkdevkonsoleview, KGenericFactory<KonsoleViewPart>( "kdevkonsoleview" ) );

KonsoleViewPart::KonsoleViewPart(QObject *parent, const char *name, const QStringList &)
  : KDevPlugin(parent, name)
{
  m_widget = new KonsoleViewWidget(this);
  
    QWhatsThis::add(m_widget, i18n("Konsole\n\n"
                                   "This window contains an embedded konsole "
				   "window. It will try to follow you when "
				   "you navigate in the source directories"));
  
  topLevel()->embedOutputView(m_widget, i18n("Konsole"));
}


KonsoleViewPart::~KonsoleViewPart()
{
  delete m_widget;
}


#include "konsoleviewpart.moc"
