/* This file is part of KDevelop

   Copyright 2010 Milian Wolff <mail@milianw.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "test_parser.h"

#include <QTest>

#include "parsesession.h"

void TestParser::testRangeBasedFor()
{
  QByteArray code("int main() {\n"
                  "  int array[5] = { 1, 2, 3, 4, 5 };\n"
                  "  for (int& x : array) {\n"
                  "    x *= 2;\n"
                  "  }\n"
                  " }\n");
  TranslationUnitAST* ast = parse(code);

  QVERIFY(ast);
  QVERIFY(ast->declarations);
  QVERIFY(control.problems().isEmpty());
}

void TestParser::testRValueReference()
{
  QByteArray code("int&& a = 1;");

  TranslationUnitAST* ast = parse(code);
  QVERIFY(control.problems().isEmpty());

  QVERIFY(ast);
  QVERIFY(ast->declarations);
}

void TestParser::testDefaultDeletedFunctions()
{
  QByteArray code("class A{\n"
                  "  A() = default;\n"
                  "  A(const A&) = delete;\n"
                  "};\n"
                  "bool operator==(const A&, const A&) = default;\n"
                  "bool operator!=(const A&, const A&) = delete;\n");
  TranslationUnitAST* ast = parse(code);
  QVERIFY(control.problems().isEmpty());

  QVERIFY(ast);
  QVERIFY(ast->declarations);
}
