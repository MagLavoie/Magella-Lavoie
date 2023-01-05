//
// Z3D - A structured light 3D scanner
// Copyright (C) 2013-2016 Nicolas Ulrich <nikolaseu@gmail.com>
//
// This file is part of Z3D.
//
// Z3D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Z3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Z3D.  If not, see <http://www.gnu.org/licenses/>.
//

#include "ZCore/zcoreplugin.h"

#include "ZCore/zlogging.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QPluginLoader>

Z3D_LOGGING_CATEGORY_FROM_FILE("z3d.zcore", QtDebugMsg)

namespace Z3D
{

ZCorePlugin::ZCorePlugin(const QString &fileName)
    : m_loader(new QPluginLoader(fileName))
{

}

ZCorePlugin::~ZCorePlugin()
{
    delete m_loader;
}

QString ZCorePlugin::id() const
{
    const QJsonObject data = metaData();
    if (data.contains("id")) {
        /// if id set explicitly by plugin developer in metadata, use it
        return data["id"].toString();
    }

    if (auto inst = qobject_cast<QObject *>(m_pluginInstance)) {
        /// otherwise, if plugin is loaded, use class name
        return inst->metaObject()->className();
    }

    /// as last resort use filename
    return m_loader->fileName();
}

QString ZCorePlugin::version() const
{
    const QJsonObject data = metaData();
    if (data.contains("version")) {
        /// if version set explicitly by plugin developer in metadata, use it
        return data["version"].toString();
    }

    /// otherwise use library version
    return QLatin1String(Z3D_VERSION_STR);
}

QJsonObject ZCorePlugin::metaData() const
{
    return m_loader->metaData()["MetaData"].toObject();
}

bool ZCorePlugin::load()
{
    QElapsedTimer timer;
    timer.start();

    if (!m_loader->load()) {
        zWarning() << "error loading plugin" << m_loader->fileName()
                   << "->" << m_loader->errorString();
        return false;
    }

    m_pluginInstance = m_loader->instance();

    zDebug() << "loaded plugin id:" << id()
             << "version:" << version()
             << "in" << timer.elapsed() << "ms";

    return true;
}

void ZCorePlugin::unload()
{
    //m_pluginInstance->deleteLater(); /// docs: If you want to ensure that the root component is deleted, you should call unload() as soon you don't need to access the core component anymore. When the library is finally unloaded, the root component will automatically be deleted
    m_pluginInstance = nullptr;
    m_loader->unload();
}

QString ZCorePlugin::errorString()
{
    return m_loader->errorString();
}

} // namespace Z3D
