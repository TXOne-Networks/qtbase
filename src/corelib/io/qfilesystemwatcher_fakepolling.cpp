/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qfilesystemwatcher_fakepolling_p.h"
#include <QtCore/qtimer.h>

#ifndef QT_NO_FILESYSTEMWATCHER

QT_BEGIN_NAMESPACE

QFakePollingFileSystemWatcherEngine::QFakePollingFileSystemWatcherEngine(QObject *parent)
    : QFileSystemWatcherEngine(parent),
      timer(this)
{
    connect(&timer, SIGNAL(timeout()), SLOT(timeout()));
}

QStringList QFakePollingFileSystemWatcherEngine::addPaths(const QStringList &paths,
                                                      QStringList *files,
                                                      QStringList *directories)
{
    QStringList p = paths;
    QMutableListIterator<QString> it(p);
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo fi(path);
        if (!fi.exists())
            continue;
        if (fi.isDir()) {
            if (directories->contains(path))
                continue;
            directories->append(path);
            if (!path.endsWith(QLatin1Char('/')))
                fi = QFileInfo(path + QLatin1Char('/'));
            this->directories.insert(path, fi);
        } else {
            if (files->contains(path))
                continue;
            files->append(path);
            this->files.insert(path, fi);
        }
        it.remove();
    }

#if 0
    if ((!this->files.isEmpty() ||
         !this->directories.isEmpty()) &&
        !timer.isActive()) {
        timer.start(PollingInterval);
    }
#endif

    return p;
}

QStringList QFakePollingFileSystemWatcherEngine::removePaths(const QStringList &paths,
                                                         QStringList *files,
                                                         QStringList *directories)
{
    QStringList p = paths;
    QMutableListIterator<QString> it(p);
    while (it.hasNext()) {
        QString path = it.next();
        if (this->directories.remove(path)) {
            directories->removeAll(path);
            it.remove();
        } else if (this->files.remove(path)) {
            files->removeAll(path);
            it.remove();
        }
    }

    if (this->files.isEmpty() &&
        this->directories.isEmpty()) {
        timer.stop();
    }

    return p;
}

void QFakePollingFileSystemWatcherEngine::timeout()
{

}

QT_END_NAMESPACE
#endif // !QT_NO_FILESYSTEMWATCHER

