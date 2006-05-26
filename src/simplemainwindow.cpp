/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "simplemainwindow.h"

#include <qtextedit.h>

#include <kaction.h>
#include <kstdaction.h>
#include <kparts/part.h>
#include <kpopupmenu.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kedittoolbar.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <kapplication.h>

#include <ddockwindow.h>
#include <dtabwidget.h>
#include <profile.h>
#include <profileengine.h>
#include <designer.h>

#include "api.h"
#include "core.h"
#include "plugincontroller.h"
#include "mainwindowshare.h"
#include "shellextension.h"
#include "partcontroller.h"
#include "statusbar.h"
#include "documentationpart.h"
#include "toplevel.h"
#include "projectmanager.h"
#include "editorproxy.h"

SimpleMainWindow::SimpleMainWindow(QWidget* parent, const char *name)
    :DMainWindow(parent, name)
{
    resize(800, 600); // starts kdevelop at 800x600 the first time
    m_mainWindowShare = new MainWindowShare(this);

    connect(m_bottomDock, SIGNAL(hidden()), this, SLOT(raiseEditor()));
    connect(m_leftDock, SIGNAL(hidden()), this, SLOT(raiseEditor()));
    connect(m_rightDock, SIGNAL(hidden()), this, SLOT(raiseEditor()));
}

SimpleMainWindow::~ SimpleMainWindow( )
{
    TopLevel::invalidateInstance( this );
}

void SimpleMainWindow::init()
{
    actionCollection()->setHighlightingEnabled( true );
    setStandardToolBarMenuEnabled( true );
    setXMLFile(ShellExtension::getInstance()->xmlFile());

    createFramework();
    createActions();
    new KDevStatusBar(this);

    createGUI(0);

    m_mainWindowShare->init();
    setupWindowMenu();
    menuBar()->setEnabled( false );

    //FIXME: this checks only for global offers which is not quite correct because
    //a profile can offer core plugins and no global plugins.
    if ( PluginController::getInstance()->engine().allOffers(ProfileEngine::Global).isEmpty() )
    {
        KMessageBox::sorry( this, i18n("Unable to find plugins, KDevelop will not work"
            " properly.\nPlease make sure "
            "that KDevelop is installed in your KDE directory; otherwise, you have "
            "to add KDevelop's installation "
            "path to the environment variable KDEDIRS and run kbuildsycoca. Restart "
            "KDevelop afterwards.\n"
            "Example for BASH users:\nexport KDEDIRS=/path/to/kdevelop:$KDEDIRS && kbuildsycoca"),
            i18n("Could Not Find Plugins") );
    }

    connect(Core::getInstance(), SIGNAL(coreInitialized()), this, SLOT(slotCoreInitialized()));
    connect(Core::getInstance(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(Core::getInstance(), SIGNAL(contextMenu(QPopupMenu *, const Context *)),
        this, SLOT(contextMenu(QPopupMenu *, const Context *)));
    connect(PartController::getInstance(), SIGNAL(partURLChanged(KParts::ReadOnlyPart *)),
        this, SLOT(slotPartURLChanged(KParts::ReadOnlyPart * )));
    connect(PartController::getInstance(), SIGNAL(activePartChanged(KParts::Part*)),
        this, SLOT(activePartChanged(KParts::Part*)));

    connect(PartController::getInstance(),
        SIGNAL(documentChangedState(const KURL &, DocumentState)),
        this, SLOT(documentChangedState(const KURL&, DocumentState)));

    loadSettings();
}

void SimpleMainWindow::contextMenu(QPopupMenu *popupMenu, const Context *context)
{
    int cont = context->type();
    m_splitURLs.clear();
    if (cont == Context::EditorContext)
    {
        KURL url = static_cast<const EditorContext*>(context)->url();
        QWidget *w = widgetForURL(url);
        if (w && m_widgetTabs[w] && m_widgetTabs[w]->count() > 1)
        {
            m_splitURLs.append(url);
            m_splitHor1->plug(popupMenu);
            m_splitVer1->plug(popupMenu);
            popupMenu->insertSeparator();
        }
    }
    else if (cont == Context::FileContext)
    {
        if (PartController::getInstance()->openURLs().count() > 0)
        {
            m_splitURLs = static_cast<const FileContext*>(context)->urls();
            bool isOpen = true;
            for (KURL::List::const_iterator it = m_splitURLs.begin(); it != m_splitURLs.end(); ++it)
            {
                if (!PartController::getInstance()->openURLs().contains(*it))
                {
                    isOpen = false;
                    break;
                }
            }
            if (isOpen && PartController::getInstance()->openURLs().count() == 1)
                return;
            popupMenu->insertSeparator();
            if (isOpen)
            {
                m_splitHor1->plug(popupMenu);
                m_splitVer1->plug(popupMenu);
            }
            else
            {
                m_splitHor2->plug(popupMenu);
                m_splitVer2->plug(popupMenu);
            }
        }
    }
}

void SimpleMainWindow::embedPartView(QWidget *view, const QString &title, const QString &/*toolTip*/)
{
    kdDebug() << "SimpleMainWindow::embedPartView: " << view << endl;
    if (!view )
        return;

    QString shortName = title;
    shortName = shortName.right( shortName.length() - (shortName.findRev('/') +1));

    addWidget(view, title);
    view->show();
}

void SimpleMainWindow::embedSelectView(QWidget *view, const QString &title, const QString &/*toolTip*/)
{
    toolWindow(DDockWindow::Left)->addWidget(title, view);
    m_docks[view] = DDockWindow::Left;
}

void SimpleMainWindow::embedOutputView(QWidget *view, const QString &title, const QString &/*toolTip*/)
{
    toolWindow(DDockWindow::Bottom)->addWidget(title, view);
    m_docks[view] = DDockWindow::Bottom;
}

void SimpleMainWindow::embedSelectViewRight(QWidget *view, const QString &title, const QString &/*toolTip*/)
{
    toolWindow(DDockWindow::Right)->addWidget(title, view);
    m_docks[view] = DDockWindow::Right;
}

void SimpleMainWindow::removeView(QWidget *view)
{
    if (!view)
        return;

    //try to remove it from all parts of main window
    //@fixme This method needs to be divided in two - one for docks and one for part views
    if (m_docks.contains(view))
        toolWindow(m_docks[view])->removeWidget(view);
    else
        removeWidget(view);
}

void SimpleMainWindow::setViewAvailable(QWidget *pView, bool bEnabled)
{
    DDockWindow *dock;
    if (m_docks.contains(pView))
        dock = toolWindow(m_docks[pView]);
    else
        return;

    bEnabled ? dock->showWidget(pView) : dock->hideWidget(pView);
}

void SimpleMainWindow::raiseView(QWidget *view)
{
    //adymo: a workaround to make editor wrappers work:
    //editor view is passed to this function but the ui library knows only
    //of its parent which is an editor wrapper, simply replacing the view
    //by its wrapper helps here
    if (view->parent())
        kdDebug() << view->parent()->className() << endl;
    if (view->parent() && (view->parent()->isA("EditorWrapper") || view->parent()->isA("MultiBuffer")))
    {
//         kdDebug() << "parent is editor wrapper: " <<
//             static_cast<EditorWrapper*>(view->parent()) << endl;
        view = (QWidget*)view->parent();
    }

    if (m_docks.contains(view))
    {
        DDockWindow *dock = toolWindow(m_docks[view]);
        dock->raiseWidget(view);
    }
    else if (m_widgets.contains(view) && m_widgetTabs.contains(view))
        m_widgetTabs[view]->showPage(view);
}

void SimpleMainWindow::lowerView(QWidget */*view*/)
{
    //nothing to do
}

void SimpleMainWindow::loadSettings()
{
    KConfig *config = kapp->config();

    ProjectManager::getInstance()->loadSettings();
    applyMainWindowSettings(config, "SimpleMainWindow");
}

void SimpleMainWindow::saveSettings( )
{
    KConfig *config = kapp->config();

    ProjectManager::getInstance()->saveSettings();
    saveMainWindowSettings(config, "SimpleMainWindow");

    DMainWindow::saveSettings();
}

void SimpleMainWindow::setCurrentDocumentCaption( const QString &caption )
{}

KMainWindow *SimpleMainWindow::main()
{
    return this;
}

void SimpleMainWindow::createFramework()
{
    PartController::createInstance( this );

    connect(PartController::getInstance(), SIGNAL(activePartChanged(KParts::Part*)),
        this, SLOT(createGUI(KParts::Part*)));
}

void SimpleMainWindow::createActions()
{
    m_raiseEditor = new KAction(i18n("Raise &Editor"), ALT+Key_C,
        this, SLOT(raiseEditor()), actionCollection(), "raise_editor");
    m_raiseEditor->setToolTip(i18n("Raise editor"));
    m_raiseEditor->setWhatsThis(i18n("<b>Raise editor</b><p>Focuses the editor."));

    m_splitHor = new KAction(i18n("Split &Horizontal"), CTRL+SHIFT+Key_T,
        this, SLOT(slotSplitHorizontalBase()), actionCollection(), "split_h");

    m_splitVer = new KAction(i18n("Split &Vertical"), CTRL+SHIFT+Key_L,
        this, SLOT(slotSplitVerticalBase()), actionCollection(), "split_v");

    m_splitHor1 = new KAction(i18n("Split &Horizontal"), 0,
        this, SLOT(slotSplitHorizontal()), actionCollection(), "split_h1");

    m_splitVer1 = new KAction(i18n("Split &Vertical"), 0,
        this, SLOT(slotSplitVertical()), actionCollection(), "split_v1");

    m_splitHor2 = new KAction(i18n("Split &Horizontal and Open"), 0,
        this, SLOT(slotSplitHorizontal()), actionCollection(), "split_h2");

    m_splitVer2 = new KAction(i18n("Split &Vertical and Open"), 0,
        this, SLOT(slotSplitVertical()), actionCollection(), "split_v2");

    m_raiseLeftDock = new KAction(i18n("Switch Left Dock"), CTRL+SHIFT+ALT+Key_L,
        this, SLOT(raiseLeftDock()), actionCollection(), "switch_left_dock");

    m_raiseRightDock = new KAction(i18n("Switch Right Dock"), CTRL+SHIFT+ALT+Key_R,
        this, SLOT(raiseRightDock()), actionCollection(), "switch_right_dock");

    m_raiseBottomDock = new KAction(i18n("Switch Bottom Dock"), CTRL+SHIFT+ALT+Key_B,
        this, SLOT(raiseBottomDock()), actionCollection(), "switch_bottom_dock");

    KStdAction::configureToolbars(this, SLOT(configureToolbars()),
        actionCollection(), "set_configure_toolbars");

    m_mainWindowShare->createActions();

    connect(m_mainWindowShare, SIGNAL(gotoNextWindow()), this, SLOT(gotoNextWindow()));
    connect(m_mainWindowShare, SIGNAL(gotoPreviousWindow()), this, SLOT(gotoPreviousWindow()));
    connect(m_mainWindowShare, SIGNAL(gotoFirstWindow()), this, SLOT(gotoFirstWindow()));
    connect(m_mainWindowShare, SIGNAL(gotoLastWindow()), this, SLOT(gotoLastWindow()));
}

void SimpleMainWindow::raiseEditor()
{
    kdDebug() << "SimpleMainWindow::raiseEditor" << endl;
    KDevPartController *partcontroller = API::getInstance()->partController();
    if (partcontroller->activePart() && partcontroller->activePart()->widget())
        partcontroller->activePart()->widget()->setFocus();
}

void SimpleMainWindow::gotoNextWindow()
{
    if (m_bottomDock->isActive())
        m_bottomDock->selectNextWidget();
    else if (m_rightDock->isActive())
        m_rightDock->selectNextWidget();
    else if (m_leftDock->isActive())
        m_leftDock->selectNextWidget();
    else
    {
        if ((m_activeTabWidget->currentPageIndex() + 1) < m_activeTabWidget->count())
            m_activeTabWidget->setCurrentPage(m_activeTabWidget->currentPageIndex() + 1);
        else
            m_activeTabWidget->setCurrentPage(0);
    }
}

void SimpleMainWindow::gotoPreviousWindow()
{
    if (m_bottomDock->isActive())
        m_bottomDock->selectPrevWidget();
    else if (m_rightDock->isActive())
        m_rightDock->selectPrevWidget();
    else if (m_leftDock->isActive())
        m_leftDock->selectPrevWidget();
    else
    {
        if ((m_activeTabWidget->currentPageIndex() - 1) >= 0)
            m_activeTabWidget->setCurrentPage(m_activeTabWidget->currentPageIndex() - 1);
        else
            m_activeTabWidget->setCurrentPage(m_activeTabWidget->count() - 1);
    }
}

void SimpleMainWindow::gotoFirstWindow()
{
    //@todo implement
}

void SimpleMainWindow::gotoLastWindow()
{
    //@todo implement
}

void SimpleMainWindow::slotCoreInitialized()
{
    menuBar()->setEnabled(true);
}

void SimpleMainWindow::projectOpened()
{
    setCaption(QString::null);
}

void SimpleMainWindow::slotPartURLChanged(KParts::ReadOnlyPart *part)
{
    if (QWidget *widget = EditorProxy::getInstance()->topWidgetForPart(part))
        widget->setCaption(part->url().fileName());
        //do smth with caption: ro_part->url().fileName()
}

void SimpleMainWindow::documentChangedState(const KURL &url, DocumentState state)
{
    QWidget * widget = EditorProxy::getInstance()->topWidgetForPart(
        PartController::getInstance()->partForURL(url));
    kdDebug() << "SimpleMainWindow::documentChangedState: " << widget << endl;
    if (widget)
    {
        //calculate the icon size if showTabIcons is false
        //this is necessary to avoid tab resizing by setIcon() call
        int isize = 16;
        if (m_activeTabWidget && !m_showIconsOnTabs)
        {
            isize = m_activeTabWidget->fontMetrics().height() - 1;
            isize = isize > 16 ? 16 : isize;
        }
        switch (state)
        {
            // we should probably restore the original icon instead of just using "kdevelop",
            // but I have never seen any other icon in use so this should do for now
            case Clean:
                if (m_showIconsOnTabs)
                    widget->setIcon(SmallIcon("kdevelop", isize));
                else
                    widget->setIcon(QPixmap());
                break;
            case Modified:
                widget->setIcon(SmallIcon("filesave", isize));
                break;
            case Dirty:
                widget->setIcon(SmallIcon("revert", isize));
                break;
            case DirtyAndModified:
                widget->setIcon(SmallIcon("stop", isize));
                break;
        }
    }
}

void SimpleMainWindow::closeTab()
{
//    actionCollection()->action("file_close")->activate();
    if (sender()->isA("QToolButton") && sender()->parent()->isA("DTabWidget"))
    {
        DTabWidget *tab = (DTabWidget*)sender()->parent();
        if (tab && tab->currentPage())
            closeTab(tab->currentPage());
    }
}

void SimpleMainWindow::tabContext(QWidget *w, const QPoint &p)
{
    DTabWidget *tabWidget = static_cast<DTabWidget*>(const_cast<QObject*>(sender()));
    if (!tabWidget)
        return;

    KPopupMenu tabMenu;
    tabMenu.insertTitle(tabWidget->tabLabel(w));

    //Find the document on whose tab the user clicked
    m_currentTabURL = QString::null;
    QPtrListIterator<KParts::Part> it(*PartController::getInstance()->parts());
    while (KParts::Part* part = it.current())
    {
        QWidget *top_widget = EditorProxy::getInstance()->topWidgetForPart(part);
        if (top_widget == w)
        {
            if (KParts::ReadOnlyPart *ro_part = dynamic_cast<KParts::ReadOnlyPart*>(part))
            {
                m_currentTabURL = ro_part->url();
                tabMenu.insertItem(i18n("Close"), 0);

                if (PartController::getInstance()->parts()->count() > 1)
                    tabMenu.insertItem(i18n("Close All Others"), 4);

                if (dynamic_cast<HTMLDocumentationPart*>(ro_part))
                {
                    tabMenu.insertItem(i18n("Duplicate"), 3);
                    break;
                }

                //Create the file context
                KURL::List list;
                list << m_currentTabURL;
                FileContext context( list );
                Core::getInstance()->fillContextMenu(&tabMenu, &context);
            }
            break;
        }
        ++it;
    }

    connect(&tabMenu, SIGNAL(activated(int)), this, SLOT(tabContextActivated(int)));
    tabMenu.exec(p);
}

void SimpleMainWindow::tabContextActivated(int id)
{
    if(m_currentTabURL.isEmpty())
            return;

    switch(id)
    {
        case 0:
            PartController::getInstance()->closeFile(m_currentTabURL);
            break;
        case 1:
            PartController::getInstance()->saveFile(m_currentTabURL);
            break;
        case 2:
            PartController::getInstance()->reloadFile(m_currentTabURL);
            break;
        case 3:
            PartController::getInstance()->showDocument(m_currentTabURL, true);
            break;
        case 4:
            PartController::getInstance()->closeAllOthers(m_currentTabURL);
            break;
        default:
            break;
    }
}

void SimpleMainWindow::configureToolbars()
{
    saveMainWindowSettings(KGlobal::config(), "SimpleMainWindow");
    KEditToolbar dlg(factory());
    connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(slotNewToolbarConfig()));
    dlg.exec();
}

void SimpleMainWindow::slotNewToolbarConfig()
{
//    setupWindowMenu();
    m_mainWindowShare->slotGUICreated(PartController::getInstance()->activePart());
    applyMainWindowSettings(KGlobal::config(), "SimpleMainWindow");
}

bool SimpleMainWindow::queryClose()
{
    saveSettings();
    return Core::getInstance()->queryClose();
}

bool SimpleMainWindow::queryExit()
{
    return true;
}

void SimpleMainWindow::setupWindowMenu()
{
    // get the xmlgui created one instead
    m_windowMenu = static_cast<QPopupMenu*>(main()->child("window", "KPopupMenu"));

    if (!m_windowMenu)
    {
        kdDebug(9000) << "Couldn't find the XMLGUI window menu. Creating new." << endl;

        m_windowMenu = new QPopupMenu(main(), "window");
        menuBar()->insertItem(i18n("&Window"), m_windowMenu);
    }

    actionCollection()->action("file_close")->plug(m_windowMenu);
    actionCollection()->action("file_close_all")->plug(m_windowMenu);
    actionCollection()->action("file_closeother")->plug(m_windowMenu);

    QObject::connect(m_windowMenu, SIGNAL(activated(int)), this, SLOT(openURL(int)));
    QObject::connect(m_windowMenu, SIGNAL(aboutToShow()), this, SLOT(fillWindowMenu()));
}

void SimpleMainWindow::openURL(int w)
{
    QValueList<QPair<int, KURL> >::ConstIterator it = m_windowList.begin();
    while (it != m_windowList.end())
    {
        if ((*it).first == w)
        {
            KURL url((*it).second);
            if (!url.isEmpty())
            {
                PartController::getInstance()->editDocument(url);
                return;
            }
        }
        ++it;
    }
}

void SimpleMainWindow::fillWindowMenu()
{
    // clear menu
    QValueList< QPair< int, KURL > >::ConstIterator it = m_windowList.begin();
    while (it != m_windowList.end())
    {
        m_windowMenu->removeItem( (*it).first );
        ++it;
    }

    int temp = 0;

    QMap<QString, KURL> map;
    QStringList string_list;
    KURL::List list = PartController::getInstance()->openURLs();
    KURL::List::Iterator itt = list.begin();
    while (itt != list.end())
    {
        map[(*itt).fileName()] = *itt;
        string_list.append((*itt).fileName());
        ++itt;
    }
    string_list.sort();

    list.clear();
    for(uint i = 0; i != string_list.size(); ++i)
        list.append(map[string_list[i]]);

    itt = list.begin();
    int i = 0;

    if (list.count() > 0)
        m_windowList << qMakePair(m_windowMenu->insertSeparator(), KURL());

    while (itt != list.end())
    {
        temp = m_windowMenu->insertItem( i < 10 ? QString("&%1 %2").arg(i).arg((*itt).fileName()) : (*itt).fileName() );
        m_windowList << qMakePair(temp, *itt);
        ++i;
        ++itt;
    }
}

void SimpleMainWindow::slotSplitVertical()
{
    DTabWidget *tab = splitVertical();
    openDocumentsAfterSplit(tab);
}

void SimpleMainWindow::slotSplitHorizontal()
{
    DTabWidget *tab = splitHorizontal();
    openDocumentsAfterSplit(tab);
}

void SimpleMainWindow::slotSplitVerticalBase()
{
    if (KParts::ReadOnlyPart *ro_part = activePartForSplitting())
    {
        m_splitURLs << ro_part->url();
        slotSplitVertical();
    }
}

KParts::ReadOnlyPart *SimpleMainWindow::activePartForSplitting()
{
    if (PartController::getInstance()->openURLs().count() < 2)
        return 0;
    m_splitURLs.clear();
    KParts::ReadOnlyPart *ro_part =
        dynamic_cast<KParts::ReadOnlyPart*>(PartController::getInstance()->activePart());
    return ro_part;
}

void SimpleMainWindow::slotSplitHorizontalBase()
{
    if (KParts::ReadOnlyPart *ro_part = activePartForSplitting())
    {
        m_splitURLs << ro_part->url();
        slotSplitHorizontal();
    }
}

void SimpleMainWindow::openDocumentsAfterSplit(DTabWidget *tab)
{
    if (m_splitURLs.count() > 0)
    {
        for (KURL::List::const_iterator it = m_splitURLs.begin(); it != m_splitURLs.end(); ++it)
        {
            KParts::ReadOnlyPart *part = PartController::getInstance()->partForURL(*it);
            if (!part)
                PartController::getInstance()->editDocument(*it);
            else
            {
                QWidget *inTab = widgetForURL(*it);
                if (inTab)
                {
                    DTabWidget *oldTab = m_widgetTabs[inTab];
                    QString title = oldTab->tabLabel(inTab);
                    removeWidget(inTab);
                    addWidget(tab, inTab, title);
                }
            }
        }
        m_splitURLs.clear();
    }
}

QWidget *SimpleMainWindow::widgetForURL(KURL url)
{
    KParts::ReadOnlyPart *part = PartController::getInstance()->partForURL(url);
    return widgetInTab(part->widget());
}

QWidget *SimpleMainWindow::widgetInTab(QWidget *w)
{
    QWidget *inTab = 0;
    if (w && w->parent() && w->parent()->isA("EditorProxy"))
        inTab = (QWidget*)w->parent();
    else if (w && w->parent() && w->parent()->isA("MultiBuffer")
        && w->parent()->parent() && w->parent()->parent()->isA("EditorProxy"))
        inTab = (QWidget*)w->parent()->parent();
    else if (w && w->parent() && w->parent()->isA("MultiBuffer"))
        inTab = (QWidget*)w->parent();
    else
        inTab = w;
    return inTab;
}

void SimpleMainWindow::closeTab(QWidget *w)
{
    const QPtrList<KParts::Part> *partlist = PartController::getInstance()->parts();
    QPtrListIterator<KParts::Part> it(*partlist);
    while (KParts::Part* part = it.current())
    {
        QWidget *widget = EditorProxy::getInstance()->topWidgetForPart(part);
        if (widget && widget == w)
        {
            PartController::getInstance()->closePart(part);
            return;
        }
        ++it;
    }
}

void SimpleMainWindow::activePartChanged(KParts::Part *part)
{
    if (!part)
        return;
    QWidget *w = part->widget();
    kdDebug() << "active part widget is : " << w << endl;
    QWidget *inTab = widgetInTab(w);
    if (m_widgetTabs[inTab] != 0)
    {
        kdDebug() << " setting m_activeTabWidget " << endl;
        m_activeTabWidget = m_widgetTabs[inTab];
    }
}

void SimpleMainWindow::createGUI(KParts::Part *part) {
    if ( !part )
        setCaption( QString::null );
    DMainWindow::createGUI(part);

    m_mainWindowShare->slotGUICreated( part );
}

void SimpleMainWindow::raiseBottomDock()
{
    raiseDock(m_bottomDock);
}

void SimpleMainWindow::raiseLeftDock()
{
    raiseDock(m_leftDock);
}

void SimpleMainWindow::raiseRightDock()
{
    raiseDock(m_rightDock);
}

void SimpleMainWindow::raiseDock(DDockWindow *dock)
{
    dock->selectLastWidget();
}

#include "simplemainwindow.moc"
