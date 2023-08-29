/*
 * Copyright (C) Martijn Koopman
 * All Rights Reserved
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 */

#include "ribbonbuttongroup.h"


#include <QToolButton>
#include "Ribbon.h"

RibbonButtonGroup::RibbonButtonGroup(QWidget* parent)
	: QWidget(parent)

	, m_title("")
{

	setGeometry(0, 0, 150, 37);


	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(gridLayout);

	// QVBoxLayout* verticalLayout = new QVBoxLayout();
	QGridLayout* verticalLayout = new QGridLayout();
	verticalLayout->setSpacing(0);
	verticalLayout->setContentsMargins(2, 2, 2, 2);


	gridLayout->addLayout(verticalLayout, 0, 0);



	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);

	verticalLayout->addLayout(horizontalLayout, 0, 0);

	auto horizontalLine = new QFrame();
	horizontalLine->setFrameShape(QFrame::HLine);
	horizontalLine->setFrameShadow(QFrame::Plain);
	horizontalLine->setStyleSheet("color: #c0c0c0;");

	verticalLayout->addWidget(horizontalLine, 1, 0);

	label = new QLabel();
	label->setText("Toolbar");
	label->setAlignment(Qt::AlignCenter);
	label->setMargin(0);
	label->setContentsMargins(0, 2, 0, 2);
	label->setMaximumHeight(20);
	verticalLayout->addWidget(label, 2, 0);


	auto verticalLine = new QFrame();
	verticalLine->setFrameShape(QFrame::VLine);
	verticalLine->setFrameShadow(QFrame::Plain);
	verticalLine->setStyleSheet("color: #c0c0c0;");
	gridLayout->addWidget(verticalLine, 0, 1);

}


RibbonButtonGroup::~RibbonButtonGroup()
{

}

void RibbonButtonGroup::setTitle(const QString& title)
{
	m_title = title;
	label->setText(m_title);
}

QString RibbonButtonGroup::title() const
{
	return m_title;
}

int RibbonButtonGroup::buttonCount() const
{
	return horizontalLayout->count();
}

void RibbonButtonGroup::addButton(QToolButton* button)
{
	button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	button->setMinimumSize(48, 48);
	button->setAutoRaise(true);
	button->setIconSize(QSize(32, 32));
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	horizontalLayout->addWidget(button);
}


void RibbonButtonGroup::addWidget(QWidget* widget)
{
	widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	horizontalLayout->addWidget(widget);
}

void RibbonButtonGroup::addLayout(QLayout* layout)
{
 	horizontalLayout->addLayout(layout);
}
 


void RibbonButtonGroup::addToolbar(QToolBar* toolbar)
{
	toolbar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	horizontalLayout->addWidget(toolbar);
}


void RibbonButtonGroup::removeButton(QToolButton* button)
{
	/// \todo What happens if button is not part of the layout?
	horizontalLayout->removeWidget(button);
}
