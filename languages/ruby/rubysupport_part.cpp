#include <qwhatsthis.h>
#include <qtimer.h>
#include <qfileinfo.h>


#include <kiconloader.h>
#include <klocale.h>
#include <kdevgenericfactory.h>
#include <kprocess.h>
#include <kdebug.h>
#include <kaction.h>
#include <kparts/part.h>
#include <kdialogbase.h>
#include <kapplication.h>
#include <qregexp.h>


#include "kdevcore.h"
#include "kdevmainwindow.h"
#include "kdevlanguagesupport.h"
#include "kdevpartcontroller.h"
#include "kdevproject.h"
#include "kdevappfrontend.h"

#include "rubysupport_part.h"
#include "rubyconfigwidget.h"
#include "domutil.h"

typedef KDevGenericFactory<RubySupportPart> RubySupportFactory;
static const KAboutData data("kdevrubysupport", I18N_NOOP("Language"), "1.0");
K_EXPORT_COMPONENT_FACTORY( libkdevrubysupport, RubySupportFactory( &data ) )

RubySupportPart::RubySupportPart(QObject *parent, const char *name, const QStringList& )
  : KDevLanguageSupport ("KDevPart", "kdevpart", parent, name ? name : "RubySupportPart" )
{
  setInstance(RubySupportFactory::instance());
  setXMLFile("kdevrubysupport.rc");

  KAction *action;
  action = new KAction( i18n("&Run"), "exec",Key_F9,this, SLOT(slotRun()),actionCollection(), "build_execute" );
  action->setToolTip(i18n("Run"));
  action->setWhatsThis(i18n("<b>Run</b><p>Starts an application."));
  action->setIcon("ruby_run.png");

  kdDebug() << "Creating RubySupportPart" << endl;

  connect( core(), SIGNAL(projectOpened()), this, SLOT(projectOpened()) );
  connect( partController(), SIGNAL(savedFile(const KURL&)),
  	this, SLOT(savedFile(const KURL&)) );
  connect( core(), SIGNAL(projectConfigWidget(KDialogBase*)),
             this, SLOT(projectConfigWidget(KDialogBase*)) );
}


RubySupportPart::~RubySupportPart() {
}


void RubySupportPart::projectConfigWidget(KDialogBase *dlg) 
{
    QVBox *vbox = dlg->addVBoxPage(i18n("Ruby"), i18n("Ruby"), BarIcon("ruby_config.png", KIcon::SizeMedium, KIcon::DefaultState, RubySupportPart::instance()));
    RubyConfigWidget *w = new RubyConfigWidget(*projectDom(), (QWidget *)vbox, "ruby config widget");
    connect( dlg, SIGNAL(okClicked()), w, SLOT(accept()) );
}

void RubySupportPart::projectOpened()
{
  kdDebug() << "projectOpened()" << endl;

  connect( project(), SIGNAL(addedFilesToProject(const QStringList &)),
  	this, SLOT(addedFilesToProject(const QStringList &)) );
  connect( project(), SIGNAL(removedFilesFromProject(const QStringList &)),
  	this, SLOT(removedFilesFromProject(const QStringList &)) );
  
  // We want to parse only after all components have been
  // properly initialized
  QTimer::singleShot(0, this, SLOT(initialParse()));
}

void RubySupportPart::maybeParse(const QString fileName)
{
  QFileInfo fi(fileName);

  if (fi.extension() == "rb") {
    if (codeModel()->hasFile(fileName)) {
      emit aboutToRemoveSourceInfo(fileName);
      codeModel()->removeFile(codeModel()->fileByName(fileName));
    }

    parse(fileName);
  }
}

void RubySupportPart::initialParse()
{
  kdDebug() << "initialParse()" << endl;

  if (project()) {
    kapp->setOverrideCursor(waitCursor);
    QStringList files = project()->allFiles();
    for (QStringList::Iterator it = files.begin(); it != files.end() ;++it) {
      kdDebug() << "maybe parse " << project()->projectDirectory() + "/" + (*it) << endl;
      maybeParse(project()->projectDirectory() + "/" + *it);
    }

    emit updatedSourceInfo();
    kapp->restoreOverrideCursor();
  } else {
    kdDebug() << "No project" << endl;
  }
}

void RubySupportPart::addedFilesToProject(const QStringList &fileList)
{
  kdDebug() << "addedFilesToProject()" << endl;

  QStringList::ConstIterator it;

  for ( it = fileList.begin(); it != fileList.end(); ++it )
  {
    QString fileName = project()->projectDirectory() + "/" + ( *it );
    maybeParse( fileName );
    emit addedSourceInfo( fileName );
  }
}

void RubySupportPart::removedFilesFromProject(const QStringList &fileList)
{
  kdDebug() << "removedFilesFromProject()" << endl;

  QStringList::ConstIterator it;

  for ( it = fileList.begin(); it != fileList.end(); ++it )
  {
    QString fileName = project()->projectDirectory() + "/" + ( *it );

    if( codeModel()->hasFile(fileName) ){
      emit aboutToRemoveSourceInfo( fileName );
      codeModel()->removeFile( codeModel()->fileByName(fileName) );
    }
  }
}

void RubySupportPart::savedFile(const KURL &fileName)
{
  kdDebug() << "savedFile()" << endl;

  if (project()->allFiles().contains(fileName.path().mid ( project()->projectDirectory().length() + 1 ))) {
    maybeParse(fileName.path());
    emit addedSourceInfo( fileName.path() );
  }
}

KDevLanguageSupport::Features RubySupportPart::features()
{
  return Features(Classes | Functions | Declarations | Signals | Slots);
}

void RubySupportPart::parse(const QString &fileName)
{
  QFile f(QFile::encodeName(fileName));
  if (!f.open(IO_ReadOnly))
    return;
  QTextStream stream(&f);

  QRegExp classre("^\\s*(class|module)\\s+([A-Z][A-Za-z0-9_]+)\\s*(<\\s*([A-Z][A-Za-z0-9_:]+))?$");
  QRegExp methodre("^\\s*def\\s+([A-Z][A-Za-z0-9_:]+\\.)?([A-Za-z0-9_]+[!?=]?|\\[\\]=?|\\*\\*||\\-|[!~+*/%&|><^]|>>|<<||<=>|<=|>=|==|===|!=|=~|!~).*$");
  QRegExp accessre("^\\s*(private|protected|public)\\s*((:([A-Za-z0-9_]+[!?=]?|\\[\\]=?|\\*\\*||\\-|[!~+*/%&|><^]|>>|<<||<=>|<=|>=|==|===|!=|=~|!~),?\\s*)*)$");
  QRegExp attr_accessorre("^\\s*(attr_accessor|attr_reader|attr_writer)\\s*((:([A-Za-z0-9_]+),?\\s*)*)$");
  QRegExp symbolre(":([^,]+),?");
  QRegExp line_contre(",\\s*$");
  QRegExp slot_signalre("^\\s*(slots|signals|k_dcop|k_dcop_signals)\\s*(('[^)]+\\)',?\\s*)*)$");
  QRegExp memberre("'([A-Za-z0-9_ &*]+\\s)?([A-Za-z0-9_]+)\\([^)]*\\)',?");
  QRegExp begin_commentre("^*=begin");
  QRegExp end_commentre("^*=end");
 
  FileDom m_file = codeModel()->create<FileModel>();
  m_file->setName(fileName);

  ClassDom lastClass;
  int lastAccess = CodeModelItem::Public;
  QString rawline;
  QCString line;
  int lineNo = 0;

  while (!stream.atEnd()) {
    rawline = stream.readLine();
    line = rawline.stripWhiteSpace().local8Bit();
    if (classre.search(line) != -1) {
      lastClass = codeModel()->create<ClassModel>();
      lastClass->setName(classre.cap(2));
      lastClass->setFileName( fileName );
      lastClass->setStartPosition( lineNo, 0 );

      QString parent = classre.cap(4);
      if (!parent.isEmpty())
      {
        kdDebug() << "Add parent " << parent << endl;
        lastClass->addBaseClass( parent );
      }

      if (m_file->hasClass(lastClass->name())) {
        ClassDom old = m_file->classByName( lastClass->name() )[ 0 ];
        old->setFileName( lastClass->fileName() );

        int line, col;
        lastClass->getStartPosition( &line, &col );
        old->setStartPosition( line, col );

        lastClass = old;
      } else {
          kdDebug() << "Add class " << lastClass->name() << endl;
          m_file->addClass( lastClass );
      }
	  lastAccess = CodeModelItem::Public;
    } else if (methodre.search(line) != -1) {
      FunctionDom methodDecl;
      if ( lastClass != 0 && lastClass->hasFunction( methodre.cap(2) ) ) {
        FunctionList methods = lastClass->functionByName( methodre.cap(2) );
	    methodDecl = methods[0];
	  } else {
        methodDecl = codeModel()->create<FunctionModel>();
        methodDecl->setFileName( fileName );
        methodDecl->setStartPosition( lineNo, 0 );
        methodDecl->setName(methodre.cap(2));
	  }
      FunctionDefinitionDom method = codeModel()->create<FunctionDefinitionModel>();
      method->setName(methodre.cap(2));
      kdDebug() << "Add method: " << method->name() << endl;
      method->setFileName( fileName );
      method->setStartPosition( lineNo, 0 );
	  if (methodDecl->name() == "initialize") {
	    // Ruby constructors are alway private
	    methodDecl->setAccess( CodeModelItem::Private );
	  } else {
	    methodDecl->setAccess( lastAccess );
	  }
	  if (methodre.cap(1) != "") {
	    // A ruby class/singleton method of the form <classname>.<methodname>
	  	methodDecl->setStatic( true );
	  }

      if (lastClass != 0 && rawline.left(3) != "def") {
		QStringList scope( lastClass->name() );
		method->setScope( scope );
		methodDecl->setScope( scope );
        if( !lastClass->hasFunction(methodDecl->name()) ) {
          lastClass->addFunction( methodDecl );
		}
        if( !lastClass->hasFunctionDefinition(method->name()) ) {
          lastClass->addFunctionDefinition( method );
		}
      } else if( !m_file->hasFunctionDefinition(method->name()) ){
        m_file->addFunction( methodDecl );
        m_file->addFunctionDefinition( method );
        lastClass = 0;
      }
    } else if (accessre.search(line) != -1 && lastClass != 0) {
	  int currentAccess = lastAccess;
	  if (accessre.cap(1) == "public") {
	    currentAccess = CodeModelItem::Public;
	  } else if (accessre.cap(1) == "protected") {
	    currentAccess = CodeModelItem::Protected;
	  } else if (accessre.cap(1) == "private") {
	    currentAccess = CodeModelItem::Private;
	  }
	  
	  if (accessre.cap(2) == "") {
	  	lastAccess = currentAccess;
	  } else {
		QString symbolList( accessre.cap(2) );
        int pos = 0;
		
        while ( pos >= 0 ) {
          pos = symbolre.search( symbolList, pos );
		  if (pos == -1) {
			if (line_contre.search(line) != -1) {
              rawline = stream.readLine();
			  if (!stream.atEnd()) {
                line = rawline.stripWhiteSpace().local8Bit();
                ++lineNo;
			    symbolList = line;
			    pos = 0;
			  }
			}
		  } else {
            if ( lastClass->hasFunction( symbolre.cap(1) ) ) {
              FunctionList methods = lastClass->functionByName( symbolre.cap(1) );
			  methods[0]->setAccess( currentAccess );
			}
            pos += symbolre.matchedLength();
          }
        }
	  }	  
    } else if (slot_signalre.search(line) != -1 && lastClass != 0) {
      QString memberList( slot_signalre.cap(2) );
      int pos = 0;
		
      while ( pos >= 0 ) {
        pos = memberre.search( memberList, pos );
		if (pos == -1) {
	      if (line_contre.search(line) != -1) {
            rawline = stream.readLine();
			if (!stream.atEnd()) {
              line = rawline.stripWhiteSpace().local8Bit();
              ++lineNo;
			  memberList = line;
			  pos = 0;
			}
		  }
		} else {
          FunctionDom method;
          if ( lastClass->hasFunction( memberre.cap(2) ) ) {
            FunctionList methods = lastClass->functionByName( memberre.cap(2) );
	        method = methods[0];
	      } else {
            method = codeModel()->create<FunctionModel>();
		  }
		  QStringList scope( lastClass->name() );
		  method->setScope( scope );
          method->setName(memberre.cap(2));
          method->setFileName( fileName );
          method->setStartPosition( lineNo, 0 );
			 
		  if (slot_signalre.cap(1) == "slots" || slot_signalre.cap(1) == "k_dcop") {
		    method->setSlot( true );
		  } else {
		    method->setSignal( true );
		  }
          if ( !lastClass->hasFunction(method->name()) ) {
            lastClass->addFunction( method );
		  }
          pos += memberre.matchedLength();
        }
	  }
	} else if (attr_accessorre.search(line) != -1 && lastClass != 0) {
	  QString attr( attr_accessorre.cap(1) );
	  QString symbolList( attr_accessorre.cap(2) );
      int pos = 0;
		
      while ( pos >= 0 ) {
        pos = symbolre.search( symbolList, pos );
		if (pos == -1) {
		  if (line_contre.search(line) != -1) {
            rawline = stream.readLine();
			if (!stream.atEnd()) {
              line = rawline.stripWhiteSpace().local8Bit();
              ++lineNo;
			  symbolList = line;
			  pos = 0;
			}
		  }
		} else {
          if ( !lastClass->hasFunction(symbolre.cap(1)) ) { 
		    QStringList scope( lastClass->name() );
			if (attr == "attr_accessor" || attr == "attr_reader") {
              FunctionDefinitionDom method = codeModel()->create<FunctionDefinitionModel>();
              method->setName(symbolre.cap(1));
              kdDebug() << "Add method: " << method->name() << endl;
              method->setFileName( fileName );
              method->setStartPosition( lineNo, 0 );
			  method->setScope(scope);
              lastClass->addFunction( model_cast<FunctionDom>(method) );
              lastClass->addFunctionDefinition( method );
			}
			
			if (attr == "attr_accessor" || attr == "attr_writer") {
              FunctionDefinitionDom method = codeModel()->create<FunctionDefinitionModel>();
              method->setName(symbolre.cap(1) + "=");
              kdDebug() << "Add method: " << method->name() << endl;
              method->setFileName( fileName );
              method->setStartPosition( lineNo, 0 );
			  method->setScope(scope);
              lastClass->addFunction( model_cast<FunctionDom>(method) );
              lastClass->addFunctionDefinition( method );
			}
			
            pos  += symbolre.matchedLength();
		  }
        }
	  }	  
   } else if (begin_commentre.search(line) != -1) {
     while (!stream.atEnd() && end_commentre.search(line) == -1) {
       rawline = stream.readLine();
       line = rawline.stripWhiteSpace().local8Bit();
       ++lineNo;
	 }
   }

    ++lineNo;
  }

  f.close();

  codeModel()->addFile( m_file );
}


void RubySupportPart::slotRun () {
	QFileInfo program(project()->mainProgram());
    QString cmd = QString("%1 -C%2 %3").arg(interpreter()).arg(program.dirPath()).arg(program.fileName());
    startApplication(cmd);
}

QString RubySupportPart::interpreter() {
    QString prog = DomUtil::readEntry(*projectDom(), "/kdevrubysupport/run/interpreter");
    if (prog.isEmpty()) prog = "ruby";
    return prog;
}


void RubySupportPart::startApplication(const QString &program) {
	bool inTerminal = DomUtil::readBoolEntry(*projectDom(), "/kdevrubysupport/run/terminal");
    appFrontend()->startAppCommand(QString::QString(), program, inTerminal);
}


KMimeType::List RubySupportPart::mimeTypes( )
{
    KMimeType::List list;
    KMimeType::Ptr mime = KMimeType::mimeType( "text/x-ruby" );
    if( mime )
	list << mime;
    return list;
}

#include "rubysupport_part.moc"
