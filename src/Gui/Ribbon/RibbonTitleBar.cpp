
#include "RibbonTitlebar.h"

#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include <QApplication>
#include <QPainter>

#include "Application.h"
#include "Command.h"
#include "MainWindow.h"

// -----------------------------------------------------------



RibbonTitlebar::RibbonTitlebar(QWidget* parent)
	: QWidget(parent)
{
	Gui::MainWindow* mw = Gui::getMainWindow();
	assert(mw);

	QMenuBar* menu = mw->menuBar();
	if (menu) {
		// menu->hide();
	}


	QVBoxLayout* mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	QHBoxLayout* hlayout = new QHBoxLayout();
	auto picon = App::Application::Config()["ProgramLogo"];
	QPixmap pixmap(QString::fromLatin1(picon.c_str()));  

	QLabel* icon = new QLabel();
	icon->setPixmap(pixmap.scaled(55, 55, Qt::KeepAspectRatio));  
	icon->setStyleSheet(QString::fromLatin1("QLabel { position: absolute; left: 5px; top: 20px; margin-top: 4px; background-color : #2470BF; color : black; }"));

	auto title = App::Application::Config()["WindowTitle"];
	QLabel* label = new QLabel(QString::fromLatin1(title.c_str()).toUpper());
	label->setStyleSheet(QString::fromLatin1("QLabel { margin-left: 10px; margin-top: 4px;font-size:14px; font-weight: bold; background-color : #2470BF; color : white;  }"));


	hlayout->addWidget(icon);
	hlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	hlayout->addWidget(label);
	hlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	resizeButton_ = new QToolButton(mw);
	if (mw->isMaximized() || mw->isFullScreen())
	{
		resizeButton_->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarMinButton), QColor::fromRgb(235, 235, 235)));
	}
	else
	{
		resizeButton_->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarMaxButton), QColor::fromRgb(235, 235, 235)));
	}

	resizeButton_->setStyleSheet(QString::fromLatin1("QToolButton { border: 1px solid rgb(235,235,235); color: white; }"));

	QToolButton* closeButton = new QToolButton(mw);

	closeButton->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarCloseButton), QColor::fromRgb(235, 235, 235)));
	closeButton->setStyleSheet(QString::fromLatin1("QToolButton { margin-left: 4px; margin-right: 10px; border: 1px solid rgb(235,235,235);}"));

	//border: 1px solid rgb(235,235,235); color: white; 

	QObject::connect(resizeButton_, &QToolButton::clicked, this, &RibbonTitlebar::maximizeRestore);
	QObject::connect(closeButton, &QToolButton::clicked, [this]() {
		qApp->exit();
		});


	hlayout->addWidget(resizeButton_);
	hlayout->addWidget(closeButton);



	mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	mainLayout->addLayout(hlayout);
	mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

}


void RibbonTitlebar::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		dragPosition_ = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void RibbonTitlebar::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		Gui::MainWindow* mw = Gui::getMainWindow();
		if (!mw) return;
		mw->move(event->globalPos() - dragPosition_);
		event->accept();
	}
}

void RibbonTitlebar::maximizeRestore()
{
	Gui::MainWindow* mw = Gui::getMainWindow();
	if (!mw) return;
	if (mw->isMaximized() || mw->isFullScreen()) {
		mw->showNormal();

		resizeButton_->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarMaxButton), QColor::fromRgb(235, 235, 235)));
	}
	else
		if (mw->isMinimized()) {
			mw->showMaximized();

			resizeButton_->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarMinButton), QColor::fromRgb(235, 235, 235)));
		}
		else {
			mw->showMaximized();

			resizeButton_->setIcon(createIconWithColor(mw->style()->standardIcon(QStyle::SP_TitleBarMinButton), QColor::fromRgb(235, 235, 235)));
		}
}

QIcon RibbonTitlebar::createIconWithColor(const QIcon& icon, const QColor& color)
{
	QPixmap pixmap = icon.pixmap(icon.availableSizes().first());
	QImage image = pixmap.toImage();
	QPainter painter(&image);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(image.rect(), color);
	painter.end();
	return QIcon(QPixmap::fromImage(image));
}
