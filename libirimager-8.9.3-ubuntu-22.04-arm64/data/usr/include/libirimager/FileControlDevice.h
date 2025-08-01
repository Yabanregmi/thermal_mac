#ifndef FILECONTROLDEVICE_H
#define FILECONTROLDEVICE_H

#include "BaseControlDevice.h"

namespace evo
{

/**
 * @class FileControlDevice
 * @brief File control interface to PI imagers, necessary to replay recorded files
 * @author Stefan May (Evocortex GmbH)
 */
class FileControlDevice : public BaseControlDevice
{
public:

  FileControlDevice(unsigned short hwRev, unsigned short fwRev, int serial);

  ~FileControlDevice();

  int Init(unsigned long vid, unsigned long pid);

  int Init(unsigned long vid, unsigned long pid, unsigned long serno);

  unsigned long InitPif(void);

  int GetBuffer(unsigned char *buffer, int len);

  unsigned long GetSerialNumber(void) const;

  unsigned short GetFirmwareRev(void) const;

  unsigned short GetHardwareRev(void) const;

  void GetPifIn(unsigned short* pVoltage, unsigned char PifInChn);

  void SetPifOut(unsigned short Voltage, unsigned char PifOutChn);

  void FailSafe(bool Val);

  void SetTecEnable(bool on);

  void GetTecEnable(bool* pVal);

  void SetTempTec(float Temp);

  void GetTempTec(float* pVal);

  void GetFlag(TFlagState* pVal);

  void SetFlag(TFlagState Val);

  void SetFlagCycle(unsigned short CycleCount);

  void SetTecA(unsigned short Val);

  void GetTecA(unsigned short* pVal);

  void SetTecB(unsigned short Val);

  void GetTecB(unsigned short* pVal);

  void SetTecC(unsigned short Val);

  void GetTecC(unsigned short* pVal);

  void SetTecD(unsigned short Val);

  void GetTecD(unsigned short* pVal);

  void SetSkim(unsigned short voltage);

  void SetSkim_WaitForFlag(unsigned short voltage);

  void GetSkim(unsigned short* pVal);

  void SetSkim_Adjust(unsigned short voltage);

  void GetSkim_Adjust(unsigned short* pVal);

  void SetFid(unsigned short voltage);

  void SetFid_WaitForFlag(unsigned short voltage);

  void GetFid(unsigned short* pVal);

  void SetFid_Adjust(unsigned short voltage);

  void GetFid_Adjust(unsigned short* pVal);

  void SetBiasEnable(bool on);

  void GetBiasEnable(bool* pVal);

  void SetLaserEnable(bool on);

  void GetLaserEnable(bool* on);

  void SetPowerEnable(bool on);

  void GetPowerEnable(bool* on);

  void GetAntiFlicker(bool* on);

  void SetAntiFlicker(bool on);

  void SetClippedFormatPosition(unsigned short x, unsigned short y);

  void GetClippedFormatPosition(unsigned short* pValx, unsigned short* pValy);

  bool GetShortImagerCaps(void);

  TPifMode GetPIFInMode(short PifInChn);

  void SetPIFInMode(TPifMode Val, short PifInChn);

  TPifMode GetPIFInDigitalMode(void);

  void SetPIFInDigitalMode(TPifMode Val);

  TPifMode GetPIFOutMode(short PifInChn);

  void SetPIFOutMode(TPifMode Val, short PifInChn);

  unsigned short GetPIFInFlagThreshold(void);

  void SetPIFInFlagThreshold(unsigned short val);

  bool GetPIFInFlagOpenIfLower(void);

  void SetPIFInFlagOpenIfLower(bool val);

  void SetPIFOutFlagOpen(unsigned short value, short PifOutChn);

  void GetPIFOutFlagOpen(unsigned short* pVal, short PifOutChn);

  void SetPIFOutFlagClosed(unsigned short value, short PifOutChn);

  void GetPIFOutFlagClosed(unsigned short* pVal, short PifOutChn);

  void SetPIFOutFlagMoving(unsigned short value, short PifOutChn);

  void GetPIFOutFlagMoving(unsigned short* pVal, short PifOutChn);

  void SetPIFOutFrameSync(unsigned short value, short PifOutChn);

  void GetPIFOutFrameSync(unsigned short* pVal, short PifOutChn);

  void SetPIFOutSamplePointTable(PSamplePoint Table, int TableSize, double SlopeFactor, double SlopeOffset, bool TempHighResolution, short PifOutChn);

  bool GetPIFInDigitalFlagLowActive(void);

  void SetPIFInDigitalFlagLowActive(bool val);

  unsigned short GetPIFInThreshold(void);

  void SetPIFInThreshold(unsigned short val);

  bool GetPIFInOpenIfLower(void);

  void SetPIFInOpenIfLower(bool val);

  bool GetPIFInDigitalLowActive(void);

  void SetPIFInDigitalLowActive(bool val);

  //TPifVersion GetPifVersion(void);

  bool GetChecksumValid(void);

  void GetTempChip(float* pVal);

  void GetTempBox(float* pVal);

  void GetTempFlag(float* pVal);

private:

  bool SendCommand(unsigned int id, unsigned short cmd_lsb, unsigned short cmd_msb);

  void CheckFlagSource(void);

  int FirmwareRead(unsigned char* Daten, unsigned short count);

  int FirmwareWrite(unsigned char* Daten, unsigned short count);

  void ControlJTAGPort(unsigned char* ptr_data1, unsigned char* ptr_data2, unsigned char rw, unsigned short bit_cnt, unsigned char sel);

  bool MCSFirmwareCheck(unsigned char* fw_from_file, int fw_size);

  bool MCSFirmwareWrite(unsigned char* fw_data, int fw_size);

  unsigned short _skim;
  unsigned short _fid;

  unsigned short _tecA;
  unsigned short _tecB;
  unsigned short _tecC;
  unsigned short _tecD;

  unsigned short _hwRev;
  unsigned short _fwRev;
  int _serial;
};

} //namespace

#endif
