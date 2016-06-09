#ifndef DIALOG_APP_H
#define DIALOG_APP_H

#include <QtGui/QDialog>
#include "ui_dialog_app.h"

class dialog_app : public QDialog
{
	Q_OBJECT

public:
	dialog_app(QWidget *parent = 0, Qt::WFlags flags = 0);
	~dialog_app();

private:
	Ui::dialog_appClass ui;
};

#endif // DIALOG_APP_H
