#ifndef FLIR_GIGE_H_
#define FLIR_GIGE_H_

#include <memory>

#include <PvSystem.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvBuffer.h>
#include <PvPipeline.h>

#include <sensor_msgs/Image.h>

#include "flir_gige/planck.h"

namespace flir_gige {

struct FreeDevice {
  void operator()(PvDevice *device) const { PvDevice::Free(device); }
};

struct FreeStream {
  void operator()(PvStream *stream) const { PvStream::Free(stream); }
};

class FlirGige {
 public:
  FlirGige(const std::string &ip_address);

  const std::string &ip_address() const { return ip_address_; }
  const std::string &display_id() const { return display_di_; }

  void Connect();

  void Start();

  void Stop();

  void Disconnect();

  void Configure(FlirGigeDynConfig &config);

 private:
  using PvDevicePtr = std::unique_ptr<PvDevice, FreeDevice>;
  using PvStreamPtr = std::unique_ptr<PvStream, FreeStream>;
  using PvPipelinePtr = std::unique_ptr<PvPipeline>;

  void FindDevice(const std::string &ip);
  std::string AvailableDevice() const;
  void ConnectDevice();
  void OpenStream();
  void ConfigureStream();
  void CreatePipeline();
  void StartAcquisition();
  void StopAcquisition();
  void AcquireImages();
  void LabeledOutput(const std::string &msg) const;

  double GetSpotPixel(const cv::Mat &image) const;
  void SetPixelFormat(int bit);

  std::string ip_address_;
  std::string display_id_;
  PvSystem system_;
  const PvDeviceInfo *dinfo_;
  PvDevicePtr device_;
  PvStreamPtr stream_;
  PvPipelinePtr pipeline_;
};

}  // namespace flir_gige

#endif  // FLIR_GIGE_H_