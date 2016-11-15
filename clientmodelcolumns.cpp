#include "clientmodelcolumns.h"


ModelColumns::ModelColumns(){
	add(m_col_id);
	add(m_col_name);
	add(m_col_value);
	add(m_col_mutable);
	add(m_col_type);
	add(m_col_parent);
}
