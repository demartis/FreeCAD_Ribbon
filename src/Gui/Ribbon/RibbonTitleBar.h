


#ifndef RIBBONTITLEBAR_H
#define RIBBONTITLEBAR_H


#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>


#ifndef FC_GLOBAL_H
#include <FCGlobal.h>
#endif


class GuiExport RibbonTitlebar : public QWidget
{
	Q_OBJECT

public:
	explicit RibbonTitlebar(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	QIcon createIconWithColor(const QIcon& icon, const QColor& color);
private Q_SLOTS:
	void maximizeRestore();
private:
	QPoint dragPosition_;
	QToolButton* resizeButton_;
};

#endif // RIBBONTITLEBAR_H
