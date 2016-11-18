#ifndef MODELCOLUMNS_H
#define MODELCOLUMNS_H

#include <gtkmm/treemodel.h>

class ModelColumns : public Gtk::TreeModel::ColumnRecord{
  	public:

    	ModelColumns();

    	Gtk::TreeModelColumn<int> m_col_id;
    	Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    	Gtk::TreeModelColumn<Glib::ustring> m_col_value;
    	Gtk::TreeModelColumn<bool> m_col_mutable;
        Gtk::TreeModelColumn<Glib::ustring> m_col_parent;
};

#endif
