/* * Z3D - A structured light 3D scanner
 * Copyright (C) 2013-2016 Nicolas Ulrich <nikolaseu@gmail.com>
 *
 * This file is part of Z3D.
 *
 * Z3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Z3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Z3D.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <ZPointCloud/zpointcloud.h>

#include <memory>

namespace pcl
{
struct PCLPointCloud2;
}

namespace Z3D
{

class ZPointCloudPCLWrapper : public ZPointCloud
{
public:
    static ZPointCloudUniquePtr create(std::unique_ptr<pcl::PCLPointCloud2> pointCloud);

    explicit ZPointCloudPCLWrapper(std::unique_ptr<pcl::PCLPointCloud2> pointCloud,
                                   const std::vector<ZPointField *> &fields,
                                   QObject *parent = nullptr);

    virtual ~ZPointCloudPCLWrapper() override;

    // ZPointCloud interface
    QByteArray vertexData() const override;
    QByteArray trianglesData() const override;

private:
    const std::unique_ptr<pcl::PCLPointCloud2> m_pointCloud;
};

} // namespace Z3D
