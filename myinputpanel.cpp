/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "myinputpanel.hpp"

MyInputPanel::MyInputPanel()
	: QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint),
	  lastFocusedWidget(0)
{
	form.setupUi(this);

	connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
			this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

	signalMapper.setMapping(form.panelButton_and, form.panelButton_and);
	signalMapper.setMapping(form.panelButton_or, form.panelButton_or);
	signalMapper.setMapping(form.panelButton_not, form.panelButton_not);
	signalMapper.setMapping(form.panelButton_truth, form.panelButton_truth);
	signalMapper.setMapping(form.panelButton_falsity, form.panelButton_falsity);
	signalMapper.setMapping(form.panelButton_implies, form.panelButton_implies);
	signalMapper.setMapping(form.panelButton_equiv, form.panelButton_equiv);
	signalMapper.setMapping(form.panelButton_forall, form.panelButton_forall);
	signalMapper.setMapping(form.panelButton_exist, form.panelButton_exist);
	signalMapper.setMapping(form.panelButton_equal, form.panelButton_equal);

	connect(form.panelButton_and, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_or, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_not, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_truth, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_falsity, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_implies, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_equiv, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_forall, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_exist, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_equal, SIGNAL(clicked()), &signalMapper, SLOT(map()));

	connect(&signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(buttonClicked(QWidget*)));
}

bool MyInputPanel::event(QEvent *e)
{
	switch (e->type()) {
	case QEvent::WindowActivate:
		if (lastFocusedWidget) {
			lastFocusedWidget->activateWindow();
		}
		break;
	default:
		break;
	}

	return QWidget::event(e);
}

void MyInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
	if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
		lastFocusedWidget = newFocus;
	}
}

void MyInputPanel::buttonClicked(QWidget *w)
{
	QString str = qvariant_cast<QString>(w->property("buttonValue"));
	emit stringGenerated(str);
}
