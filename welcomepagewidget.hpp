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

private:
	Ui::WelcomePageWidget *ui;
};

#endif // WELCOMEPAGEWIDGET_HPP
