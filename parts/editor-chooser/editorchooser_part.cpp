#include <qvbox.h>


#include <kiconloader.h>
#include <klocale.h>
#include <kdialogbase.h>
#include <kgenericfactory.h>

#include <kdevcore.h>


#include "editorchooser_part.h"
#include "editorchooser_widget.h"

typedef KGenericFactory<EditorChooserPart> EditorChooserFactory;
K_EXPORT_COMPONENT_FACTORY( libkdeveditorchooser, EditorChooserFactory( "kdeveditorchooser" ) )

EditorChooserPart::EditorChooserPart(QObject *parent, const char *name, const QStringList &)
  : KDevPlugin("EditorChooser", "editorchooser", parent, name ? name : "EditorChooserPart")
{
  setInstance(EditorChooserFactory::instance());

  connect(core(), SIGNAL(configWidget(KDialogBase*)), this, SLOT(configWidget(KDialogBase*)));
}


EditorChooserPart::~EditorChooserPart()
{
}


void EditorChooserPart::configWidget(KDialogBase *dlg)
{
  QVBox *vbox = dlg->addVBoxPage(i18n("Editor"));
  EditorChooserWidget *w = new EditorChooserWidget(vbox);
  connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
}



#include "editorchooser_part.moc"
