#ifndef EMBEDDEDPUSHBUTTON_HPP
#define EMBEDDEDPUSHBUTTON_HPP

#include <QPushButton>

template <class T>
class EmbeddedPushButton : public QPushButton
{
	T extra;
public:
	explicit EmbeddedPushButton(const QString &text, T extra, QWidget *parent = 0):
		QPushButton(text, parent),
		extra(extra) { }

	T getExtra() {
		return extra;
	}
};

#endif // EMBEDDEDPUSHBUTTON_HPP
