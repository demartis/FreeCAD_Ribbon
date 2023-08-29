


#ifndef GUI_RIBBONTOOLBARMANAGER_H
#define GUI_RIBBONTOOLBARMANAGER_H

#include <string>
#include <QStringList>
#include <QToolbar>

#include "../ToolBarManagerInterface.h"

class QAction;
class QToolBar;
class Ribbon;
class QDockWidget;
class RibbonTabContent;
class RibbonButtonGroup;
class RibbonToolbar;

namespace Gui {

	class ToolBarItem;



	/**
	 * The RibbonToolBarManager class is responsible for the creation of toolbars and appending them
	 * to the main window.
	 * @see ToolBoxManager
	 * @see MenuManager
	 * @author Fabrice Lainard
	 */
	class GuiExport RibbonToolBarManager : public Gui::ToolBarManagerInterface
	{
	public:

		RibbonToolBarManager();
		~RibbonToolBarManager();
		void setup(const QString& workbenchName, ToolBarItem*) override;
		void saveState() const override;
		void restoreState() const override;
		void retranslate() const override;
		void setMovable(bool movable) const override;
		void setState(const QList<QString>& names, State state) override;
		void setState(const QString& name, State state) override;
		ToolBarItem::DefaultVisibility getToolbarPolicy(const QToolBar *) const override;
		Ribbon* getRibbon() { return ribbon_; }
		static std::function<QString(const QString&, const QString&)> getTabNameCallback;
	protected:
		void setup(ToolBarItem*, RibbonToolbar*) const;
		QList<RibbonToolbar*> toolBars() const;
		QString getTabName(const QString& workbenchName, const QString& groupLabel);
		RibbonToolbar* findToolBar(const QString&) const;
		QAction* findAction(const QList<QAction*>&, const QString&) const;
		void createRibbon();
		RibbonToolbar* createToolbar(const QString& workbenchName, const QString& label);
	private:
		QStringList toolbarNames;
		QDockWidget* ribbonDockWidget_{ nullptr };
		Ribbon* ribbon_{ nullptr };
		QList<RibbonToolbar*> toolbars_;
	};

} // namespace Gui


#endif // GUI_RIBBONTOOLBARMANAGER_H
