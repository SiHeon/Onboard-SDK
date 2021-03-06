/** @file dji_waypoint.cpp
 *  @version 3.3
 *  @date April 2017
 *
 *  @brief Implementation of GPS Waypoint Missions for DJI OSDK
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "dji_waypoint.hpp"
#include "dji_mission_manager.hpp"
#include "dji_vehicle.hpp"

using namespace DJI;
using namespace DJI::OSDK;

WaypointMission::WaypointMission(Vehicle* vehicle)
  : MissionBase(vehicle)
  , index(NULL)
{
  wayPointEventCallback.callback = 0;
  wayPointEventCallback.userData = 0;
  wayPointCallback.callback      = 0;
  wayPointCallback.userData      = 0;
}

WaypointMission::~WaypointMission()
{
  if (index)
  {
    delete[] (index);
  }
}

void
WaypointMission::init(WayPointInitSettings* Info, VehicleCallBack callback,
                      UserData userData)
{
  if (Info)
    setInfo(*Info);

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = MissionManager::missionCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointInit,
      &info, sizeof(info), 500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::init(WayPointInitSettings* Info, int timeout)
{
  ACK::ErrorCode ack;

  if (Info)
  {
    setInfo(*Info);
  }

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointInit, &info, sizeof(info),
      timeout * 1000 / 2, 2);

}

void
WaypointMission::start(VehicleCallBack callback, UserData userData)
{
  uint8_t start = 0;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = MissionManager::missionCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetStart, &start, sizeof(start),
      500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::start(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        start = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetStart, &start, sizeof(start),
      timeout * 1000 / 2, 2);
}

void
WaypointMission::stop(VehicleCallBack callback, UserData userData)
{
  uint8_t stop = 1;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = MissionManager::missionCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetStart, &stop, sizeof(stop),
      500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::stop(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        stop = 1;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetStart, &stop, sizeof(stop),
      timeout * 1000 / 2, 2);
}

void
WaypointMission::pause(VehicleCallBack callback, UserData userData)
{
  uint8_t data = 0;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = MissionManager::missionCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetPause, &data, sizeof(data),
      500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::pause(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        data = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetPause, &data, sizeof(data),
      timeout * 1000 / 2, 2);
}

void
WaypointMission::resume(VehicleCallBack callback, UserData userData)
{
  uint8_t data = 1;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = MissionManager::missionCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetPause, &data, sizeof(data),
      500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::resume(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        data = 1;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetPause, &data, sizeof(data),
      timeout * 1000 / 2, 2);
}

ACK::WayPointInit
WaypointMission::getWaypointSettings(int timer)
{
  ACK::WayPointInit ack;
  uint8_t           arbNumber = 0;

  return *(ACK::WayPointInit *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointDownload, &arbNumber,
      sizeof(arbNumber), timer * 1000 / 4, 4);
}

void
WaypointMission::getWaypointSettings(VehicleCallBack callback,
                                     UserData        userData)
{
  uint8_t arbNumber = 0;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = getWaypointSettingsCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointDownload, &arbNumber,
      sizeof(arbNumber), 1000, 4, cb, udata);
}

void
WaypointMission::getWaypointSettingsCallback(Vehicle*      vehicle,
                                             RecvContainer recvFrame,
                                             UserData      userData)
{
  ACK::WayPointInit wpInitInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::WayPointInit))
  {
    wpInitInfo.data     = recvFrame.recvData.wpInitACK.data;
    wpInitInfo.ack.data = recvFrame.recvData.wpInitACK.ack;
    wpInitInfo.ack.info = recvFrame.recvInfo;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    return;
  }

  if (ACK::getError(wpInitInfo.ack))
    ACK::getErrorCodeMessage(wpInitInfo.ack, __func__);

  vehicle->missionManager->wpMission->info = wpInitInfo.data;

  DSTATUS("Index number: %d\n",
          vehicle->missionManager->wpMission->info.indexNumber);
}

ACK::WayPointIndex
WaypointMission::getIndex(uint8_t index, int timer)
{
  ACK::WayPointIndex ack;

  return *(ACK::WayPointIndex *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointIndexDownload, &index,
      sizeof(index), timer * 1000 / 4, 4);
}

void
WaypointMission::getIndex(uint8_t index, VehicleCallBack callback,
                          UserData userData)
{
  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = getIndexCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointIndexDownload, &index,
      sizeof(index), 1000, 4, cb, udata);
}

void
WaypointMission::getIndexCallback(Vehicle* vehicle, RecvContainer recvFrame,
                                  UserData userData)
{
  ACK::WayPointIndex wpIndexInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::WayPointIndex))
  {
    wpIndexInfo.data     = recvFrame.recvData.wpIndexACK.data;
    wpIndexInfo.ack.data = recvFrame.recvData.wpIndexACK.ack;
    wpIndexInfo.ack.info = recvFrame.recvInfo;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    return;
  }

  if (ACK::getError(wpIndexInfo.ack))
    ACK::getErrorCodeMessage(wpIndexInfo.ack, __func__);

  vehicle->missionManager->wpMission->index = &(wpIndexInfo.data);

  DSTATUS("Index number: %d\n",
          vehicle->missionManager->wpMission->index->index);
}

/*WayPointInitSettings
WaypointMission::getInfo() const
{
  return info;
}*/

void
WaypointMission::setInfo(const WayPointInitSettings& value)
{
  //! @todo set information for way point
  info = value;
  for (int i         = 0; i < 16; ++i)
    info.reserved[i] = 0;

  // TODO this might affect something, don't delete it yet for future debug use
  //#ifndef STATIC_MEMORY
  //  if (index != 0) delete index;
  //  index = 0;
  //#else
  //  if (maxIndex < info.indexNumber) index = 0;
  //#endif  // STATIC_MEMORY
}

void
WaypointMission::setIndex(WayPointSettings* value, size_t pos)
{
  if (index == 0)
  {
    index = new WayPointSettings[info.indexNumber];
    if (index == NULL)
    {
      DERROR("Lack of memory\n");
      return;
    }
  }
  index[pos] = *value;
  for (int i               = 0; i < 8; ++i)
    index[pos].reserved[i] = 0;
}

/*WayPointSettings*
WaypointMission::getIndex() const
{
  return index;
}

WayPointSettings*
WaypointMission::getIndex(size_t pos) const
{
  return &(index[pos]);
}*/

bool
WaypointMission::uploadIndexData(WayPointSettings* data,
                                 VehicleCallBack callback, UserData userData)
{
  setIndex(data, data->index);

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = WaypointMission::uploadIndexDataCallback;
    udata = NULL;
  }

  WayPointSettings send;
  if (data->index < info.indexNumber)
    send = index[data->index];
  else
    return false; //! @note range error

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointAddPoint, &send, sizeof(send),
      1000, 4, cb, udata);

  return true;
}

ACK::WayPointIndex
WaypointMission::uploadIndexData(WayPointSettings* data, int timeout)
{
  WayPointSettings   wpData;
  ACK::WayPointIndex ack;

  setIndex(data, data->index);

  if (data->index < info.indexNumber)
  {
    wpData = index[data->index];
  }
  else
  {
    // TODO add error handling
    DERROR("Range error\n");
  }

  return *(ACK::WayPointIndex *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointAddPoint, &wpData,
      sizeof(wpData), timeout * 1000 / 4, 4);
}

void
WaypointMission::readIdleVelocity(VehicleCallBack callback, UserData userData)
{
  uint8_t zero = 0;

  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = WaypointMission::idleVelocityCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointGetVelocity, &zero,
      sizeof(zero), 500, 2, cb, udata);
}

ACK::ErrorCode
WaypointMission::readIdleVelocity(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        zero = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointGetVelocity, &zero,
      sizeof(zero), timeout * 1000 / 2, 2);
}

void
WaypointMission::updateIdleVelocity(float32_t       meterPreSecond,
                                    VehicleCallBack callback, UserData userData)
{
  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = WaypointMission::idleVelocityCallback;
    udata = NULL;
  }

  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetVelocity, &meterPreSecond,
      sizeof(meterPreSecond), 500, 2, cb, udata);
}

ACK::WayPointVelocity
WaypointMission::updateIdleVelocity(float32_t meterPreSecond, int timeout)
{
  ACK::WayPointVelocity ack;

  return *(ACK::WayPointVelocity *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::waypointSetVelocity, &meterPreSecond,
      sizeof(meterPreSecond), timeout * 1000 / 2, 2);
}

void
WaypointMission::idleVelocityCallback(Vehicle* vehicle, RecvContainer recvFrame,
                                      UserData userData)
{
  ACK::WayPointVelocityInternal wpIdleVelocityInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::WayPointVelocityInternal))
  {
    wpIdleVelocityInfo = recvFrame.recvData.wpVelocityACK;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    return;
  }

  ACK::ErrorCode ack;
  ack.data = wpIdleVelocityInfo.ack;
  ack.info = recvFrame.recvInfo;

  if (ACK::getError(ack))
    ACK::getErrorCodeMessage(ack, __func__);

  vehicle->missionManager->wpMission->info.idleVelocity =
    wpIdleVelocityInfo.idleVelocity;

  DSTATUS("Current idle velocity: %f\n",
          vehicle->missionManager->wpMission->info.idleVelocity);
}

void
WaypointMission::uploadIndexDataCallback(Vehicle*      vehicle,
                                         RecvContainer recvFrame,
                                         UserData      userData)
{
  ACK::WayPointAddPointInternal wpDataInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::WayPointAddPointInternal))
  {
    wpDataInfo = recvFrame.recvData.wpAddPointACK;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    return;
  }

  ACK::ErrorCode ack;
  ack.data = wpDataInfo.ack;
  ack.info = recvFrame.recvInfo;

  if (ACK::getError(ack))
    ACK::getErrorCodeMessage(ack, __func__);

  DSTATUS("Index number: %d\n", wpDataInfo.index);
}

void
WaypointMission::setWaypointEventCallback(VehicleCallBack callback,
                                          UserData        userData)
{
  wayPointEventCallback.callback = callback;
  wayPointEventCallback.userData = userData;
}

void
WaypointMission::setWaypointCallback(VehicleCallBack callback,
                                     UserData        userData)
{
  wayPointCallback.callback = callback;
  wayPointCallback.userData = userData;
}
