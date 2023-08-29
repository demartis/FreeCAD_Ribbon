
#ifndef RIBBONTABCONTENT_H
#define RIBBONTABCONTENT_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>


#ifndef FC_GLOBAL_H
#include <FCGlobal.h>
#endif


class RibbonButtonGroup;

class GuiExport RibbonTabContent : public QWidget
{
	Q_OBJECT

public:
	explicit RibbonTabContent(QWidget* parent = 0);
	virtual ~RibbonTabContent();

	/// Add a group to the tab content.
	///
	/// \param[in] groupName Name of the group
	RibbonButtonGroup* addGroup(const QString& groupName);

	/// Remove a group from the tab content.
	///
	/// \param[in] groupName Name of the group
	void removeGroup(const QString& groupName);

	/// Gets a group from the tab content.
	RibbonButtonGroup* getGroup(const QString& groupName);

	/// Gets a group from the tab content.
	RibbonButtonGroup* getGroupByIndex(int index);

	/// Gets all groups from the tab.
	QList<RibbonButtonGroup*> getGroups();

	/// Get the number of button groups in this tab content.
	///
	/// \return The number of button groups
	int groupCount() const;

	/// Add a button to the specified group.
	/// The group is created if it does not exist.
	///
	/// \param[in] groupName Name of the group
	/// \param[in] button The button
	void addButton(const QString& groupName, QToolButton* button);
	void addAction(const QString& groupName, QAction* action);

	/// Remove a button from the specified group.
	/// The group is also removed if it's empty.
	///
	/// \param[in] groupName Name of the group
	/// \param[in] button The button

	void removeButton(const QString& groupName, QToolButton* button);
private:
	QHBoxLayout* ribbonHorizontalLayout{ nullptr };
};

#endif // RIBBONTABCONTENT_H
