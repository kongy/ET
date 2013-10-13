#ifndef NEWSOLUTIONDIALOG_HPP
#define NEWSOLUTIONDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewSolutionDialog;
}

class NewSolutionDialog : public QDialog {
	Q_OBJECT

public:
	explicit NewSolutionDialog(QWidget *parent = 0);
	~NewSolutionDialog();

private:
	Ui::NewSolutionDialog *ui;
};

#endif // NEWSOLUTIONDIALOG_HPP
