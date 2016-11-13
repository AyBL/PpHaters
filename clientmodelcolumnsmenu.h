#ifndef MODELCOLUMNSMENU_H
#define MODELCOLUMNSMENU_H

#include <gtkmm/treemodel.h>

class ModelColumnsMenu : public Gtk::TreeModel::ColumnRecord{
  	public:

    	ModelColumnsMenu();

    	Gtk::TreeModelColumn<unsigned char> m_col_id;
    	Gtk::TreeModelColumn<Glib::ustring> m_col_name;
};

#endif