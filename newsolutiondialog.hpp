#ifndef NEWSOLUTIONDIALOG_HPP
#define NEWSOLUTIONDIALOG_HPP

#include <QDialog>
#include <QAbstractButton>

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
private slots:
    void onClick(QAbstractButton* btn);
};

#endif // NEWSOLUTIONDIALOG_HPP
