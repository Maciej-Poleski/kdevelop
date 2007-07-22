/*
 * KDevelop C++ Code Completion Support
 *
 * Copyright (c) 2006 Hamish Rodda <rodda@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "cppcodecompletionmodel.h"

#include <QMetaType>
#include <kdebug.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <kiconloader.h>

#include <khtml_part.h>
#include <khtmlview.h>

#include "duchainbuilder/cppduchain.h"

#include <declaration.h>
#include "cpptypes.h"
#include "typeutils.h"
#include <classfunctiondeclaration.h>
#include <ducontext.h>
#include <duchain.h>
#include <duchainlock.h>
#include <duchainbase.h>
#include <topducontext.h>
#include "dumpchain.h"
#include "codecompletioncontext.h"

using namespace KTextEditor;
using namespace KDevelop;

CppCodeCompletionModel::CppCodeCompletionModel( QObject * parent )
  : CodeCompletionModel(parent)
{
}

CppCodeCompletionModel::~CppCodeCompletionModel()
{
}

void CppCodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
  Q_UNUSED(invocationType)

  KUrl url = view->document()->url();
  if (TopDUContext* top = DUChain::self()->chainForDocument(url)) {
    kDebug(9007) << "completion invoked for context " << top << endl;
    DUContextPointer thisContext;
    {
      DUChainReadLocker lock(DUChain::lock());
      thisContext = top->findContextAt(range.start());

       kDebug(9007) << "context is set to " << thisContext.data() << endl;
        if( thisContext ) {
          kDebug( 9007 ) << "================== duchain for the context =======================" << endl;
          DumpChain dump;
          dump.dump(thisContext.data());
        } else {
          kDebug( 9007 ) << "================== NO CONTEXT FOUND =======================" << endl;
          return;
        }
    }

    setContext(thisContext, range.start(), view);

  } else {
    kDebug(9007) << k_funcinfo << "Completion invoked for unknown context. Document: " << url << ", Known documents: " << DUChain::self()->documents() << endl;
  }
}

QVariant CppCodeCompletionModel::data(const QModelIndex& index, int role) const
{
  quint32 dataIndex = index.internalId();

  if( dataIndex >= (quint32)m_declarations.size() )
    return QVariant();
  
  DUChainReadLocker lock(DUChain::lock());
  
  Declaration* dec = const_cast<Declaration*>( m_declarations[dataIndex].first.data() );
  if (!dec) {
    kDebug() <<  "code-completion model item " << dataIndex << ": Du-chain item is deleted" << endl;
    return QVariant();
  }

  bool isArgumentHint = false;
  if( m_declarations[dataIndex].second && m_declarations[dataIndex].second->memberAccessOperation() == Cpp::CodeCompletionContext::FunctionCallAccess )
    isArgumentHint = true;

  switch (role) {
    case SetMatchContext:
      m_currentMatchContext = m_declarations[dataIndex];
      return QVariant(1);
    case MatchQuality:
    {
      if( m_currentMatchContext.first && m_currentMatchContext.second && m_currentMatchContext.second->memberAccessOperation() == Cpp::CodeCompletionContext::FunctionCallAccess ) {
        //Cpp::TypeConversion conv;
        //return conv.implicitConversion(
      } else {
        kDebug() << "MatchQuality requested with invalid match-context" << endl;
      }
    }
    case ArgumentHintDepth:
      if( isArgumentHint )
        return m_declarations[dataIndex].second->depth();
    case ItemSelected:
       return QVariant(dec->toString());
    case IsExpandable:
      return QVariant(true);
    case ExpandingWidget: {
       KHTMLPart *w = new KHTMLPart();
       w->begin();
       w->write( QString( "<html><body><p><small>Item: %1</small></p></body></html>" ).arg(dec->toString()) );
       w->end();
       w->view()->resize(500, 70);

//        QPalette palette;
//        palette.setColor(w->view()->backgroundRole(), QColor(0,0,0));//0xffcfcfcf));
//        w->view()->setPalette(palette);

       QVariant v;
       v.setValue<QWidget*>(w->view());
       return v;
    }
    case Qt::DisplayRole:
      switch (index.column()) {
        case Prefix:
          if( dec->kind() == Declaration::Type && !dec->type<CppFunctionType>() ) {
            if (CppClassType::Ptr classType =  dec->type<CppClassType>())
              switch (classType->classType()) {
                case CppClassType::Class:
                  return "class";
                  break;
                case CppClassType::Struct:
                  return "struct";
                  break;
                case CppClassType::Union:
                  return "union";
                  break;
              }
            return QVariant();
          }
          if (dec->abstractType()) {
            if (CppFunctionType::Ptr functionType = dec->type<CppFunctionType>()) {
              if (functionType->returnType())
                return functionType->returnType()->toString();
              else
                return "<incomplete type>";

            } else {
              return dec->abstractType()->toString();
            }

          } else {
            return "<incomplete type>";
          }

        case Scope: {
          QualifiedIdentifier id = dec->qualifiedIdentifier();
          if (id.isEmpty())
            return QVariant();
          id.pop();
          if (id.isEmpty())
            return QVariant();
          return id.toString() + "::";
        }

        case Name:
          if (dec->identifier().toString().isEmpty())
            return "<unknown>";
          else
            return dec->identifier().toString();

        case Arguments:
          if (CppFunctionType::Ptr functionType = dec->type<CppFunctionType>()) {
            QString ret = "(";
            bool first = true;
            foreach (const AbstractType::Ptr& argument, functionType->arguments()) {
              if (first)
                first = false;
              else
                ret += ", ";

              if (argument)
                ret += argument->toString();
              else
                ret += "<incomplete type>";
            }
            ret += ")";
            return ret;
          }
          break;

        case Postfix:
          if (CppFunctionType::Ptr functionType = dec->type<CppFunctionType>()) {
            return functionType->cvString();
          }
          break;
      }
      break;

    case Qt::DecorationRole:
    case CompletionRole: {
      CompletionProperties p;
      if (ClassMemberDeclaration* member = dynamic_cast<ClassMemberDeclaration*>(dec)) {
        switch (member->accessPolicy()) {
          case Declaration::Public:
            p |= Public;
            break;
          case Declaration::Protected:
            p |= Protected;
            break;
          case Declaration::Private:
            p |= Private;
            break;
        }

        if (member->isStatic())
          p |= Static;
        if (member->isAuto())
          ;//TODO
        if (member->isFriend())
          p |= Friend;
        if (member->isRegister())
          ;//TODO
        if (member->isExtern())
          ;//TODO
        if (member->isMutable())
          ;//TODO
      }

      if (ClassFunctionDeclaration* function = dynamic_cast<ClassFunctionDeclaration*>(dec)) {
        if (function->isVirtual())
          p |= Virtual;
        if (function->isInline())
          p |= Inline;
        if (function->isExplicit())
          ;//TODO
      }

      if (dec->abstractType()) {
        if (CppCVType* cv = dynamic_cast<CppCVType*>(dec->abstractType().data())) {
          if (cv->isConstant())
            p |= Const;
          if (cv->isVolatile())
            ;//TODO
          }

        switch (dec->abstractType()->whichType()) {
          case AbstractType::TypeIntegral:
            if (dec->type<CppEnumerationType>())
              p |= Enum;
            else
              p |= Variable;
            break;
          case AbstractType::TypePointer:
            p |= Variable;
            break;
          case AbstractType::TypeReference:
            p |= Variable;
            break;
          case AbstractType::TypeFunction:
            p |= Function;
            break;
          case AbstractType::TypeStructure:
            if (CppClassType::Ptr classType =  dec->type<CppClassType>())
              switch (classType->classType()) {
                case CppClassType::Class:
                  p |= Class;
                  break;
                case CppClassType::Struct:
                  p |= Struct;
                  break;
                case CppClassType::Union:
                  p |= Union;
                  break;
              }
            break;
          case AbstractType::TypeArray:
            p |= Variable;
            break;
          case AbstractType::TypeAbstract:
            // TODO
            break;
        }

        if( dec->kind() == Declaration::Instance )
          p |= Variable;
      }

      /*
      LocalScope      = 0x80000,
      NamespaceScope  = 0x100000,
      GlobalScope     = 0x200000,
      */

      if( role == CompletionRole ) {
        return (int)p;
      } else {
        ///Assign mini-icons
        QString iconName;

        if( (p & Variable) )
          iconName = "CVprotected_var";
        else if( (p & Variable) && (p & Protected) )
          iconName = "CVprotected_var";
        else if( (p & Variable) && (p & Private) )
          iconName = "CVprivate_var";
        else if( (p & Union) && (p & Protected) )
          iconName = "protected_union";
        else if( (p & Enum) && (p & Protected) )
          iconName = "protected_enum";
        else if( (p & Struct) && (p & Private) )
          iconName = "private_struct";
        else if( (p & Slot) && (p & Protected) )
          iconName = "CVprotected_slot";
        else if( (p & Enum) && (p & Private) )
          iconName = "private_enum";
        else if( (p & Signal) && (p & Protected) )
          iconName = "CVprotected_signal";
        else if( (p & Slot) && (p & Private) )
          iconName = "CVprivate_slot";
        else if( (p & Class) && (p & Protected) )
          iconName = "protected_class";
        else if( (p & Class) && (p & Private) )
          iconName = "private_class";
        else if( (p & Union) && (p & Private) )
          iconName = "private_union";
/*        else if( (p & TypeAlias) && ((p & Const) ||  (p & Volatile)) )
          iconName = "CVtypedef";*/
        else if( (p & Function) && (p & Protected) )
          iconName = "protected_function";
        else if( (p & Function) && (p & Private) )
          iconName = "private_function";
        else if( p & Signal )
          iconName = "signal";
        else if( p & Variable )
          iconName = "CVpublic_var";
        else if( p & Enum )
          iconName = "enum";
        else if( p & Class )
          iconName = "class";
        else if( p & Slot )
          iconName = "CVpublic_slot";
        else if( p & Union )
          iconName = "union";
/*        else if( p & TypeAlias )
          iconName = "typedef";*/
        else if( p & Function )
          iconName = "function";
        else if( p & Struct )
          iconName = "struct";
        else if( p & Protected )
          iconName = "protected_field";
        else if( p & Private )
          iconName = "private_field";
        else
          iconName = "field";
        
        if( index.column() == Icon ) {
          lock.unlock();
          return QVariant( KIconLoader::global()->loadIcon(iconName, K3Icon::Small) );
        }
        break;

      }
    }

    case ScopeIndex:
      return static_cast<int>(reinterpret_cast<long>(dec->context()));
  }

  return QVariant();
}

QModelIndex CppCodeCompletionModel::index(int row, int column, const QModelIndex& parent) const
{
  if (row < 0 || row >= m_declarations.count() || column < 0 || column >= ColumnCount || parent.isValid())
    return QModelIndex();

  QModelIndex ret = createIndex(row, column, row);

  return ret;
}

int CppCodeCompletionModel::rowCount ( const QModelIndex & parent ) const
{
  if (parent.isValid())
    return 0;

  return m_declarations.count();
}

void CppCodeCompletionModel::setContext(DUContextPointer context, const KTextEditor::Cursor& position, KTextEditor::View* view)
{
  m_context = context;
  Q_ASSERT(m_context);
  //@todo move completion-context-building into another thread

  m_declarations.clear();

  //Compute the text we should complete on
  KTextEditor::Document* doc = view->document();
  if( !doc ) {
    kDebug() << "No document for completion" << endl;
    return;
  }

  KTextEditor::Range range;
  QString text;
  {
    DUChainReadLocker lock(DUChain::lock());
    range = KTextEditor::Range(context->textRange().start(), position);

    text = doc->text(range);
  }

  if( text.isEmpty() ) {
    kDebug() << "no text for context" << endl;
    return;
  }

  if( position.column() == 0 ) //Seems like when the cursor is a the beginning of a line, kate does not give the \n
    text += "\n";

  Cpp::CodeCompletionContext::Ptr completionContext( new Cpp::CodeCompletionContext( context, text ) );
  m_completionContext = completionContext;

  if( completionContext->isValid() ) {
    DUChainReadLocker lock(DUChain::lock());

    if( completionContext->memberAccessContainer().isValid() ) {
      IdentifiedType* idType = dynamic_cast<IdentifiedType*>(completionContext->memberAccessContainer().type.data());
      if( idType && idType->declaration() ) {
        DUContext* ctx = TypeUtils::getInternalContext( idType->declaration() );
        if( ctx ) {
          m_declarations.clear();
          foreach( Declaration* decl, Cpp::localDeclarations( ctx ) )
            m_declarations << DeclarationContextPair( decl, completionContext );
        } else {
          kDebug() << "Could not get internal context from declaration \"" << idType->declaration()->toString() << "\"" << endl;
        }
      } else {
        kDebug() << "CppCodeCompletionModel::setContext: bad container-type" << endl;
      }
    } else {
      m_declarations.clear();
      foreach( Declaration* decl, m_context->allDeclarations(position).values() )
        m_declarations << DeclarationContextPair( decl, completionContext );
      kDebug() << "CppCodeCompletionModel::setContext: using all declarations visible: " << m_declarations.count() << endl;
    }

    ///Find all recursive function-calls that should be shown as call-tips
    Cpp::CodeCompletionContext::Ptr parentContext = completionContext;
    do {
      parentContext = parentContext->parentContext();
      if( parentContext ) {
        if( parentContext->memberAccessOperation() == Cpp::CodeCompletionContext::FunctionCallAccess ) {
          foreach( Cpp::CodeCompletionContext::Function function, parentContext->functions() )
            m_declarations << DeclarationContextPair( function.function.declaration(), parentContext );
        } else {
          kDebug() << "parent-context has non function-call access type" << endl;
        }
      }
    } while( parentContext );
  } else {
    kDebug() << "CppCodeCompletionModel::setContext: Invalid code-completion context" << endl;
  }

  // TODO maybe one day just behave like a nice model and call insert rows etc.
  reset();
}

#include "cppcodecompletionmodel.moc"

// kate: space-indent on; indent-width 2; replace-tabs on
