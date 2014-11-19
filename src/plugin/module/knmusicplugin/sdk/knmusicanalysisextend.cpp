/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicmodelassist.h"

#include "knmusicanalysisextend.h"

#include <QDebug>

KNMusicAnalysisExtend::KNMusicAnalysisExtend(QObject *parent) :
    QObject(parent)
{
}

void KNMusicAnalysisExtend::onActionAnalysisComplete(const KNMusicDetailInfo &detailInfo)
{
    //Add this detail to model.
    emit requireAppendRow(KNMusicModelAssist::generateRow(detailInfo));
}
