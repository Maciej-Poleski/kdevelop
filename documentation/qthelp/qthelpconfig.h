/*  This file is part of KDevelop

    Copyright 2010 Benjamin Port <port.benjamin@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef QTHELPCONFIG_H
#define QTHELPCONFIG_H

#include <KCModule>

#include "ui_qthelpconfig.h"

namespace Ui
{
    class QtHelpConfigUI;
}

class QtHelpConfig : public KCModule
{
public:
    Q_OBJECT

    public:
      explicit QtHelpConfig(QWidget *parent = 0, const QVariantList &args = QVariantList());
      virtual ~QtHelpConfig();

      virtual void save();
      virtual void load();
      virtual void defaults();

    private slots:
      void add();
      void remove();
      void up();
      void down();
      void modify();
      void selectionChanged();
    private:
      Ui::QtHelpConfigUI* m_configWidget;
};

#endif // QTHELPCONFIG_H
