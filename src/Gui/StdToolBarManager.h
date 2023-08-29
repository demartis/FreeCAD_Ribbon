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


#ifndef GUI_STDTOOLBARMANAGER_H
#define GUI_STDTOOLBARMANAGER_H

#include <string>
#include <QStringList>


#include "ToolBarManagerInterface.h"

class QAction;
class QToolBar;

namespace Gui {

	class ToolBarItem;


	/**
	 * The Standard ToolBarManager class is responsible for the creation of toolbars and appending them
	 * to the main window.
	 * @see ToolBoxManager
	 * @see MenuManager
	 * @author Werner Mayer
	 */
	class GuiExport StdToolBarManager : public Gui::ToolBarManagerInterface
	{
	public:
		StdToolBarManager();
		~StdToolBarManager();

		void setup(const QString& workbenchName, ToolBarItem*) override;
		void saveState() const override;
		void restoreState() const override;
		void retranslate() const override;
		void setMovable(bool movable) const override;
		void setState(const QList<QString>& names, State state) override;
		void setState(const QString& name, State state) override;
		ToolBarItem::DefaultVisibility getToolbarPolicy(const QToolBar *) const override;
	protected:
		void setup(ToolBarItem*, QToolBar*) const;
		/** Returns a list of all currently existing toolbars. */
		QList<QToolBar*> toolBars() const;
		QToolBar* findToolBar(const QList<QToolBar*>&, const QString&) const;
		QAction* findAction(const QList<QAction*>&, const QString&) const;


	private:
		QStringList toolbarNames;

	};

} // namespace Gui


#endif // GUI_STDTOOLBARMANAGER_H
