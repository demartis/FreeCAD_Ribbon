/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <QAction>
# include <QApplication>
# include <QToolBar>
# include <QToolButton>
#endif

#include "ToolBarManager.h"
#include "StdToolBarManager.h"
#include "Ribbon/RibbonToolBarManager.h"
#include "Application.h"
#include "Command.h"
#include "MainWindow.h"


using namespace Gui;

ToolBarItem::ToolBarItem() : visibilityPolicy(DefaultVisibility::Visible)
{
}

ToolBarItem::ToolBarItem(ToolBarItem* item, DefaultVisibility visibilityPolicy) : visibilityPolicy(visibilityPolicy)
{
    if (item) {
        item->appendItem(this);
    }
}

ToolBarItem::~ToolBarItem()
{
    clear();
}

void ToolBarItem::setCommand(const std::string& name)
{
    _name = name;
}

const std::string & ToolBarItem::command() const
{
    return _name;
}

bool ToolBarItem::hasItems() const
{
    return _items.count() > 0;
}

ToolBarItem* ToolBarItem::findItem(const std::string& name)
{
    if ( _name == name ) {
        return this;
    }

    for (auto it : qAsConst(_items)) {
        if (it->_name == name) {
            return it;
        }
    }

    return nullptr;
}

ToolBarItem* ToolBarItem::copy() const
{
    auto root = new ToolBarItem;
    root->setCommand( command() );

    QList<ToolBarItem*> items = getItems();
    for (auto it : items) {
        root->appendItem(it->copy());
    }

    return root;
}

uint ToolBarItem::count() const
{
    return _items.count();
}

void ToolBarItem::appendItem(ToolBarItem* item)
{
    _items.push_back( item );
}

bool ToolBarItem::insertItem( ToolBarItem* before, ToolBarItem* item)
{
    int pos = _items.indexOf(before);
    if (pos != -1) {
        _items.insert(pos, item);
        return true;
    }

    return false;
}

void ToolBarItem::removeItem(ToolBarItem* item)
{
    int pos = _items.indexOf(item);
    if (pos != -1) {
        _items.removeAt(pos);
    }
}

void ToolBarItem::clear()
{
    for (auto it : qAsConst(_items)) {
        delete it;
    }

    _items.clear();
}

ToolBarItem& ToolBarItem::operator << (ToolBarItem* item)
{
    appendItem(item);
    return *this;
}

ToolBarItem& ToolBarItem::operator << (const std::string& command)
{
    auto item = new ToolBarItem(this);
    item->setCommand(command);
    return *this;
}

QList<ToolBarItem*> ToolBarItem::getItems() const
{
    return _items;
}

// -----------------------------------------------------------

ToolBarManager* ToolBarManager::_instance = nullptr;
//StdToolBarManager or RibbonToolBarManager
ToolBarManagerInterface* ToolBarManager::_implementation = nullptr;


namespace {
QPointer<QWidget> createActionWidget()
{
    static QPointer<QWidget> _ActionWidget;
    if (!_ActionWidget) {
        _ActionWidget = new QWidget(getMainWindow());
        _ActionWidget->setObjectName(QStringLiteral("_fc_action_widget_"));
        /* TODO This is a temporary hack until a longterm solution
        is found, thanks to @realthunder for this pointer.
        Although _ActionWidget has zero size, it somehow has a
        'phantom' size without any visible content and will block the top
        left tool buttons and menus of the application main window.
        Therefore it is moved out of the way. */
        _ActionWidget->move(QPoint(-100,-100));
    }
    else {
        auto actions = _ActionWidget->actions();
        for (auto action : actions) {
            _ActionWidget->removeAction(action);
        }
    }

    return _ActionWidget;
}
}


ToolBarManager* ToolBarManager::getInstance()
{
	if (!_instance)
	{
		_instance = new ToolBarManager;
		_implementation = new RibbonToolBarManager();
	}
	return _instance;
}

ToolBarManagerInterface* ToolBarManager::getToolbarImplementation()
{
	return _implementation;
}

void ToolBarManager::destruct()
{
	delete _implementation;
	_implementation = nullptr;
	delete _instance;
	_instance = nullptr;
}

ToolBarManager::ToolBarManager()
{
}

ToolBarManager::~ToolBarManager()
{
}

void ToolBarManager::setup(const QString& workbenchName,ToolBarItem* toolBarItems)
{
	assert(_implementation);
	_implementation->setup(workbenchName,toolBarItems);

}


void ToolBarManager::saveState() const
{
	assert(_implementation);
	_implementation->saveState();
}

void ToolBarManager::restoreState() const
{
	assert(_implementation);
	_implementation->restoreState();
}

void ToolBarManager::retranslate() const
{
	assert(_implementation);
	_implementation->retranslate();
}

void Gui::ToolBarManager::setMovable(bool moveable) const
{
	assert(_implementation);
	_implementation->setMovable(moveable);
}

