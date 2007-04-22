/***************************************************************************
  copyright            : (C) 2006 by David Nolden
  email                : david.nolden.kdevelop@art-master.de
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kdevutils.h"
#include "teamworkfoldermanager.h"
#include "kdevteamwork.h"
#include <idocumentcontroller.h>
#include <idocument.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/cursor.h>

using namespace KDevelop;

QString currentDocumentPath() {
  IDocumentController * docControl = KDevTeamwork::documentController();

  IDocument* d = docControl->activeDocument();
  if ( !d )
    throw QString( "no active document" );

  KTextEditor::Document* doc = d->textDocument();
  if ( !doc )
    throw QString( "no active text-document" );

  KTextEditor::View* view = doc->activeView();
  if ( !view )
    throw QString( "no active view" );

  return TeamworkFolderManager::workspaceRelative( d->url().path() );
}

// kate: space-indent on; indent-width 2; tab-width 2; replace-tabs on
