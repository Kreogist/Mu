/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knfocuslineedit.h"

void KNFocusLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    //When getting a focus, emit a signal.
    emit focusIn();
}

void KNFocusLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    //When losing a focus, emit a signal.
    emit focusOut();
}
