#include <stdio.h>
#include <string.h>
#include <signal.h>

// Optris device interfaces
#include "IRDevice.h"

// Optris imager interfaces
#include "IRImager.h"

// Optris logging interface
#include "IRLogger.h"

// Optris tiff image file writer
#include "extras/IRTiffWriter.h"

// evo Frame Rate Counter
#include "FramerateCounter.h"

using namespace std;
using namespace evo;

bool _keepCapturing = true;
FramerateCounter _fpsCounter(1000./*ms*/, 100);
IRTiffWriter _irTiffWriter;
IRImager _imager;

void sigHandler(int dummy=0)
{
    _keepCapturing = false;
}

void onThermalFrame(unsigned short* thermal, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg)
{
  //write tiff files with maximum 1 per second
  if(_fpsCounter.trigger() && meta.flagState == EnumFlagState::irFlagOpen) 
  {
    //create filepath for tiff file
    std::string fileName = std::to_string(meta.counter) + ".tiff";

    //write the tiff file
    if(!_irTiffWriter.writeTiff(fileName.c_str(), &_imager, thermal, w, h))
    {
      cout << "Written tiff file: " << fileName << endl;
    }
    else
    {
      cout << "Failed to write tiff file: " << fileName << endl;
    }
  }
}

int main (int argc, char* argv[])
{
  if(argc!=2)
  {
    cout << "usage: " << argv[0] << " <xml configuration file>" << endl;
    return -1;
  }

  signal(SIGINT, sigHandler);
  IRLogger::setVerbosity(IRLOG_ERROR, IRLOG_OFF);
  IRDeviceParams params;
  IRDeviceParamsReader::readXML(argv[1], params);
  IRDevice* dev = IRDevice::IRCreateDevice(params);

  if(dev)
  {
    /**
     * Initialize Optris image processing chain
     */
    if(_imager.init(&params, dev->getFrequency(), dev->getWidth(), dev->getHeight(), dev->controlledViaHID()))
    {
      if(_imager.getWidth()!=0 && _imager.getHeight()!=0)
      {
        cout << "Thermal channel: " << _imager.getWidth() << "x" << _imager.getHeight() << "@" << _imager.getMaxFramerate() << "Hz" << endl;

        if(!dev)
        {
          cout << "NO DEV" << endl;
          exit(1);
        }

        // Start UVC streaming
        if(dev->isOpen())
        {
          // Enter loop in order to pass raw data to Optris image processing library.
          // Processed data are supported by the frame callback function.
          unsigned char* bufferRaw = new unsigned char[dev->getRawBufferSize()];
          _imager.setThermalFrameCallback(onThermalFrame);
          dev->startStreaming();

          while(_keepCapturing)
          {
            evo::IRDeviceError ret = dev->getFrame(bufferRaw);
            if(ret == evo::IRIMAGER_SUCCESS)
            {
              _imager.process(bufferRaw, NULL);
            }
            else
            {
              std::cout << "Error on getFrame: " << ret << std::endl;
            }
          }
        }
        else
        {
          cout << "Error occurred in starting stream ... aborting. You may need to reconnect the camera." << endl;
        }
      }
    }
    else
    {
      cout << "Error: Image streams not available or wrongly configured. Check connection of camera and config file." << endl;
    }
    delete dev;
  }
  return 0;
}
