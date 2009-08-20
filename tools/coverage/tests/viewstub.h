/* KDevelop coverage plugin
 *
 * Copyright 2008 Manuel Breugelmans <mbr.nxi@gmail.com>
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

#ifndef TESTSTUBS_VIEWSTUB_H
#define TESTSTUBS_VIEWSTUB_H

#include <ktexteditor/view.h>
#include <ktexteditor/annotationinterface.h>
#include "../coverageexport.h"

namespace TestStubs
{

class VERITAS_COVERAGE_EXPORT TextView :
                public KTextEditor::View,
                public KTextEditor::AnnotationViewInterface
{
Q_OBJECT
Q_INTERFACES(KTextEditor::AnnotationViewInterface)

public:
    void setAnnotationModel(KTextEditor::AnnotationModel* model) {
        m_annotationModel = model;
    }
    KTextEditor::AnnotationModel* annotationModel() const {
        return m_annotationModel;
    }
    void setAnnotationBorderVisible(bool visible) {
        m_borderVisible = visible;
    }
    bool isAnnotationBorderVisible() const {
        return m_borderVisible;
    }
    KTextEditor::AnnotationModel* m_annotationModel;
    bool m_borderVisible;

Q_SIGNALS:
    void annotationContextMenuAboutToShow(KTextEditor::View* view, QMenu* menu, int line);
    void annotationActivated(KTextEditor::View* view, int line);
    void annotationBorderVisibilityChanged(View* view, bool visible);

public:
    TextView(QWidget *parent) :
        KTextEditor::View(parent),
        m_annotationModel(0) {}
    virtual ~TextView() {}

    virtual KTextEditor::Document *document() const { return 0; }
    virtual QString viewMode() const { return ""; }
//     enum EditMode {
//       EditInsert = 0,   /**< Insert mode. Characters will be added. */
//       EditOverwrite = 1 /**< Overwrite mode. Characters will be replaced. */
//     };
    virtual enum EditMode viewEditMode() const { return EditOverwrite; }
    virtual void setContextMenu(QMenu *menu) { Q_UNUSED(menu); }
    virtual QMenu *contextMenu() const { return 0; }
    virtual QMenu* defaultContextMenu(QMenu* menu = 0L) const { Q_UNUSED(menu); return 0; }
    virtual bool setCursorPosition(KTextEditor::Cursor position) { Q_UNUSED(position); return false; }
    virtual KTextEditor::Cursor cursorPosition() const { return KTextEditor::Cursor(); }
    virtual KTextEditor::Cursor cursorPositionVirtual() const { return KTextEditor::Cursor(); }
    virtual QPoint cursorToCoordinate(const KTextEditor::Cursor& cursor) const { Q_UNUSED(cursor); return QPoint(); }
    virtual QPoint cursorPositionCoordinates() const { return QPoint(); }
    virtual bool setSelection(const KTextEditor::Range &range) { Q_UNUSED(range); return true; }
    /*    virtual bool setSelection(const Cursor &position,
                                  int length,
                                  bool wrap = true);*/
    virtual bool selection() const { return true; }

    KTextEditor::Range m_selectionRange;

    virtual const KTextEditor::Range &selectionRange() const { return m_selectionRange; }
    virtual QString selectionText() const { return ""; }
    virtual bool removeSelection() { return true; }
    virtual bool removeSelectionText() { return true; }
    virtual bool setBlockSelection(bool on) { Q_UNUSED(on); return true; }
    virtual bool blockSelection() const { return true; }

    virtual bool mouseTrackingEnabled() const { return true; }
    virtual bool setMouseTrackingEnabled(bool b) { Q_UNUSED(b); return true; }

#if 0
                                      Q_SIGNALS:
                                          void contextMenuAboutToShow(KTextEditor::View* view, QMenu* menu);
    void focusIn(KTextEditor::View *view);
    void focusOut(KTextEditor::View *view);
    void viewModeChanged(KTextEditor::View *view);
    void viewEditModeChanged(KTextEditor::View *view,
                             enum KTextEditor::View::EditMode mode);
    void informationMessage(KTextEditor::View *view, const QString &message);
    void textInserted(KTextEditor::View *view,
                      const KTextEditor::Cursor &position,
                      const QString &text);

    void cursorPositionChanged(KTextEditor::View *view,
                               const KTextEditor::Cursor& newPosition);
    void verticalScrollPositionChanged(KTextEditor::View *view, const KTextEditor::Cursor& newPos);
    void horizontalScrollPositionChanged(KTextEditor::View *view);
    void mousePositionChanged(KTextEditor::View *view,
                              const KTextEditor::Cursor& newPosition);
    void selectionChanged(KTextEditor::View *view);
#endif

};

}

#endif // TESTFAKES_VIEWSTUB
