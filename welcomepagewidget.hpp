#ifndef WELCOMEPAGEWIDGET_HPP
#define WELCOMEPAGEWIDGET_HPP

#include <QWidget>

namespace Ui {
class WelcomePageWidget;
}

class WelcomePageWidget : public QWidget
{
	Q_OBJECT

public:
	explicit WelcomePageWidget(QWidget *parent = 0);
	~WelcomePageWidget();

signals:
	void startNewSolution();

private:
	Ui::WelcomePageWidget *ui;

private slots:
	void startNewSolutionSlot();
};

#endif // WELCOMEPAGEWIDGET_HPP
