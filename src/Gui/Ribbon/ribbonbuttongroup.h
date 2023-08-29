

#ifndef RIBBONBUTTONGROUP_H
#define RIBBONBUTTONGROUP_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QToolbar>

#ifndef FC_GLOBAL_H
#include <FCGlobal.h>
#endif


class GuiExport RibbonButtonGroup : public QWidget
{
	Q_OBJECT

public:
	explicit RibbonButtonGroup(QWidget* parent = 0);
	virtual ~RibbonButtonGroup();

	/// Set the title of the button group.
	/// The title is shown underneath the buttons.
	///
	/// \param[in] title The title
	void setTitle(const QString& title);

	/// Get the title of the button group.
	///
	/// \return The title
	QString title() const;

	/// Get the number of buttons in the button group.
	///
	/// \return The number of buttons
	int buttonCount() const;

	/// Add a button to the group.
	///
	/// \param[in] button The button
	void addButton(QToolButton* button);

	/// Add a toolbar to the group.
	void addToolbar(QToolBar* toolbar);

	/// Add a Widget to the group.
	void addWidget(QWidget* widget);

	/// Add a layout to the group.
	void addLayout(QLayout* layout);

	/// Remove a button from the group.
	///
	/// \param[in] button The button
	void removeButton(QToolButton* button);

private:
	QLabel* label;
	QHBoxLayout* horizontalLayout;
	QString m_title; ///< Title of the button group
};

#endif // RIBBONBUTTONGROUP_H
