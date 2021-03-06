/******************************************************************************
 * This file is part of dOpenCL.
 * 
 * dOpenCL is an implementation of the OpenCL application programming
 * interface for distributed systems. See <http://dopencl.uni-muenster.de/>
 * for more information.
 * 
 * Developed by: Research Group Parallel and Distributed Systems
 *               Department of Mathematics and Computer Science
 *               University of Muenster, Germany
 *               <http://pvs.uni-muenster.de/>
 * 
 * Copyright (C) 2013  Philipp Kegel <philipp.kegel@uni-muenster.de>
 *
 * dOpenCL is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dOpenCL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with dOpenCL. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Permission to use dOpenCL for scientific, non-commercial work is
 * granted under the terms of the dOpenCL Academic License provided
 * appropriate credit is given. See the dOpenCL Academic License for
 * more details.
 * 
 * You should have received a copy of the dOpenCL Academic License
 * along with dOpenCL. If not, see <http://dopencl.uni-muenster.de/>.
 ******************************************************************************/

/*!
 * \file ProgramBuildListenerImpl.cpp
 *
 * \date 2013-11-14
 * \author Philipp Kegel
 */

#include "ProgramBuildListenerImpl.h"

#include "DeviceImpl.h"
#include "HostImpl.h"

#include "message/ProgramBuildMessage.h"

#include <dcl/DCLTypes.h>
#include <dcl/Device.h>
#include <dcl/Remote.h>

#include <dcl/util/Logger.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <cassert>
#include <memory>
#include <ostream>
#include <vector>

namespace dclasio {

ProgramBuildListenerImpl::ProgramBuildListenerImpl(
        dcl::object_id id, HostImpl& host) : Remote(id), _host(host) {
}

ProgramBuildListenerImpl::~ProgramBuildListenerImpl() {
}

void ProgramBuildListenerImpl::onComplete(
        const std::vector<dcl::Device *>& devices,
        const std::vector<cl_build_status>& buildStatus) {
    std::vector<dcl::object_id> deviceIds;

    deviceIds.reserve(devices.size());
    for (auto device : devices) {
        auto deviceImpl = dynamic_cast<DeviceImpl *>(device);
        assert(deviceImpl != nullptr);
        deviceIds.push_back(deviceImpl->remoteId());
    }

    message::ProgramBuildMessage message(_id, deviceIds, buildStatus);
    _host.sendMessage(message);
    /* TODO Handle errors */

    dcl::util::Logger << dcl::util::Debug
            << "Sent update of program build status (ID=" << _id
            << ')' << std::endl;
}

} /* namespace dclasio */
