/****************************************************************************
** CKDevelop meta object code from reading C++ file 'ckdevelop.h'
**
** Created: Sun Jan 31 17:19:14 1999
**      by: The Qt Meta Object Compiler ($Revision$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "ckdevelop.h"
#include <qmetaobject.h>


const char *CKDevelop::className() const
{
    return "CKDevelop";
}

QMetaObject *CKDevelop::metaObj = 0;


#if QT_VERSION >= 200
static QMetaObjectInit init_CKDevelop(&CKDevelop::staticMetaObject);

#endif

void CKDevelop::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(KTMainWindow::className(), "KTMainWindow") != 0 )
	badSuperclassWarning("CKDevelop","KTMainWindow");

#if QT_VERSION >= 200
    staticMetaObject();
}

void CKDevelop::staticMetaObject()
{
    if ( metaObj )
	return;
    KTMainWindow::staticMetaObject();
#else

    KTMainWindow::initMetaObject();
#endif

    typedef void(CKDevelop::*m1_t0)();
    typedef void(CKDevelop::*m1_t1)();
    typedef void(CKDevelop::*m1_t2)();
    typedef void(CKDevelop::*m1_t3)();
    typedef void(CKDevelop::*m1_t4)();
    typedef void(CKDevelop::*m1_t5)();
    typedef void(CKDevelop::*m1_t6)();
    typedef void(CKDevelop::*m1_t7)();
    typedef void(CKDevelop::*m1_t8)();
    typedef void(CKDevelop::*m1_t9)();
    typedef void(CKDevelop::*m1_t10)();
    typedef void(CKDevelop::*m1_t11)();
    typedef void(CKDevelop::*m1_t12)();
    typedef void(CKDevelop::*m1_t13)();
    typedef void(CKDevelop::*m1_t14)();
    typedef void(CKDevelop::*m1_t15)();
    typedef void(CKDevelop::*m1_t16)();
    typedef void(CKDevelop::*m1_t17)();
    typedef void(CKDevelop::*m1_t18)();
    typedef void(CKDevelop::*m1_t19)();
    typedef void(CKDevelop::*m1_t20)();
    typedef void(CKDevelop::*m1_t21)();
    typedef void(CKDevelop::*m1_t22)();
    typedef void(CKDevelop::*m1_t23)();
    typedef void(CKDevelop::*m1_t24)();
    typedef void(CKDevelop::*m1_t25)();
    typedef void(CKDevelop::*m1_t26)();
    typedef void(CKDevelop::*m1_t27)();
    typedef void(CKDevelop::*m1_t28)();
    typedef void(CKDevelop::*m1_t29)();
    typedef void(CKDevelop::*m1_t30)();
    typedef void(CKDevelop::*m1_t31)();
    typedef void(CKDevelop::*m1_t32)();
    typedef void(CKDevelop::*m1_t33)();
    typedef void(CKDevelop::*m1_t34)();
    typedef void(CKDevelop::*m1_t35)();
    typedef void(CKDevelop::*m1_t36)();
    typedef void(CKDevelop::*m1_t37)();
    typedef void(CKDevelop::*m1_t38)();
    typedef void(CKDevelop::*m1_t39)();
    typedef void(CKDevelop::*m1_t40)();
    typedef void(CKDevelop::*m1_t41)();
    typedef void(CKDevelop::*m1_t42)();
    typedef void(CKDevelop::*m1_t43)();
    typedef void(CKDevelop::*m1_t44)();
    typedef void(CKDevelop::*m1_t45)();
    typedef void(CKDevelop::*m1_t46)();
    typedef void(CKDevelop::*m1_t47)();
    typedef void(CKDevelop::*m1_t48)();
    typedef void(CKDevelop::*m1_t49)();
    typedef void(CKDevelop::*m1_t50)();
    typedef void(CKDevelop::*m1_t51)();
    typedef void(CKDevelop::*m1_t52)();
    typedef void(CKDevelop::*m1_t53)();
    typedef void(CKDevelop::*m1_t54)();
    typedef void(CKDevelop::*m1_t55)();
    typedef void(CKDevelop::*m1_t56)();
    typedef void(CKDevelop::*m1_t57)();
    typedef void(CKDevelop::*m1_t58)();
    typedef void(CKDevelop::*m1_t59)();
    typedef void(CKDevelop::*m1_t60)();
    typedef void(CKDevelop::*m1_t61)();
    typedef void(CKDevelop::*m1_t62)(QString);
    typedef void(CKDevelop::*m1_t63)();
    typedef void(CKDevelop::*m1_t64)();
    typedef void(CKDevelop::*m1_t65)();
    typedef void(CKDevelop::*m1_t66)();
    typedef void(CKDevelop::*m1_t67)();
    typedef void(CKDevelop::*m1_t68)();
    typedef void(CKDevelop::*m1_t69)();
    typedef void(CKDevelop::*m1_t70)();
    typedef void(CKDevelop::*m1_t71)();
    typedef void(CKDevelop::*m1_t72)();
    typedef void(CKDevelop::*m1_t73)();
    typedef void(CKDevelop::*m1_t74)(int);
    typedef void(CKDevelop::*m1_t75)(KHTMLView*,const char*,int,const char*);
    typedef void(CKDevelop::*m1_t76)(KProcess*,char*,int);
    typedef void(CKDevelop::*m1_t77)(KProcess*,char*,int);
    typedef void(CKDevelop::*m1_t78)(KProcess*,char*,int);
    typedef void(CKDevelop::*m1_t79)(KHTMLView*);
    typedef void(CKDevelop::*m1_t80)(KProcess*);
    typedef void(CKDevelop::*m1_t81)(KProcess*);
    typedef void(CKDevelop::*m1_t82)(int);
    typedef void(CKDevelop::*m1_t83)(int);
    typedef void(CKDevelop::*m1_t84)(int);
    typedef void(CKDevelop::*m1_t85)(int);
    typedef void(CKDevelop::*m1_t86)(int);
    typedef void(CKDevelop::*m1_t87)(int);
    typedef void(CKDevelop::*m1_t88)(int);
    typedef void(CKDevelop::*m1_t89)(int);
    typedef void(CKDevelop::*m1_t90)();
    typedef void(CKDevelop::*m1_t91)(int);
    typedef void(CKDevelop::*m1_t92)(int);
    typedef void(CKDevelop::*m1_t93)(const char*);
    typedef void(CKDevelop::*m1_t94)(const char*);
    typedef void(CKDevelop::*m1_t95)(int);
    typedef void(CKDevelop::*m1_t96)();
    typedef void(CKDevelop::*m1_t97)();
    typedef void(CKDevelop::*m1_t98)();
    typedef int(CKDevelop::*m1_t99)(QString);
    typedef void(CKDevelop::*m1_t100)(QString,QString);
    typedef void(CKDevelop::*m1_t101)(QString,QString);
    typedef int(CKDevelop::*m1_t102)(QString);
    typedef QString(CKDevelop::*m1_t103)(QString);
    typedef QString(CKDevelop::*m1_t104)(QString);
    typedef void(CKDevelop::*m1_t105)();
    typedef void(CKDevelop::*m1_t106)();
    m1_t0 v1_0 = &CKDevelop::slotFileNewFile;
    m1_t1 v1_1 = &CKDevelop::slotFileOpenFile;
    m1_t2 v1_2 = &CKDevelop::slotFileSave;
    m1_t3 v1_3 = &CKDevelop::slotFileSaveAll;
    m1_t4 v1_4 = &CKDevelop::slotFileSaveAs;
    m1_t5 v1_5 = &CKDevelop::slotFileClose;
    m1_t6 v1_6 = &CKDevelop::slotFileCloseAll;
    m1_t7 v1_7 = &CKDevelop::slotFilePrint;
    m1_t8 v1_8 = &CKDevelop::slotFileQuit;
    m1_t9 v1_9 = &CKDevelop::slotEditUndo;
    m1_t10 v1_10 = &CKDevelop::slotEditRedo;
    m1_t11 v1_11 = &CKDevelop::slotEditCut;
    m1_t12 v1_12 = &CKDevelop::slotEditCopy;
    m1_t13 v1_13 = &CKDevelop::slotEditPaste;
    m1_t14 v1_14 = &CKDevelop::slotEditSelectAll;
    m1_t15 v1_15 = &CKDevelop::slotEditInvertSelection;
    m1_t16 v1_16 = &CKDevelop::slotEditDeselectAll;
    m1_t17 v1_17 = &CKDevelop::slotEditInsertFile;
    m1_t18 v1_18 = &CKDevelop::slotEditSearch;
    m1_t19 v1_19 = &CKDevelop::slotEditRepeatSearch;
    m1_t20 v1_20 = &CKDevelop::slotEditReplace;
    m1_t21 v1_21 = &CKDevelop::slotBookmarksAdd;
    m1_t22 v1_22 = &CKDevelop::slotBookmarksEdit;
    m1_t23 v1_23 = &CKDevelop::slotEditGotoLine;
    m1_t24 v1_24 = &CKDevelop::slotOptionsTTreeView;
    m1_t25 v1_25 = &CKDevelop::slotOptionsTOutputView;
    m1_t26 v1_26 = &CKDevelop::slotOptionsTStdToolbar;
    m1_t27 v1_27 = &CKDevelop::slotOptionsTBrowserToolbar;
    m1_t28 v1_28 = &CKDevelop::slotOptionsTStatusbar;
    m1_t29 v1_29 = &CKDevelop::slotOptionsRefresh;
    m1_t30 v1_30 = &CKDevelop::slotFileNewAppl;
    m1_t31 v1_31 = &CKDevelop::slotProjectNew;
    m1_t32 v1_32 = &CKDevelop::slotFileOpenPrj;
    m1_t33 v1_33 = &CKDevelop::slotProjectCompileFile;
    m1_t34 v1_34 = &CKDevelop::slotProjectAddNewFile;
    m1_t35 v1_35 = &CKDevelop::slotProjectAddExistingFiles;
    m1_t36 v1_36 = &CKDevelop::slotProjectRemoveFile;
    m1_t37 v1_37 = &CKDevelop::slotProjectNewClass;
    m1_t38 v1_38 = &CKDevelop::slotProjectFileProperties;
    m1_t39 v1_39 = &CKDevelop::slotProjectOptions;
    m1_t40 v1_40 = &CKDevelop::slotBuildRun;
    m1_t41 v1_41 = &CKDevelop::slotBuildDebug;
    m1_t42 v1_42 = &CKDevelop::slotBuildMake;
    m1_t43 v1_43 = &CKDevelop::slotBuildRebuildAll;
    m1_t44 v1_44 = &CKDevelop::slotBuildCleanRebuildAll;
    m1_t45 v1_45 = &CKDevelop::slotBuildDistClean;
    m1_t46 v1_46 = &CKDevelop::slotBuildAutoconf;
    m1_t47 v1_47 = &CKDevelop::slotBuildConfigure;
    m1_t48 v1_48 = &CKDevelop::slotBuildStop;
    m1_t49 v1_49 = &CKDevelop::slotBuildAPI;
    m1_t50 v1_50 = &CKDevelop::slotBuildManual;
    m1_t51 v1_51 = &CKDevelop::slotToolsKIconEdit;
    m1_t52 v1_52 = &CKDevelop::slotToolsKDbg;
    m1_t53 v1_53 = &CKDevelop::slotToolsKTranslator;
    m1_t54 v1_54 = &CKDevelop::slotOptionsEditor;
    m1_t55 v1_55 = &CKDevelop::slotOptionsEditorColors;
    m1_t56 v1_56 = &CKDevelop::slotOptionsSyntaxHighlighting;
    m1_t57 v1_57 = &CKDevelop::slotOptionsKDevelop;
    m1_t58 v1_58 = &CKDevelop::slotOptionsDocBrowser;
    m1_t59 v1_59 = &CKDevelop::slotDocBack;
    m1_t60 v1_60 = &CKDevelop::slotDocForward;
    m1_t61 v1_61 = &CKDevelop::slotDocSText;
    m1_t62 v1_62 = &CKDevelop::slotDocSText;
    m1_t63 v1_63 = &CKDevelop::slotDocQtLib;
    m1_t64 v1_64 = &CKDevelop::slotDocKDECoreLib;
    m1_t65 v1_65 = &CKDevelop::slotDocKDEGUILib;
    m1_t66 v1_66 = &CKDevelop::slotDocKDEKFileLib;
    m1_t67 v1_67 = &CKDevelop::slotDocKDEHTMLLib;
    m1_t68 v1_68 = &CKDevelop::slotDocAPI;
    m1_t69 v1_69 = &CKDevelop::slotDocManual;
    m1_t70 v1_70 = &CKDevelop::slotDocUpdateKDEDocumentation;
    m1_t71 v1_71 = &CKDevelop::slotHelpContent;
    m1_t72 v1_72 = &CKDevelop::slotHelpHomepage;
    m1_t73 v1_73 = &CKDevelop::slotHelpAbout;
    m1_t74 v1_74 = &CKDevelop::slotToolbarClicked;
    m1_t75 v1_75 = &CKDevelop::slotURLSelected;
    m1_t76 v1_76 = &CKDevelop::slotReceivedStdout;
    m1_t77 v1_77 = &CKDevelop::slotReceivedStderr;
    m1_t78 v1_78 = &CKDevelop::slotSearchReceivedStdout;
    m1_t79 v1_79 = &CKDevelop::slotDocumentDone;
    m1_t80 v1_80 = &CKDevelop::slotProcessExited;
    m1_t81 v1_81 = &CKDevelop::slotSearchProcessExited;
    m1_t82 v1_82 = &CKDevelop::slotLogFileTreeSelected;
    m1_t83 v1_83 = &CKDevelop::slotRealFileTreeSelected;
    m1_t84 v1_84 = &CKDevelop::slotDocTreeSelected;
    m1_t85 v1_85 = &CKDevelop::slotClassTreeSelected;
    m1_t86 v1_86 = &CKDevelop::slotCVViewDeclaration;
    m1_t87 v1_87 = &CKDevelop::slotCVViewDefinition;
    m1_t88 v1_88 = &CKDevelop::slotSTabSelected;
    m1_t89 v1_89 = &CKDevelop::slotMenuBuffersSelected;
    m1_t90 v1_90 = &CKDevelop::slotClickedOnOutputWidget;
    m1_t91 v1_91 = &CKDevelop::slotClassChoiceCombo;
    m1_t92 v1_92 = &CKDevelop::slotMethodChoiceCombo;
    m1_t93 v1_93 = &CKDevelop::slotStatusMsg;
    m1_t94 v1_94 = &CKDevelop::slotStatusHelpMsg;
    m1_t95 v1_95 = &CKDevelop::statusCallback;
    m1_t96 v1_96 = &CKDevelop::slotNewStatus;
    m1_t97 v1_97 = &CKDevelop::slotNewLineColumn;
    m1_t98 v1_98 = &CKDevelop::slotNewUndo;
    m1_t99 v1_99 = &CKDevelop::CVGotoClassDecl;
    m1_t100 v1_100 = &CKDevelop::CVGotoClassVarDecl;
    m1_t101 v1_101 = &CKDevelop::CVGotoMethodeImpl;
    m1_t102 v1_102 = &CKDevelop::searchToolGetNumber;
    m1_t103 v1_103 = &CKDevelop::searchToolGetTitle;
    m1_t104 v1_104 = &CKDevelop::searchToolGetURL;
    m1_t105 v1_105 = &CKDevelop::slotCreateSearchDatabase;
    m1_t106 v1_106 = &CKDevelop::refreshClassCombos;
    QMetaData *slot_tbl = new QMetaData[107];
    slot_tbl[0].name = "slotFileNewFile()";
    slot_tbl[1].name = "slotFileOpenFile()";
    slot_tbl[2].name = "slotFileSave()";
    slot_tbl[3].name = "slotFileSaveAll()";
    slot_tbl[4].name = "slotFileSaveAs()";
    slot_tbl[5].name = "slotFileClose()";
    slot_tbl[6].name = "slotFileCloseAll()";
    slot_tbl[7].name = "slotFilePrint()";
    slot_tbl[8].name = "slotFileQuit()";
    slot_tbl[9].name = "slotEditUndo()";
    slot_tbl[10].name = "slotEditRedo()";
    slot_tbl[11].name = "slotEditCut()";
    slot_tbl[12].name = "slotEditCopy()";
    slot_tbl[13].name = "slotEditPaste()";
    slot_tbl[14].name = "slotEditSelectAll()";
    slot_tbl[15].name = "slotEditInvertSelection()";
    slot_tbl[16].name = "slotEditDeselectAll()";
    slot_tbl[17].name = "slotEditInsertFile()";
    slot_tbl[18].name = "slotEditSearch()";
    slot_tbl[19].name = "slotEditRepeatSearch()";
    slot_tbl[20].name = "slotEditReplace()";
    slot_tbl[21].name = "slotBookmarksAdd()";
    slot_tbl[22].name = "slotBookmarksEdit()";
    slot_tbl[23].name = "slotEditGotoLine()";
    slot_tbl[24].name = "slotOptionsTTreeView()";
    slot_tbl[25].name = "slotOptionsTOutputView()";
    slot_tbl[26].name = "slotOptionsTStdToolbar()";
    slot_tbl[27].name = "slotOptionsTBrowserToolbar()";
    slot_tbl[28].name = "slotOptionsTStatusbar()";
    slot_tbl[29].name = "slotOptionsRefresh()";
    slot_tbl[30].name = "slotFileNewAppl()";
    slot_tbl[31].name = "slotProjectNew()";
    slot_tbl[32].name = "slotFileOpenPrj()";
    slot_tbl[33].name = "slotProjectCompileFile()";
    slot_tbl[34].name = "slotProjectAddNewFile()";
    slot_tbl[35].name = "slotProjectAddExistingFiles()";
    slot_tbl[36].name = "slotProjectRemoveFile()";
    slot_tbl[37].name = "slotProjectNewClass()";
    slot_tbl[38].name = "slotProjectFileProperties()";
    slot_tbl[39].name = "slotProjectOptions()";
    slot_tbl[40].name = "slotBuildRun()";
    slot_tbl[41].name = "slotBuildDebug()";
    slot_tbl[42].name = "slotBuildMake()";
    slot_tbl[43].name = "slotBuildRebuildAll()";
    slot_tbl[44].name = "slotBuildCleanRebuildAll()";
    slot_tbl[45].name = "slotBuildDistClean()";
    slot_tbl[46].name = "slotBuildAutoconf()";
    slot_tbl[47].name = "slotBuildConfigure()";
    slot_tbl[48].name = "slotBuildStop()";
    slot_tbl[49].name = "slotBuildAPI()";
    slot_tbl[50].name = "slotBuildManual()";
    slot_tbl[51].name = "slotToolsKIconEdit()";
    slot_tbl[52].name = "slotToolsKDbg()";
    slot_tbl[53].name = "slotToolsKTranslator()";
    slot_tbl[54].name = "slotOptionsEditor()";
    slot_tbl[55].name = "slotOptionsEditorColors()";
    slot_tbl[56].name = "slotOptionsSyntaxHighlighting()";
    slot_tbl[57].name = "slotOptionsKDevelop()";
    slot_tbl[58].name = "slotOptionsDocBrowser()";
    slot_tbl[59].name = "slotDocBack()";
    slot_tbl[60].name = "slotDocForward()";
    slot_tbl[61].name = "slotDocSText()";
    slot_tbl[62].name = "slotDocSText(QString)";
    slot_tbl[63].name = "slotDocQtLib()";
    slot_tbl[64].name = "slotDocKDECoreLib()";
    slot_tbl[65].name = "slotDocKDEGUILib()";
    slot_tbl[66].name = "slotDocKDEKFileLib()";
    slot_tbl[67].name = "slotDocKDEHTMLLib()";
    slot_tbl[68].name = "slotDocAPI()";
    slot_tbl[69].name = "slotDocManual()";
    slot_tbl[70].name = "slotDocUpdateKDEDocumentation()";
    slot_tbl[71].name = "slotHelpContent()";
    slot_tbl[72].name = "slotHelpHomepage()";
    slot_tbl[73].name = "slotHelpAbout()";
    slot_tbl[74].name = "slotToolbarClicked(int)";
    slot_tbl[75].name = "slotURLSelected(KHTMLView*,const char*,int,const char*)";
    slot_tbl[76].name = "slotReceivedStdout(KProcess*,char*,int)";
    slot_tbl[77].name = "slotReceivedStderr(KProcess*,char*,int)";
    slot_tbl[78].name = "slotSearchReceivedStdout(KProcess*,char*,int)";
    slot_tbl[79].name = "slotDocumentDone(KHTMLView*)";
    slot_tbl[80].name = "slotProcessExited(KProcess*)";
    slot_tbl[81].name = "slotSearchProcessExited(KProcess*)";
    slot_tbl[82].name = "slotLogFileTreeSelected(int)";
    slot_tbl[83].name = "slotRealFileTreeSelected(int)";
    slot_tbl[84].name = "slotDocTreeSelected(int)";
    slot_tbl[85].name = "slotClassTreeSelected(int)";
    slot_tbl[86].name = "slotCVViewDeclaration(int)";
    slot_tbl[87].name = "slotCVViewDefinition(int)";
    slot_tbl[88].name = "slotSTabSelected(int)";
    slot_tbl[89].name = "slotMenuBuffersSelected(int)";
    slot_tbl[90].name = "slotClickedOnOutputWidget()";
    slot_tbl[91].name = "slotClassChoiceCombo(int)";
    slot_tbl[92].name = "slotMethodChoiceCombo(int)";
    slot_tbl[93].name = "slotStatusMsg(const char*)";
    slot_tbl[94].name = "slotStatusHelpMsg(const char*)";
    slot_tbl[95].name = "statusCallback(int)";
    slot_tbl[96].name = "slotNewStatus()";
    slot_tbl[97].name = "slotNewLineColumn()";
    slot_tbl[98].name = "slotNewUndo()";
    slot_tbl[99].name = "CVGotoClassDecl(QString)";
    slot_tbl[100].name = "CVGotoClassVarDecl(QString,QString)";
    slot_tbl[101].name = "CVGotoMethodeImpl(QString,QString)";
    slot_tbl[102].name = "searchToolGetNumber(QString)";
    slot_tbl[103].name = "searchToolGetTitle(QString)";
    slot_tbl[104].name = "searchToolGetURL(QString)";
    slot_tbl[105].name = "slotCreateSearchDatabase()";
    slot_tbl[106].name = "refreshClassCombos()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl[8].ptr = *((QMember*)&v1_8);
    slot_tbl[9].ptr = *((QMember*)&v1_9);
    slot_tbl[10].ptr = *((QMember*)&v1_10);
    slot_tbl[11].ptr = *((QMember*)&v1_11);
    slot_tbl[12].ptr = *((QMember*)&v1_12);
    slot_tbl[13].ptr = *((QMember*)&v1_13);
    slot_tbl[14].ptr = *((QMember*)&v1_14);
    slot_tbl[15].ptr = *((QMember*)&v1_15);
    slot_tbl[16].ptr = *((QMember*)&v1_16);
    slot_tbl[17].ptr = *((QMember*)&v1_17);
    slot_tbl[18].ptr = *((QMember*)&v1_18);
    slot_tbl[19].ptr = *((QMember*)&v1_19);
    slot_tbl[20].ptr = *((QMember*)&v1_20);
    slot_tbl[21].ptr = *((QMember*)&v1_21);
    slot_tbl[22].ptr = *((QMember*)&v1_22);
    slot_tbl[23].ptr = *((QMember*)&v1_23);
    slot_tbl[24].ptr = *((QMember*)&v1_24);
    slot_tbl[25].ptr = *((QMember*)&v1_25);
    slot_tbl[26].ptr = *((QMember*)&v1_26);
    slot_tbl[27].ptr = *((QMember*)&v1_27);
    slot_tbl[28].ptr = *((QMember*)&v1_28);
    slot_tbl[29].ptr = *((QMember*)&v1_29);
    slot_tbl[30].ptr = *((QMember*)&v1_30);
    slot_tbl[31].ptr = *((QMember*)&v1_31);
    slot_tbl[32].ptr = *((QMember*)&v1_32);
    slot_tbl[33].ptr = *((QMember*)&v1_33);
    slot_tbl[34].ptr = *((QMember*)&v1_34);
    slot_tbl[35].ptr = *((QMember*)&v1_35);
    slot_tbl[36].ptr = *((QMember*)&v1_36);
    slot_tbl[37].ptr = *((QMember*)&v1_37);
    slot_tbl[38].ptr = *((QMember*)&v1_38);
    slot_tbl[39].ptr = *((QMember*)&v1_39);
    slot_tbl[40].ptr = *((QMember*)&v1_40);
    slot_tbl[41].ptr = *((QMember*)&v1_41);
    slot_tbl[42].ptr = *((QMember*)&v1_42);
    slot_tbl[43].ptr = *((QMember*)&v1_43);
    slot_tbl[44].ptr = *((QMember*)&v1_44);
    slot_tbl[45].ptr = *((QMember*)&v1_45);
    slot_tbl[46].ptr = *((QMember*)&v1_46);
    slot_tbl[47].ptr = *((QMember*)&v1_47);
    slot_tbl[48].ptr = *((QMember*)&v1_48);
    slot_tbl[49].ptr = *((QMember*)&v1_49);
    slot_tbl[50].ptr = *((QMember*)&v1_50);
    slot_tbl[51].ptr = *((QMember*)&v1_51);
    slot_tbl[52].ptr = *((QMember*)&v1_52);
    slot_tbl[53].ptr = *((QMember*)&v1_53);
    slot_tbl[54].ptr = *((QMember*)&v1_54);
    slot_tbl[55].ptr = *((QMember*)&v1_55);
    slot_tbl[56].ptr = *((QMember*)&v1_56);
    slot_tbl[57].ptr = *((QMember*)&v1_57);
    slot_tbl[58].ptr = *((QMember*)&v1_58);
    slot_tbl[59].ptr = *((QMember*)&v1_59);
    slot_tbl[60].ptr = *((QMember*)&v1_60);
    slot_tbl[61].ptr = *((QMember*)&v1_61);
    slot_tbl[62].ptr = *((QMember*)&v1_62);
    slot_tbl[63].ptr = *((QMember*)&v1_63);
    slot_tbl[64].ptr = *((QMember*)&v1_64);
    slot_tbl[65].ptr = *((QMember*)&v1_65);
    slot_tbl[66].ptr = *((QMember*)&v1_66);
    slot_tbl[67].ptr = *((QMember*)&v1_67);
    slot_tbl[68].ptr = *((QMember*)&v1_68);
    slot_tbl[69].ptr = *((QMember*)&v1_69);
    slot_tbl[70].ptr = *((QMember*)&v1_70);
    slot_tbl[71].ptr = *((QMember*)&v1_71);
    slot_tbl[72].ptr = *((QMember*)&v1_72);
    slot_tbl[73].ptr = *((QMember*)&v1_73);
    slot_tbl[74].ptr = *((QMember*)&v1_74);
    slot_tbl[75].ptr = *((QMember*)&v1_75);
    slot_tbl[76].ptr = *((QMember*)&v1_76);
    slot_tbl[77].ptr = *((QMember*)&v1_77);
    slot_tbl[78].ptr = *((QMember*)&v1_78);
    slot_tbl[79].ptr = *((QMember*)&v1_79);
    slot_tbl[80].ptr = *((QMember*)&v1_80);
    slot_tbl[81].ptr = *((QMember*)&v1_81);
    slot_tbl[82].ptr = *((QMember*)&v1_82);
    slot_tbl[83].ptr = *((QMember*)&v1_83);
    slot_tbl[84].ptr = *((QMember*)&v1_84);
    slot_tbl[85].ptr = *((QMember*)&v1_85);
    slot_tbl[86].ptr = *((QMember*)&v1_86);
    slot_tbl[87].ptr = *((QMember*)&v1_87);
    slot_tbl[88].ptr = *((QMember*)&v1_88);
    slot_tbl[89].ptr = *((QMember*)&v1_89);
    slot_tbl[90].ptr = *((QMember*)&v1_90);
    slot_tbl[91].ptr = *((QMember*)&v1_91);
    slot_tbl[92].ptr = *((QMember*)&v1_92);
    slot_tbl[93].ptr = *((QMember*)&v1_93);
    slot_tbl[94].ptr = *((QMember*)&v1_94);
    slot_tbl[95].ptr = *((QMember*)&v1_95);
    slot_tbl[96].ptr = *((QMember*)&v1_96);
    slot_tbl[97].ptr = *((QMember*)&v1_97);
    slot_tbl[98].ptr = *((QMember*)&v1_98);
    slot_tbl[99].ptr = *((QMember*)&v1_99);
    slot_tbl[100].ptr = *((QMember*)&v1_100);
    slot_tbl[101].ptr = *((QMember*)&v1_101);
    slot_tbl[102].ptr = *((QMember*)&v1_102);
    slot_tbl[103].ptr = *((QMember*)&v1_103);
    slot_tbl[104].ptr = *((QMember*)&v1_104);
    slot_tbl[105].ptr = *((QMember*)&v1_105);
    slot_tbl[106].ptr = *((QMember*)&v1_106);
    metaObj = new QMetaObject( "CKDevelop", "KTMainWindow",
	slot_tbl, 107,
	0, 0 );
}
