


#ifndef GUI_TOOLBARMANAGERINTERFACE_H
#define GUI_TOOLBARMANAGERINTERFACE_H

#include <string>
#include <QStringList>

class QAction;
class QToolBar;

namespace Gui {




	class GuiExport ToolBarItem
	{
	public:
		/** Manages the default visibility status of a toolbar item, as well as the default status
		 * of the toggleViewAction usable by the contextual menu to enable and disable its visibility
		*/
		enum class DefaultVisibility {
			Visible,     // toolbar is hidden by default, visibility toggle action is enabled
			Hidden,      // toolbar hidden by default, visibility toggle action is enabled
			Unavailable, // toolbar visibility is managed independently by client code and defaults to
						 // hidden, visibility toggle action is disabled by default (it is unavailable
						 // to the UI). Upon being forced to be available, these toolbars default to
						 // visible.
		};

		ToolBarItem();
		explicit ToolBarItem(ToolBarItem* item, DefaultVisibility visibilityPolicy = DefaultVisibility::Visible);
		~ToolBarItem();

		void setCommand(const std::string&);
		const std::string& command() const;

		bool hasItems() const;
		ToolBarItem* findItem(const std::string&);
		ToolBarItem* copy() const;
		uint count() const;

		void appendItem(ToolBarItem* item);
		bool insertItem(ToolBarItem*, ToolBarItem* item);
		void removeItem(ToolBarItem* item);
		void clear();

		ToolBarItem& operator << (ToolBarItem* item);
		ToolBarItem& operator << (const std::string& command);
		QList<ToolBarItem*> getItems() const;

		DefaultVisibility visibilityPolicy;

	private:
		std::string _name;
		QList<ToolBarItem*> _items;
	};

	/**
	 * Interface for ToolBarManager implementation.
	 * @see ToolBoxManager
	 * @see MenuManager
	 * @author Fabrice Lainard
	 */
	class ToolBarManagerInterface
	{
	public:
		enum class State {
			ForceHidden,    // Forces a toolbar to hide and hides the toggle action
			ForceAvailable, // Forces a toolbar toggle action to show, visibility depends on user config
			RestoreDefault, // Restores a toolbar toggle action default, visibility as user config
			SaveState,      // Saves the state of the toolbars
		};

		virtual ~ToolBarManagerInterface() = default;
		virtual void setup(const QString& workbenchName, ToolBarItem*) = 0;
		virtual void saveState() const = 0;;
		virtual void restoreState() const = 0;;
		virtual void retranslate() const = 0;;
		virtual void setMovable(bool movable) const = 0;;
		virtual void setState(const QList<QString>& names, State state) = 0;
		virtual void setState(const QString& name, State state) = 0;
		virtual ToolBarItem::DefaultVisibility getToolbarPolicy(const QToolBar*) const = 0;
	};

} // namespace Gui


#endif // GUI_TOOLBARMANAGERINTERFACE_H
