

#include "PreCompiled.h"
#ifndef _PreComp_
# include <QAction>
# include <QApplication>
# include <QToolBar>
# include <QToolButton>
#endif

#include "RibbonToolBarManager.h"
#include "ToolBarManager.h"
#include "Application.h"
#include "Command.h"
#include "MainWindow.h"
#include "Ribbon/Ribbon.h"
#include "Ribbon/ribbontabcontent.h"
#include "Ribbon/ribbonbuttongroup.h"
#include "Ribbon/RibbonToolBar.h"
#include "Ribbon/RibbonTitlebar.h"

using namespace Gui;

// -----------------------------------------------------------


RibbonToolBarManager::RibbonToolBarManager()
{
}

RibbonToolBarManager::~RibbonToolBarManager()
{
}


void RibbonToolBarManager::createRibbon()
{
	if (!ribbonDockWidget_)
	{
		Gui::getMainWindow()->setWindowFlags(Qt::FramelessWindowHint); // this line hides the title bar.
		Gui::getMainWindow()->setWindowState(Qt::WindowFullScreen);
		ribbonDockWidget_ = new QDockWidget(Gui::getMainWindow());
		ribbonDockWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
		ribbonDockWidget_->setAllowedAreas(Qt::TopDockWidgetArea);
		ribbonDockWidget_->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
		ribbonDockWidget_->setStyleSheet(QString::fromLatin1(" background-color : #2470BF;"));

		Gui::getMainWindow()->addDockWidget(Qt::TopDockWidgetArea, ribbonDockWidget_);
		QWidget* ribbonDockWidgetContents = new QWidget(ribbonDockWidget_);
		ribbonDockWidgetContents->setStyleSheet(QString::fromLatin1(" background-color : #2470BF;"));
		ribbonDockWidget_->setWidget(ribbonDockWidgetContents);
		QGridLayout* layout = new QGridLayout(ribbonDockWidgetContents);
		ribbonDockWidgetContents->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
		ribbonDockWidget_->setTitleBarWidget(new QWidget());
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		ribbonDockWidgetContents->setLayout(layout);
		ribbon_ = new Ribbon();

		layout->addWidget(new RibbonTitlebar(), 0, 0);
		layout->addWidget(ribbon_, 1, 0);



	}
}


std::function<QString(const QString&, const QString&)> RibbonToolBarManager::getTabNameCallback = [](const QString& workbenchName, const QString& groupLabel) {
	QString destTab = workbenchName;
	if (groupLabel == QString::fromLatin1("File") ||
		groupLabel == QString::fromLatin1("Workbench") ||
		groupLabel == QString::fromLatin1("Macro") ||
		groupLabel == QString::fromLatin1("Structure") ||
		groupLabel == QString::fromLatin1("View"))
		return QString::fromUtf8("HOME");
	destTab = destTab.remove(QString::fromUtf8("Workbench"));
	return destTab.toUpper();
};



QString RibbonToolBarManager::getTabName(const QString& workbenchName, const QString& groupLabel)
{
	return getTabNameCallback(workbenchName,groupLabel);
}




RibbonToolbar* RibbonToolBarManager::createToolbar(const QString& workbenchName, const QString& groupLabel)
{
	QString destTab = getTabName(workbenchName, groupLabel);
	if(destTab.isEmpty()) return nullptr;
	RibbonTabContent* tab = ribbon_->getTab(destTab);
	if (!tab)
	{

		tab = ribbon_->addTab(QIcon(QString::fromUtf8(":/icons/Group.svg")), destTab);
		//	tab = ribbon_->addTab(QIcon(QString::fromUtf8(":/icons/ClassBrowser/member.svg")), destTab);

	}

	RibbonButtonGroup* group = tab->getGroup(groupLabel);
	if (!group)
		group = tab->addGroup(groupLabel);


	auto toolbar = new RibbonToolbar;
	toolbar->UID = groupLabel;
	QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Preferred);
	//     toolbar->setSizePolicy(sp);
	group->addToolbar(toolbar);

	//getMainWindow()->addToolBar(label);
	toolbars_.append(toolbar);
	return toolbar;
}

void RibbonToolBarManager::setup(const QString& workbenchName, ToolBarItem* toolBarItems)
{

	if (!toolBarItems)
		return; // empty menu bar

	createRibbon();

	assert(ribbon_);

	saveState();
	this->toolbarNames.clear();

	int max_width = getMainWindow()->width();
	int top_width = 0;

	ParameterGrp::handle hPref = App::GetApplication().GetUserParameter().GetGroup("BaseApp")
		->GetGroup("MainWindow")->GetGroup("Toolbars");
	bool nameAsToolTip = App::GetApplication().GetUserParameter().GetGroup("BaseApp")
		->GetGroup("Preferences")->GetGroup("MainWindow")->GetBool("ToolBarNameAsToolTip", true);
	QList<ToolBarItem*> items = toolBarItems->getItems();

	CommandManager& mgr = Application::Instance->commandManager();

	for (QList<ToolBarItem*>::ConstIterator it = items.begin(); it != items.end(); ++it) {
		QString toolbarName = QString::fromUtf8((*it)->command().c_str());
		auto exclude = QString::fromUtf8("Created Macro Toolbar");
		if (toolbarName.contains(exclude))
			continue;
		std::string tabName = getTabName(workbenchName, toolbarName).toStdString();
		bool visible = hPref->GetBool(toolbarName.toStdString().c_str(), true);
		auto toolbar = findToolBar(toolbarName);
		if (!toolbar)
		{
			toolbar = createToolbar(QString::fromUtf8(tabName.c_str()), toolbarName);
			if (!toolbar) continue;
			//toolbar->setObjectName(toolbarName);
			if (nameAsToolTip) {
				auto tooltip = QChar::fromLatin1('[')
					+ QApplication::translate("Workbench", toolbarName.toStdString().c_str())
					+ QChar::fromLatin1(']');
				toolbar->setToolTip(tooltip);
			}
			toolbar->setVisible(visible);

		}
		QList<QAction*> actions = toolbar->actions();
		QList<ToolBarItem*> items = (*it)->getItems();
		for (QList<ToolBarItem*>::ConstIterator it2 = items.begin(); it2 != items.end(); ++it2) {
			QAction* action = findAction(actions, QString::fromLatin1((*it2)->command().c_str()));


			if (!action) {
				if ((*it2)->command() == "Separator") {
					action = toolbar->addSeparator();
				}
				else {
					// Check if action was added successfully
					if (mgr.addTo((*it2)->command().c_str(), toolbar))
						action = toolbar->actions().last();
				}

				// set the tool button user data
				if (action) action->setData(QString::fromLatin1((*it2)->command().c_str()));
			}

		}
	}


}

void RibbonToolBarManager::setup(ToolBarItem* item, RibbonToolbar* toolbar) const
{
	CommandManager& mgr = Application::Instance->commandManager();
	QList<ToolBarItem*> items = item->getItems();
	QList<QAction*> actions = toolbar->actions();
	for (QList<ToolBarItem*>::ConstIterator it = items.begin(); it != items.end(); ++it) {
		// search for the action item
		QAction* action = findAction(actions, QString::fromLatin1((*it)->command().c_str()));
		if (!action) {
			if ((*it)->command() == "Separator") {
				action = toolbar->addSeparator();
			}
			else {
				// Check if action was added successfully
				if (mgr.addTo((*it)->command().c_str(), toolbar))
					action = toolbar->actions().last();
			}

			// set the tool button user data
			if (action) action->setData(QString::fromLatin1((*it)->command().c_str()));
		}
		else {
			// Note: For toolbars we do not remove and re-add the actions
			// because this causes flicker effects. So, it could happen that the order of
			// buttons doesn't match with the order of commands in the workbench.
			//int index = actions.indexOf(action);
			//actions.removeAt(index);
		}
	}

	// remove all tool buttons which we don't need for the moment
	for (QList<QAction*>::Iterator it = actions.begin(); it != actions.end(); ++it) {
		toolbar->removeAction(*it);
	}
}

void RibbonToolBarManager::saveState() const
{

}

void RibbonToolBarManager::restoreState() const
{

}

void RibbonToolBarManager::retranslate() const
{

}

void Gui::RibbonToolBarManager::setMovable(bool moveable) const
{
	for (auto& tb : toolBars()) {
		tb->setMovable(moveable);
	}
}

RibbonToolbar* RibbonToolBarManager::findToolBar(const QString& item) const
{
	for (QList<RibbonToolbar*>::ConstIterator it = toolbars_.begin(); it != toolbars_.end(); ++it) {
		if ((*it)->UID == item)
			return *it;
	}

	return nullptr; // no item with the user data found
}

QAction* RibbonToolBarManager::findAction(const QList<QAction*>& acts, const QString& item) const
{
	for (QList<QAction*>::ConstIterator it = acts.begin(); it != acts.end(); ++it) {
		if ((*it)->data().toString() == item)
			return *it;
	}

	return nullptr; // no item with the user data found
}

QList<RibbonToolbar*> RibbonToolBarManager::toolBars() const
{


	return toolbars_;
}




ToolBarItem::DefaultVisibility RibbonToolBarManager::getToolbarPolicy(const QToolBar* toolbar) const
{
	auto* action = toolbar->toggleViewAction();

	QVariant property = action->property("DefaultVisibility");
	if (property.isNull()) {
		return ToolBarItem::DefaultVisibility::Visible;
	}

	return static_cast<ToolBarItem::DefaultVisibility>(property.toInt());
}



void RibbonToolBarManager::setState(const QList<QString>& names, State state)
{
	for (auto& name : names) {
		setState(name, state);
	}
}




void RibbonToolBarManager::setState(const QString& name, State state)
{
	ParameterGrp::handle hPref = App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("MainWindow")->GetGroup("Toolbars");

	auto visibility = [hPref, name](bool defaultvalue) {
		return hPref->GetBool(name.toStdString().c_str(), defaultvalue);
	};

	auto saveVisibility = [hPref, name](bool value) {
		hPref->SetBool(name.toStdString().c_str(), value);
	};

	auto showhide = [visibility](QToolBar* toolbar, ToolBarItem::DefaultVisibility policy) {

		auto show = visibility(policy == ToolBarItem::DefaultVisibility::Visible);

		if (show) {
			toolbar->show();
		}
		else {
			toolbar->hide();
		}
	};

	QToolBar* tb = findToolBar(name);
	if (tb) {

		if (state == State::RestoreDefault) {

			auto policy = getToolbarPolicy(tb);

			if (policy == ToolBarItem::DefaultVisibility::Unavailable) {
				tb->hide();
				tb->toggleViewAction()->setVisible(false);
			}
			else {
				tb->toggleViewAction()->setVisible(true);

				showhide(tb, policy);
			}
		}
		else if (state == State::ForceAvailable) {

			auto policy = getToolbarPolicy(tb);

			tb->toggleViewAction()->setVisible(true);

			// Unavailable policy defaults to a Visible toolbars when made available
			auto show = visibility(policy == ToolBarItem::DefaultVisibility::Visible ||
				policy == ToolBarItem::DefaultVisibility::Unavailable);

			if (show) {
				tb->show();
			}
			else {
				tb->hide();
			}
		}
		else if (state == State::ForceHidden) {
			tb->toggleViewAction()->setVisible(false); // not visible in context menus
			tb->hide(); // toolbar not visible

		}
		else if (state == State::SaveState) {
			auto show = tb->isVisible();
			saveVisibility(show);
		}
	}
}



