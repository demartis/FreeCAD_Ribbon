 


#ifndef RIBBONTOOLBAR_H
#define RIBBONTOOLBAR_H


#include <QToolBar>


#ifndef FC_GLOBAL_H
#include <FCGlobal.h>
#endif


class GuiExport RibbonToolbar : public QToolBar
{
	Q_OBJECT

public:
	explicit RibbonToolbar(QWidget* parent = nullptr);
	QString UID;
};

#endif // RIBBONTOOLBAR_H
