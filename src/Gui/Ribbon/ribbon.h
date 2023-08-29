

#ifndef RIBBONTABWIDGET_H
#define RIBBONTABWIDGET_H

#include <QTabWidget>
#include <QToolButton>

#ifndef FC_GLOBAL_H
#include <FCGlobal.h>
#endif

class RibbonTabContent;

class GuiExport Ribbon : public QTabWidget
{
	Q_OBJECT
public:
	explicit Ribbon(QWidget* parent = 0);

	/// Add a tab to the ribbon.
	///
	/// \param[in] tabName Name of the tab
	RibbonTabContent* addTab(const QString& tabName);

	/// Add a tab to the ribbon.
	///
	/// \param[in] tabIcon Icon of the tab
	/// \param[in] tabName Name of the tab
	RibbonTabContent* addTab(const QIcon& tabIcon, const QString& tabName);

	/// Remove a tab from the ribbon.
	///
	/// \param[in] tabName Name of the tab
	void removeTab(const QString& tabName);

	/// Gets a tab by its name
	RibbonTabContent* getTab(const QString& tabName);

	/// Gets a tab by its name
	RibbonTabContent* getTabByIndex(int index);

	/// Gets all tabs of the ribbon
	QList<RibbonTabContent*> getTabs();

	/// Add a group to the specified tab.
	/// The specified tab is created if it does not exist.
	///
	/// \param[in] tabName Name of the tab
	/// \param[in] groupName Name of the group
	void addGroup(const QString& tabName, const QString& groupName);

	/// Add a button to the specified group.
	/// The specified group and tab are created if they do not exist.
	///
	/// \param[in] tabName Name of the tab
	/// \param[in] groupName Name of the group
	/// \param[in] button The button
	void addButton(const QString& tabName, const QString& groupName, QToolButton* button);

	/// Remove a button from the specified group.
	/// Do nothing if the button, group or tab doesn't exist.
	/// The button group and tab are also removed if they become empty due to
	/// the removal of the button.
	///
	/// \param[in] tabName Name of the tab
	/// \param[in] groupName Name of the group
	/// \param[in] button The button
	void removeButton(const QString& tabName, const QString& groupName, QToolButton* button);
};

#endif // RIBBONTABWIDGET_H
