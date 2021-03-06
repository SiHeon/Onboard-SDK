/** @file dji_hotpoint.cpp
 *  @version 3.3
 *  @date April 2017
 *
 *  @brief Implementation of HotPoint (Point of Interest) Missions for DJI OSDK
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

#include "dji_hotpoint.hpp"
#include "dji_mission_manager.hpp"
#include "dji_vehicle.hpp"

using namespace DJI;
using namespace DJI::OSDK;

HotpointMission::HotpointMission(Vehicle* vehicle)
  : MissionBase(vehicle)
{
  initData();
  hotPointCallback.callback = 0;
  hotPointCallback.userData = 0;
}

HotpointMission::~HotpointMission()
{
}

void
HotpointMission::initData()
{
  hotPointData.version = 0;

  /*! @todo find a method to replace these
  hotPointData.height    = api->getBroadcastData().pos.altitude;
  hotPointData.longitude = api->getBroadcastData().pos.longitude;
  hotPointData.latitude  = api->getBroadcastData().pos.latitude;
  */

  hotPointData.radius     = 10;
  hotPointData.yawRate    = 15;
  hotPointData.clockwise  = 1;
  hotPointData.startPoint = HotpointMission::VIEW_NEARBY;
  hotPointData.yawMode    = HotpointMission::YAW_INSIDE;
}

HotPointSettings
HotpointMission::getData() const
{
  return hotPointData;
}

void
HotpointMission::start(VehicleCallBack callback, UserData userData)
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
    cb = MissionManager::missionCallback;
    udata = NULL;
  }
  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::hotpointStart, &hotPointData,
      sizeof(hotPointData), 500, 2, cb, udata);
}

ACK::ErrorCode
HotpointMission::start(int timeout)
{
  ACK::ErrorCode ack;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointStart, &hotPointData,
      sizeof(hotPointData), timeout * 1000 / 2, 2);
}

void
HotpointMission::stop(VehicleCallBack callback, UserData userData)
{
  uint8_t zero    = 0;
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
      OpenProtocolCMD::CMDSet::Mission::hotpointStop, &zero, sizeof(zero), 500,
      2, cb, udata);
}

ACK::ErrorCode
HotpointMission::stop(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        zero = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointStop, &zero, sizeof(zero),
      timeout * 1000 / 2, 2);
}

void
HotpointMission::pause(VehicleCallBack callback, UserData userData)
{
  uint8_t data    = 0;
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
      OpenProtocolCMD::CMDSet::Mission::hotpointSetPause, &data, sizeof(data),
      500, 2, cb, udata);
}

ACK::ErrorCode
HotpointMission::pause(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        data = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointSetPause, &data, sizeof(data),
      timeout * 1000 / 2, 2);
}

void
HotpointMission::resume(VehicleCallBack callback, UserData userData)
{
  uint8_t data    = 1;
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
      OpenProtocolCMD::CMDSet::Mission::hotpointSetPause, &data, sizeof(data),
      500, 2, cb, udata);
}

ACK::ErrorCode
HotpointMission::resume(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        data = 1;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointSetPause, &data, sizeof(data),
      timeout * 1000 / 2, 2);
}

void
HotpointMission::updateYawRate(HotpointMission::YawRate& Data)
{
  hotPointData.yawRate   = Data.yawRate;
  hotPointData.clockwise = Data.clockwise ? 1 : 0;

  vehicle->legacyLinker->send(
    OpenProtocolCMD::CMDSet::Mission::hotpointYawRate, &Data,
    sizeof(Data));
}

void
HotpointMission::updateYawRate(float32_t yawRate, bool isClockwise)
{
  YawRate p;
  p.yawRate   = yawRate;
  p.clockwise = isClockwise ? 1 : 0;
  updateYawRate(p);
}

void
HotpointMission::updateRadius(float32_t meter)
{
  vehicle->legacyLinker->send(
      OpenProtocolCMD::CMDSet::Mission::hotpointRadius, &meter, sizeof(meter));
}

void
HotpointMission::resetYaw(VehicleCallBack callback, UserData userData)
{
  uint8_t zero    = 0;
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
      OpenProtocolCMD::CMDSet::Mission::hotpointSetYaw, &zero, sizeof(zero),
      500, 2, cb, udata);
}

ACK::ErrorCode
HotpointMission::resetYaw(int timeout)
{
  ACK::ErrorCode ack;
  uint8_t        zero = 0;

  return *(ACK::ErrorCode *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointSetYaw, &zero, sizeof(zero),
      timeout * 1000 / 2, 2);
}

void
HotpointMission::getHotpointSettings(VehicleCallBack callback,
                                     UserData        userData)
{
  uint8_t zero    = 0;
  VehicleCallBack cb = NULL;
  UserData udata = NULL;
  if (callback)
  {
    cb = callback;
    udata = userData;
  }
  else
  {
    cb = getHotpointSettingsCallback;
    udata = NULL;
  }
  vehicle->legacyLinker->sendAsync(
      OpenProtocolCMD::CMDSet::Mission::hotpointDownload, &zero, sizeof(zero),
      500, 2, cb, udata);
}

void
HotpointMission::getHotpointSettingsCallback(Vehicle*      vehiclePtr,
                                             RecvContainer recvFrame,
                                             UserData      userData)
{
  HotpointMission*          hp = (HotpointMission*)userData;
  ACK::HotPointReadInternal hpReadInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::HotPointReadInternal))
  {
    hpReadInfo = recvFrame.recvData.hpReadACK;
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    return;
  }

  ACK::ErrorCode ack;
  ack.info = recvFrame.recvInfo;
  ack.data = hpReadInfo.ack;

  if (ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, __func__);

    DERROR("Decod ACK error 0x%X\n", hpReadInfo.ack);
  }
}

ACK::HotPointRead
HotpointMission::getHotpointSettings(int timeout)
{
  ACK::HotPointRead ack;
  uint8_t           zero = 0;

  return *(ACK::HotPointRead *) vehicle->legacyLinker->sendSync(
      OpenProtocolCMD::CMDSet::Mission::hotpointDownload, &zero, sizeof(zero),
      timeout * 1000 / 2, 2);
}

void
HotpointMission::startCallback(RecvContainer recvFrame, UserData userData)
{
  HotpointMission*           hp = (HotpointMission*)userData;
  ACK::HotPointStartInternal hpStartInfo;

  if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <=
      sizeof(ACK::HotPointStartInternal))
  {
    hpStartInfo = recvFrame.recvData.hpStartACK;

    DSTATUS("Start ack has max radius: %f, ACK 0x%X\n", hpStartInfo.maxRadius,
            hpStartInfo.ack);
  }
  else
  {
    DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
  }
}

void
HotpointMission::setHotpointCallback(VehicleCallBack callback,
                                     UserData        userData)
{
  hotPointCallback.callback = callback;
  hotPointCallback.userData = userData;
}

void
HotpointMission::setData(HotPointSettings* data)
{
  hotPointData         = *data;
  hotPointData.version = 0;
}

void
HotpointMission::setHotPoint(float64_t longitude, float64_t latitude,
                             float64_t altitude)
{
  hotPointData.longitude = longitude;
  hotPointData.latitude  = latitude;
  hotPointData.height    = altitude;
}

void
HotpointMission::setHotPoint(Telemetry::GlobalPosition gps)
{
  hotPointData.longitude = gps.longitude;
  hotPointData.latitude  = gps.latitude;
  hotPointData.height    = gps.altitude;
}

void
HotpointMission::setRadius(float64_t meter)
{
  hotPointData.radius = meter;
}

void
HotpointMission::setYawRate(float32_t degree)
{
  hotPointData.yawRate = degree;
}

void
HotpointMission::setClockwise(bool isClockwise)
{
  hotPointData.clockwise = isClockwise ? 1 : 0;
}

void
HotpointMission::setCameraView(HotpointMission::View view)
{
  hotPointData.startPoint = view;
}

void
HotpointMission::setYawMode(HotpointMission::YawMode mode)
{
  hotPointData.yawMode = mode;
}
