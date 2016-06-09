#include "dialog_app.h"

dialog_app::dialog_app(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

dialog_app::~dialog_app()
{

}
