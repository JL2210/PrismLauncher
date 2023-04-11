// SPDX-License-Identifier: GPL-3.0-only
/*
 *  PolyMC - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LocalModGetTask.h"

#include "FileSystem.h"
#include "minecraft/mod/MetadataHandler.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

LocalModGetTask::LocalModGetTask(QDir index_dir, QVariant addonId) : m_index_dir(index_dir), m_addonId(addonId)
{
    // Ensure a '.index' folder exists in the mods folder, and create it if it does not
    if (!FS::ensureFolderPathExists(index_dir.path())) {
        emitFailed(QString("Unable to create index for modId %1!").arg(m_addonId.toString()));
    }

#ifdef Q_OS_WIN32
    SetFileAttributesW(index_dir.path().toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
#endif
}

void LocalModGetTask::executeTask()
{
    setStatus(tr("Updating index for modId:\n%1").arg(m_addonId.toString()));
    emit getMod(Metadata::get(m_index_dir, m_addonId));
}

auto LocalModGetTask::abort() -> bool
{
    emitAborted();
    return true;
}
