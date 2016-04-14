#include <QtGui>
#include <QApplication>
#include <QDialog>
#include <QLabel>
// #include <qbuttongroup.h>

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QDialog *dd = new QDialog();
	QLabel *label = new QLabel(dd);


	// QPushButton *btn = new QPushButton("a", )

	label->setText("Hello World!\n1\n2\n3");
	dd->show();
	QDialog *w = new QDialog();
	w->show();
	return app.exec();

}


