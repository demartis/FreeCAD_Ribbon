/*
 * Copyright (C) Martijn Koopman
 * All Rights Reserved
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 */

#include <QApplication>
#include <QScrollArea>

#include "ribbontabcontent.h"
#include "ribbonbuttongroup.h"

RibbonTabContent::RibbonTabContent(QWidget* parent)
	: QWidget(parent)

{
		//QColor bg = qApp->palette().color(QPalette::Window);

		//setStyleSheet(QString::fromLatin1(" background-color :   rgb(%1, %2, %3);").arg(bg.red()).arg(bg.green()).arg(bg.blue()));
		setStyleSheet(QString::fromLatin1(" background-color :   #F0F0F1;"));
	
		
		setGeometry(0, 0, 400, 94);
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(gridLayout);

	QScrollArea* ribbonTabScrollArea = new QScrollArea();
	gridLayout->addWidget(ribbonTabScrollArea, 0, 0);
	ribbonTabScrollArea->setFrameShape(QFrame::NoFrame);
	ribbonTabScrollArea->setFrameShadow(QFrame::Plain);
	ribbonTabScrollArea->setLineWidth(0);
	ribbonTabScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ribbonTabScrollArea->setWidgetResizable(true);


	QWidget* ribbonTabScrollAreaContent = new QWidget();
	ribbonTabScrollArea->setWidget(ribbonTabScrollAreaContent);
	ribbonTabScrollAreaContent->setGeometry(0, 0, 400, 94);
	QGridLayout* gridLayout_2 = new QGridLayout();
	gridLayout_2->setSpacing(0);
	gridLayout_2->setContentsMargins(0, 0, 0, 0);
	ribbonTabScrollAreaContent->setLayout(gridLayout_2);
	QWidget* spacer = new QWidget(this);
	gridLayout_2->addWidget(spacer, 0, 1);
	spacer->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);


	ribbonHorizontalLayout = new QHBoxLayout();
	gridLayout_2->addLayout(ribbonHorizontalLayout, 0, 0);
	ribbonHorizontalLayout->setSpacing(0);
	ribbonHorizontalLayout->setContentsMargins(0, 0, 0, 0);
}

RibbonTabContent::~RibbonTabContent()
{

}

RibbonButtonGroup* RibbonTabContent::addGroup(const QString& groupName)
{
	RibbonButtonGroup* ribbonButtonGroup = new RibbonButtonGroup;
	ribbonButtonGroup->setTitle(groupName);

	ribbonHorizontalLayout->addWidget(ribbonButtonGroup);
	return ribbonButtonGroup;
}


QList<RibbonButtonGroup*> RibbonTabContent::getGroups()
{
	QList<RibbonButtonGroup*> groups;
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		groups.append(group);
	}
	return groups;
}

void RibbonTabContent::removeGroup(const QString& groupName)
{
	// Find ribbon group
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			ribbonHorizontalLayout->removeWidget(group); /// \todo :( No effect
			delete group;
			break;
		}
	}

	/// \todo  What if the group still contains buttons? Delete manually?
	// Or automaticly deleted by Qt parent() system.
}



RibbonButtonGroup* RibbonTabContent::getGroupByIndex(int index)
{
	// Find ribbon group
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (i==index)
		{
			return group;
		}
	}
	return nullptr;
}


RibbonButtonGroup* RibbonTabContent::getGroup(const QString& groupName)
{
	// Find ribbon group
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			return group;
		}
	}
	return nullptr;
}

int RibbonTabContent::groupCount() const
{
	return ribbonHorizontalLayout->count();
}

void RibbonTabContent::addButton(const QString& groupName, QToolButton* button)
{
	// Find ribbon group
	RibbonButtonGroup* ribbonButtonGroup = nullptr;
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			ribbonButtonGroup = group;
			break;
		}
	}

	if (ribbonButtonGroup != nullptr)
	{
		// Group found
		// Add ribbon button
		ribbonButtonGroup->addButton(button);
	}
	else
	{
		// Group not found
		// Add ribbon group
		addGroup(groupName);

		// Add ribbon button
		addButton(groupName, button);
	}
}


void RibbonTabContent::addAction(const QString& groupName, QAction* action)
{
	// Find ribbon group
	RibbonButtonGroup* ribbonButtonGroup = nullptr;
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			ribbonButtonGroup = group;
			break;
		}
	}

	if (ribbonButtonGroup != nullptr)
	{
		// Group found
		// Add ribbon button
		ribbonButtonGroup->addAction(action);
	}
	else
	{
		// Group not found
		// Add ribbon group
		addGroup(groupName);

		// Add ribbon button
		addAction(groupName, action);
	}
}

void RibbonTabContent::removeButton(const QString& groupName, QToolButton* button)
{
	// Find ribbon group
	RibbonButtonGroup* ribbonButtonGroup = nullptr;
	for (int i = 0; i < ribbonHorizontalLayout->count(); i++)
	{
		RibbonButtonGroup* group = static_cast<RibbonButtonGroup*>(ribbonHorizontalLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			ribbonButtonGroup = group;
			break;
		}
	}

	if (ribbonButtonGroup != nullptr)
	{
		// Group found
		// Remove ribbon button
		ribbonButtonGroup->removeButton(button);

		if (ribbonButtonGroup->buttonCount() == 0)
		{
			// Empty button group
			// Remove button group
			removeGroup(groupName);
		}
	}
}
