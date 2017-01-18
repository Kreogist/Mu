/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICANALYSISQUEUE_H
#define KNMUSICANALYSISQUEUE_H

#include <QFileInfo>
#include <QLinkedList>

#include "knmusicglobal.h"

#include <QObject>

/*!
 * \brief The KNMusicAnalysisQueue class provides the analysis queue for the
 * music model. It should be used with a searcher, works with KNMusicParser. It
 * will use the parser to parse all the file which is found from the searcher.
 */
class KNMusicAnalysisQueue : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAnalysisQueue object.
     * \param parent The parent object.
     */
    explicit KNMusicAnalysisQueue(QObject *parent = 0);

    /*!
     * \brief Check whether the analysis queue is working.
     * \return If the queue is working, then return true.
     */
    bool isWorking() const;

signals:
    /*!
     * \brief When a file is parsed by the parser, this signal will be emitted.
     * \param analysisItem The parsed result.
     */
    void analysisComplete(KNMusicAnalysisItem analysisItem);

    /*!
     * \brief This is signal is only used to avoid the depth recursion.
     */
    void analysisNext();

public slots:
    /*!
     * \brief Add a new file to the analysis queue. This slot should be
     * connected from the signal which the searcher give out a new file.
     * \param fileInfo The file info of the file which was recognized by a
     * searcher.
     */
    void addFile(const QFileInfo &fileInfo);

    /*!
     * \brief Add new files to the analysis queue. This slot should be connected
     * from the signal which the searcher give out several new files.
     * \param fileInfos The files information list which were recognized by a
     * searcher.
     */
    void addFiles(const QFileInfoList &fileInfos);

private slots:
    void onActionAnalysisNext();

private:
    QFileInfoList m_filePathQueue;
    bool m_isWorking;
};

#endif // KNMUSICANALYSISQUEUE_H
