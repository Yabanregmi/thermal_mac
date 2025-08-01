#ifndef BASECONTROLDEVICE_H
#define BASECONTROLDEVICE_H

#include <cstddef>
#include "ImagerDefs.h"

enum TFlagState 
{
    fsFlagOpen, 
    fsFlagClose, 
    fsFlagOpening, 
    fsFlagClosing, 
    fsError
};

enum TAutoSkimOperation // new with T#1414
{
	asoDirectSkimRunning,
	asoDirectSkimReady,
	asoFreezingFlagStart,
	asoFreezingFlagStop
};

enum TKeepFreezing // new with T#1414
{
	keepfreezingOff,
	keepfreezingPending,
	keepfreezingActiv
};

enum TPifMode // order should not be changed as we use this values to send it to the devices	
{ 
    pifmOff, 
    pifmFlag, 
    pifmRecording, 
    pifmSnapshot, 
    pifmLinescanner, 
    pifmFrameSync, 
    pifmCenterPixel, 
    pifmResetPeakValleyHold, 
    pifmTriggerEventGrabber, 
    pifmArea, 
    pifmAlarm, 
    pifmTInt, 
    pifmFailsafe, 
    pifmEmissivity, // T#914
    pifmTAmbient, // T#914
    pifmTReference, // T#914
    pifmSwitchTempRange, // T#1064
    pifmSnapshotOnEdge,
	pifmAutonomous, // T#1768
    pifmUndefined=0x0fff, 
    pifmUser=0xffff 
};

enum TPifAOOutputMode
{ 
    pifaom_0V_10V, 
    pifaom_0mA_20mA, 
    pifaom_4mA_20mA,   // Changed with T#776
    pifaomNotDefined=0xffff 
};

enum TPifPostProcessingMode  // Part of T#485
{ 
    pifppmOff, 
    pifppmAvg, 
    pifppmPeakHold, 
    pifppmValleyHold, 
    pifppmAdvPeakHold, 
    pifppmAdvValleyHold 
};

enum TPifType // Changed with T#775, order should not be changed as we use this values to send it to the devices
{ 
    ptNone, 
    ptPIF, 
    ptPIPIFmV, 
    ptIntern, 
    ptStackable, 
    ptDigital, 
    ptPIPIFmA, 
    ptUndefined=0x0fff, 
    ptError=0xffff 
};

enum TPifPortType 
{ 
    pptAI, 
    pptDI, 
    pptAO, 
    pptDO, 
    pptFS,
	pptDigital
};

enum TImagerType  //part of T#219
{
    ItUndefined, 
    ItPI1,
    ItPI160, 
    ItPI200230, 
    ItPI160orPI200230, 
    ItPI400450, 
    ItPI400I450I,
    ItPI640,
    ItPI640I,
    ItPI1M, 
    ItXI80, 
    ItXI160, 
    ItXI400, 
	ItXI410,
    ItXI640, 
    ItXI1M 
};

enum TInteraceType { interfaceUSB, interfaceEthernet }; // part of T#769

enum TDenoiseMode { denoiseOff, denoiseSimple3x3, denoiseSimple5x5, denoiseGauss3x3, denoiseGauss5x5 }; // T#2186

typedef struct tagDeviceID // new with T#769
{
    unsigned long SerialNumber;
    unsigned long PID;
    unsigned long VID;
    unsigned long IPAddress;
    TInteraceType InterfaceType;
    bool operator==(tagDeviceID id)
    {
        return (SerialNumber == id.SerialNumber) && (PID == id.PID) && (VID == id.VID) && (IPAddress == id.IPAddress) && (InterfaceType == id.InterfaceType);
    }
} DeviceID, *PDeviceID;

typedef struct tagSamplePoint
{
    short  ADU;
    short  Val; // temperature or voltage
	bool operator==(tagSamplePoint val) {	return (ADU == val.ADU) && (Val == val.Val); } //   '=='
	bool operator!=(tagSamplePoint val) {	return (ADU != val.ADU) || (Val != val.Val); } //   '!='
}
SamplePoint, *PSamplePoint;

typedef struct tagSamplePointTable
{
	PSamplePoint SamplePoints;
    int Size;
}
SamplePointTable, *PSamplePointTable;

typedef struct tagAutonomousEntry // Part of T#621
{
    unsigned char Source;
    double FixedValue;
    float SlopeFactor;
    float SlopeOffset;
    float SlopeLow;
    float SlopeHigh;
}
AutonomousEntry, *PAutonomousEntry;

typedef struct tagAutonomousSettings // Part of T#621
{
    AutonomousEntry Emissivity;    // Source: 0 = Fixed value, 1 = PIF (PIFAI)
    AutonomousEntry Transmissivity;// Source: allways 0 = Fixed value
    AutonomousEntry TAmbient;      // Source: 0 = Fixed value, 1 = PIF (PIFAI), 2 = Internal (TBox - 5K)
    AutonomousEntry TReference;    // Source: 0 = Fixed value, 1 = PIF (PIFAI), 2 = Off
    unsigned char TReferenceMeasureAreaIndex;
    unsigned char TReferenceFittingMode; // 0 = Auto, 1 = Offset, 2 = Gain
    bool TReferenceConsiderFlag;
    unsigned char TempTEC_Mode;       // 0 = Standard, 1 = Auto, 2 = Fixed
    double TempTEC_FixedValue;
    bool FlagAutomatic_Enable;
    int FlagAutomatic_MinInterval; // changed with T#913
    int FlagAutomatic_MaxInterval;
    unsigned char OpticsIndex;
    unsigned char TemprangeIndex;
    unsigned char VideoFormatIndex;
}
AutonomousSettings, *PAutonomousSettings;

typedef struct tagAutonomousAreaConfig // Part of T#406
{
    unsigned short MeasureAreaIndex;
    unsigned char AutonomousAreaIndex;
    char mode;
    short LoAlarm;
    short HiAlarm;
    short DistributionMin;
    short DistributionMax;
    short AltEmissivity;
    unsigned short AlarmPIFAOChannels;
    unsigned short AlarmPIFDOChannels;
    unsigned int matrixLength;
    bool* matrix;
    // for calculated areas:
    short Operation;
    short OperandsCount;
    short ParamsCount;
    short OptionsCount;
    short *Operands;
    float *Params;
    bool *Options;
}
AutonomousAreaConfig, *PAutonomousAreaConfig;

typedef struct tagAutonomousPifAOConfig // Part of T#406
{
    unsigned char Channel;
    unsigned char PifIndex;
    unsigned char PinIndex;
    short AutonomousAreaIndex;
    TPifMode Mode;
    TPifAOOutputMode OutputMode;
    float SlopeFactor, SlopeOffset;
    unsigned short AlarmVoltage; // Part of T#620
    unsigned short NoAlarmVoltage;
    unsigned short FailsafeVoltage; // Part of T#741
    bool FailsafeStopOnAlarm;
    bool FailsafeStopOnFlagTimeout;
    unsigned short FailsafeFlagCycles;
}
AutonomousPifAOConfig, *PAutonomousPifAOConfig;

typedef struct tagAutonomousPifDOConfig // Part of T#667
{
    unsigned char Channel;
    unsigned char PifIndex;
    unsigned char PinIndex;
    TPifMode Mode;
    bool LowActive;
    bool FailsafeStopOnAlarm;
    bool FailsafeStopOnFlagTimeout;
    unsigned short FailsafeFlagCycles;
}
AutonomousPifDOConfig, *PAutonomousPifDOConfig;

typedef struct tagAutonomousPifFSConfig // Part of T#1818
{
    bool Active;
    bool FailsafeStopOnAlarm;
    bool FailsafeStopOnFlagTimeout;
    unsigned short FailsafeFlagCycles;
}
AutonomousPifFSConfig, *PAutonomousPifFSConfig;

typedef struct tagVideoFormat
{
	long Width;
	long Height;
	short BitCount;
	unsigned long BitRate;
	unsigned long FrameInterval;
} 
VideoFormat, *PVideoFormat;

#define PIFINMAXCNT 16
#define PIFINCNT 2
#define PIFINDIGCNT 1
#define PIFOUTCNT 3
#define PIFOUTDIGCNT 0
#define PIFDEAFAULTFW 100 // T#1940


class PifDeviceInfo
{
	unsigned short *PifFW; // T#1940
	unsigned int *Serialnumber; // T#1940
	unsigned short *AI_Ports;
    unsigned short *DI_Ports;
    unsigned short *AO_Ports;
    unsigned short *DO_Ports;
    unsigned short *FS_Ports;

public:
    TPifType Version;
    unsigned char DeviceCountActual;
    unsigned char DeviceCountDesired;
    unsigned char AICount;
    unsigned char DICount;
    unsigned char AOCount;
    unsigned char DOCount;
    unsigned char FSCount;
    unsigned char MaxAutonomousMeasAreaCount;
    unsigned char MaxAutonomousCalcAreaCount; // Part of T#1320

    PifDeviceInfo(TPifType Version, unsigned char DeviceCountActual, unsigned char DeviceCountDesired, unsigned char AICount, unsigned char DICount, unsigned char AOCount, unsigned char DOCount, unsigned char FSCount)
    {
        this->Version = Version;
        this->DeviceCountActual = DeviceCountActual;
        this->DeviceCountDesired = DeviceCountDesired;
        this->AI_Ports = this->DI_Ports = this->AO_Ports = this->DO_Ports = this->FS_Ports = NULL;
        this->AICount = this->DICount = this->AOCount = this->DOCount = this->FSCount = 0;
		PifFW = new unsigned short[DeviceCountActual];
		Serialnumber = new unsigned int[DeviceCountActual];
		memset(PifFW, 0, DeviceCountActual*sizeof(unsigned short));
		memset(Serialnumber, 0, DeviceCountActual*sizeof(unsigned int));
        AddPort(&AI_Ports, &this->AICount, AICount, 0);
        AddPort(&DI_Ports, &this->DICount, DICount, 0);
        AddPort(&AO_Ports, &this->AOCount, AOCount, 0);
        AddPort(&DO_Ports, &this->DOCount, DOCount, 0);
        AddPort(&FS_Ports, &this->FSCount, FSCount, 0);
        this->MaxAutonomousMeasAreaCount = 1;
        this->MaxAutonomousCalcAreaCount = 0;
    }
    PifDeviceInfo(TPifType Version, unsigned char DeviceCount)
    {
        this->Version = Version;
        this->DeviceCountActual = DeviceCount;
        this->DeviceCountDesired = DeviceCount;
        AI_Ports = DI_Ports = AO_Ports = DO_Ports = FS_Ports = NULL;
        AICount = DICount = AOCount = DOCount = FSCount = MaxAutonomousMeasAreaCount = MaxAutonomousCalcAreaCount = 0;
		PifFW = new unsigned short[DeviceCount];
		Serialnumber = new unsigned int[DeviceCount];
		memset(PifFW, 0, DeviceCountActual*sizeof(unsigned short));
		memset(Serialnumber, 0, DeviceCountActual*sizeof(unsigned int));
	}
    ~PifDeviceInfo(void)
    {
		delete[] PifFW;
		delete[] Serialnumber;
        delete[] AI_Ports;
        delete[] DI_Ports;
        delete[] AO_Ports;
        delete[] DO_Ports;
        delete[] FS_Ports;
    }
    void AddPif(unsigned short PifFW, unsigned int Serialnumber, unsigned char AICount, unsigned char DICount, unsigned char AOCount, unsigned char DOCount, unsigned char FSCount, unsigned char PifIndex)
    {
		this->PifFW[PifIndex] = PifFW; // T#1940
		this->Serialnumber[PifIndex] = Serialnumber; // T#1940
        AddPort(&AI_Ports, &this->AICount, AICount, PifIndex);
        AddPort(&DI_Ports, &this->DICount, DICount, PifIndex);
        AddPort(&AO_Ports, &this->AOCount, AOCount, PifIndex);
        AddPort(&DO_Ports, &this->DOCount, DOCount, PifIndex);
        if (!this->FSCount) // Part of T#568
            AddPort(&FS_Ports, &this->FSCount, FSCount ? 1 : 0, PifIndex);
    }
    bool GetAI(unsigned char PifAIChannel, unsigned char *PifIndex, unsigned char *PinIndex) 
    {
        return GetPortInfo(AI_Ports, PifAIChannel, AICount, PifIndex, PinIndex);
    }
    bool GetDI(unsigned char PifDIChannel, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        return GetPortInfo(DI_Ports, PifDIChannel, DICount, PifIndex, PinIndex);
    }
    bool GetAO(unsigned char PifAOChannel, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        return GetPortInfo(AO_Ports, PifAOChannel, AOCount, PifIndex, PinIndex);
    }
    bool GetDO(unsigned char PifDOChannel, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        return GetPortInfo(DO_Ports, PifDOChannel, DOCount, PifIndex, PinIndex);
    }
    bool GetFS(unsigned char PifFSChannel, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        return GetPortInfo(FS_Ports, PifFSChannel, FSCount, PifIndex, PinIndex);
    }

    bool GetPort(TPifPortType PifPortType, unsigned char PifChannel, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        switch (PifPortType)
        {
        case pptAI: return GetPortInfo(AI_Ports, PifChannel, AICount, PifIndex, PinIndex);
        case pptDI: return GetPortInfo(DI_Ports, PifChannel, DICount, PifIndex, PinIndex);
        case pptAO: return GetPortInfo(AO_Ports, PifChannel, AOCount, PifIndex, PinIndex);
        case pptDO: return GetPortInfo(DO_Ports, PifChannel, DOCount, PifIndex, PinIndex);
        case pptFS: return GetPortInfo(FS_Ports, PifChannel, FSCount, PifIndex, PinIndex);
        default:    return false;
        }
        
    }

	unsigned short GetPifFW(unsigned char PifIndex) // T#1940
	{
		return (PifIndex < DeviceCountActual) ? PifFW[PifIndex] : 0;
	}

	unsigned int GetSerialnumber(unsigned char PifIndex) // T#1940
	{
		return (PifIndex < DeviceCountActual) ? Serialnumber[PifIndex] : 0;
	}

	void SetSerialnumber(unsigned char PifIndex, unsigned int serialnumber) // T#1940
	{
		if (PifIndex < DeviceCountActual) 
			Serialnumber[PifIndex] = serialnumber;
	}

private:
    void AddPort(unsigned short **Ports, unsigned char *ExistingCount, unsigned char NewCount, unsigned char PifIndex)
    {
        unsigned short i;
        unsigned short *NewPorts = new unsigned short[*ExistingCount + NewCount];
        for (i = 0; i < *ExistingCount; i++) // copying existings entries into new array
            NewPorts[i] = (*Ports)[i];
        unsigned short PIFIndex = PifIndex << 8; // move PifIndex into hi byte (of a short)
        for (i = 0; i < NewCount; i++, (*ExistingCount)++) // creating new entries
            NewPorts[*ExistingCount] = PIFIndex + i;
        if (*Ports) // delete old array
            delete[] * Ports;
        *Ports = NewPorts;
    }
    bool GetPortInfo(unsigned short *Ports, unsigned char PifChannel, unsigned char PifChannelCount, unsigned char *PifIndex, unsigned char *PinIndex)
    {
        if (PifChannel < PifChannelCount)
        {
            *PifIndex = (Ports[PifChannel] >> 8) & 0x00ff;
            *PinIndex = Ports[PifChannel] & 0x00ff;
            return true;
        }
        return false;

    }
};

typedef struct tagCaliDownloadConfig // T#1871
{
	unsigned long StartAddress;
	unsigned long EndAddress;
	unsigned long ReadyIdentifier;
	unsigned long DeadPixelTableLength;
	unsigned long TemperatureTableLength;
	unsigned char TemperatureRanges;
}
CaliDownloadConfig, *PCaliDownloadConfig;

/**
 * @class BaseControlDevice
 * @brief Generic control interface to PI imagers
 */
class BaseControlDevice
{
public:

  BaseControlDevice();

  virtual ~BaseControlDevice();

  virtual int Init(PDeviceID pDevID, bool ForceInitHID, bool *SameDevice) = 0; // Changed with T#769

  virtual unsigned long InitPif(bool AllowProprietaryPIF, bool UseExternalProbeForReferencing) = 0; // Changed with T#821, T#1388

  virtual void InitPifExplicitly(PifDeviceInfo *p) = 0; // Part of T#673

  virtual int SetPifType(TPifType PifType, unsigned char PifDeviceCount) = 0;  // New with T#593, T#605

  virtual int SetDigitalPifSIParams(unsigned short BusAddress, unsigned int Baudrate) = 0;  // New with T#1505
  virtual int GetDigitalPifSIParams(unsigned short *BusAddress, unsigned int *Baudrate) = 0;  // New with T#1505

  virtual int GetBuffer(unsigned char *buffer, int len) = 0;

  virtual unsigned long GetSerialNumber(void) = 0;

  virtual unsigned short GetFirmwareRev(void) = 0;

  virtual unsigned short GetHardwareRev(void) = 0;

  virtual bool ConfigIsReadable(void) = 0; // Part of T#1168

  virtual void GetPifAICount(unsigned short *pCount) = 0; // Part of T#416

  virtual void GetPifDICount(unsigned short *pCount) = 0; // Part of T#416

  virtual void GetPifAOCount(unsigned short *pCount) = 0; // Part of T#415

  virtual void GetPifDOCount(unsigned short *pCount) = 0; // Part of T#541

  virtual void GetPifFSCount(unsigned short *pCount) = 0; // Part of T#415

  virtual void GetPifMaxAutonomousAreaCount(unsigned short *pMeasAreaCount, unsigned short *pCalcAreaCount) = 0; // Part of T#638, T#1320

  virtual void GetPifAI(unsigned short *pVoltage, unsigned char PifChn) = 0;// Part of NF1080

  virtual void SetPifAO(unsigned short Voltage, unsigned char PifChn) = 0; // Part of NF1080

  virtual void SetPifDO(bool Value, unsigned char PifChn) = 0; // Part of T#541
  
  virtual void BlockPifOut(bool block) = 0; // Part of T#316

  virtual void FailSafe(bool Val) = 0; // part of NF1080

  virtual void SetTecEnable(bool on) = 0;
  virtual void GetTecEnable(bool *pVal) = 0;

  virtual void SetTempTec(float Temp) = 0;
  virtual void GetTempTec(float *pVal) = 0;

  virtual void GetFlag(TFlagState *pVal) = 0;
  virtual void SetFlag(TFlagState Val) = 0;

  virtual void SetFlagCycle(unsigned short CycleCount) = 0;

  virtual void SetTecA(unsigned short Val) = 0;
  virtual void GetTecA(unsigned short *pVal) = 0;

  virtual void SetTecB(unsigned short Val) = 0;
  virtual void GetTecB(unsigned short *pVal) = 0;

  virtual void SetTecC(unsigned short Val) = 0;
  virtual void GetTecC(unsigned short *pVal) = 0;

  virtual void SetTecD(unsigned short Val) = 0;
  virtual void GetTecD(unsigned short *pVal) = 0;

  virtual void SetSkim(unsigned short voltage) = 0;
  virtual void SetSkim_WaitForFlag(unsigned short voltage) = 0;
  virtual void GetSkim(unsigned short *pVal) = 0;

  virtual void SetSkim_Adjust(unsigned short voltage) = 0;
  virtual void GetSkim_Adjust(unsigned short *pVal) = 0;

  virtual void SetFid(unsigned short voltage) = 0;
  virtual void SetFid_WaitForFlag(unsigned short voltage) = 0;
  virtual void GetFid(unsigned short *pVal) = 0;

  virtual void SetFid_Adjust(unsigned short voltage) = 0;
  virtual void GetFid_Adjust(unsigned short *pVal) = 0;

  virtual void SetBiasEnable(bool on) = 0;
  virtual void GetBiasEnable(bool *pVal) = 0;

  virtual void SetPowerEnable(bool on) = 0;
  virtual void GetPowerEnable(bool *on) = 0;

  virtual void GetAntiFlicker(bool *on) = 0;
  virtual void SetAntiFlicker(bool on) = 0;

  virtual void SetClippedFormatPosition(unsigned short x, unsigned short y) = 0; // Part of NF1343
  virtual void GetClippedFormatPosition(unsigned short *pValx, unsigned short *pValy) = 0; // Part of NF1343

  virtual bool GetShortImagerCaps(void) = 0;

  virtual TPifMode GetPIFAIMode(unsigned char PifChn) = 0;
  virtual void SetPIFAIMode(TPifMode Val, unsigned char PifChn) = 0;

  virtual TPifMode GetPIFDIMode(unsigned char PifChn) = 0;
  virtual void SetPIFDIMode(TPifMode Val, unsigned char PifChn) = 0;

  virtual TPifMode GetPIFAOMode(unsigned char PifChn) = 0;
  virtual void SetPIFAOMode(TPifMode Val, unsigned char PifChn) = 0;

  virtual void SetPIFAOOutputMode(TPifAOOutputMode Val, unsigned char PifChn) = 0; // Part of T#541
  virtual TPifAOOutputMode GetPIFAOOutputMode(unsigned char PifChn) = 0; // Part of T#1168

  virtual TPifMode GetPIFDOMode(unsigned char PifChn) = 0;
  virtual void SetPIFDOMode(TPifMode Val, unsigned char PifChn) = 0; // Part of T#541

  virtual unsigned int GetPifSerialnumber(unsigned char PifIndex) = 0; // T#1940
  virtual unsigned short GetPifFWRev(unsigned char PifIndex) = 0; // T#1940

  virtual void TemporarilyToggleExternalFlag(void) = 0; // Part of T#918
  virtual bool ExternalFlag_IsTemporarilyToggled(void) = 0; // Part of T#918

  virtual unsigned short GetPIFAIResetPeakValleyHoldThreshold(void) = 0; // Part of T#1313
  virtual void SetPIFAIResetPeakValleyHoldThreshold(unsigned short val) = 0; // Part of T#1313

  virtual unsigned short GetPIFAIFlagThreshold(void) = 0;
  virtual void SetPIFAIFlagThreshold(unsigned short val) = 0;

  virtual bool GetPIFAIFlagOpenIfLower(void) = 0;
  virtual void SetPIFAIFlagOpenIfLower(bool val) = 0;

  virtual void SetPIFAOFlagOpen(unsigned short value, short PifChn) = 0;
  virtual void GetPIFAOFlagOpen(unsigned short *pVal, short PifChn) = 0;

  virtual void SetPIFAOFlagClosed(unsigned short value, short PifChn) = 0;
  virtual void GetPIFAOFlagClosed(unsigned short *pVal, short PifChn) = 0;

  virtual void SetPIFAOFlagMoving(unsigned short value, short PifChn) = 0;
  virtual void GetPIFAOFlagMoving(unsigned short *pVal, short PifChn) = 0;

  virtual void SetPIFAOFrameSync(unsigned short value, short PifChn) = 0; // Part of NF1083
  virtual void GetPIFAOFrameSync(unsigned short *pVal, short PifChn) = 0; // Part of NF1083

  virtual void SetPIFAOSamplePointTable(PSamplePoint Table, int TableSize, double SlopeFactor, double SlopeOffset, 
	                                    bool TempHighResolution, short PifChn) = 0; // Part of NF1283
  virtual int SetAutonomousSettings(PAutonomousSettings pSetting, bool delayed) = 0; // Part of T#621
  virtual int GetAutonomousSettings(PAutonomousSettings pSetting) = 0; // Part of T#1168

  virtual int SetConfigAutonomousPifDO(PAutonomousPifDOConfig pConfig) = 0; // Part of T#667
  virtual int GetConfigAutonomousPifDO(PAutonomousPifDOConfig pConfig) = 0; // Part of T#1168 

  virtual int SetConfigAutonomousPifFS(PAutonomousPifFSConfig pConfig) = 0; // Part of T#1818
  virtual int GetConfigAutonomousPifFS(PAutonomousPifFSConfig pConfig) = 0; // Part of T#1818 

  virtual int SetConfigAutonomousPifAO_ForMeasureArea(PAutonomousPifAOConfig pConfig) = 0; // Part of T#406
  virtual int GetConfigAutonomousPifAO_ForMeasureArea(PAutonomousPifAOConfig pConfig) = 0; // Part of T#1168

  virtual int SetConfigAutonomousPifAO_Digital(PAutonomousPifAOConfig pConfig) = 0; // Part of T#620, T#2003
  virtual int GetConfigAutonomousPifAO_Digital(PAutonomousPifAOConfig pConfig) = 0; // Part of T#1168, T#2003 

  virtual int SetConfigAutonomousPifAO_ForTInt(PAutonomousPifAOConfig pConfig) = 0; // Part of T#626
  virtual int GetConfigAutonomousPifAO_ForTInt(PAutonomousPifAOConfig pConfig) = 0; // Part of T#1168

  virtual int SetConfigAutonomousPifAO_ForFailsafe(PAutonomousPifAOConfig pConfig) = 0; // Part of T#741
  virtual int GetConfigAutonomousPifAO_ForFailsafe(PAutonomousPifAOConfig pConfig) = 0; // Part of T#1168

  virtual int SetAutonomousAreaCount(unsigned char MeasureAreaCount, unsigned char CalculatedAreaCount, bool delayed) = 0; // Part of T#753
  virtual int GetAutonomousAreaCount(unsigned char *MeasureAreaCount, unsigned char *CalculatedAreaCount) = 0; // Part of T#1168

  virtual int SetConfigAutonomousMeasureArea(PAutonomousAreaConfig pAAConfig, bool delayed) = 0; // Part of T#406
  virtual int GetConfigAutonomousMeasureArea(PAutonomousAreaConfig pAAConfig) = 0; // Part of T#1168

  virtual int SetConfigAutonomousCalculatedArea(PAutonomousAreaConfig pAAConfig, bool delayed) = 0; // Part of T#406
  virtual int GetConfigAutonomousCalculatedArea(PAutonomousAreaConfig pAAConfig) = 0; // Part of T#1168

  virtual void WriteFlash(bool delayed) = 0;
  virtual void WriteFlashNow(void) = 0;
  virtual void GetFlashingProgress(unsigned short *progress) = 0;// T#1840

  virtual int CheckForDelayedCommands(unsigned long time) = 0;

  virtual bool GetPIFDIFlagLowActive(void) = 0;
  virtual void SetPIFDIFlagLowActive(bool val) = 0;

  virtual unsigned short GetPIFAIThreshold(void) = 0;
  virtual void SetPIFAIThreshold(unsigned short val) = 0;

  //virtual bool GetPIFAIOpenIfLower(void) = 0;
  //virtual void SetPIFAIOpenIfLower(bool val) = 0;

  virtual bool GetPIFDILowActive(void) = 0;
  virtual void SetPIFDILowActive(bool val) = 0;

  virtual void GetTempChip(float *pVal) = 0;

  virtual void GetTempBox(float *pVal) = 0;

  virtual void GetTempFlag(float *pVal) = 0;

  virtual void GetTempOptics(float *pVal) = 0; // Part of T#403

  virtual void GetFocusmotorRange(unsigned short *pMin, unsigned short *pMax) = 0; // Part of T#404

  virtual void GetFocusmotorPos(unsigned short *pVal) = 0; // Part of T#404
  virtual void SetFocusmotorPos(unsigned short Val, bool delayed) = 0; // Part of T#404
  
  virtual void ReadCaliDataFromDevice(unsigned long address) = 0; // Part of T#407
  virtual void GetCaliDownloadConfig(PCaliDownloadConfig pConfig) = 0; // T#1871

  virtual TPifType GetPifType(void) = 0;

  virtual unsigned char GetPifDeviceCount(bool actual) = 0; // Part of T#605, T#729

  virtual void SetLaserEnable(bool Val) = 0;
  virtual void GetLaserEnable(bool *pVal) = 0;

  virtual bool GetChecksumValid(void) = 0; // Part of NF1100

  virtual void CheckFlagSource(void) = 0; // Part of NF1080
	
  virtual int FirmwareRead(unsigned char* Daten, unsigned short count) = 0; // Part of NFI1556
  virtual int FirmwareWrite(unsigned char* Daten, unsigned short count) = 0; // Part of NFI1556

  //virtual bool DFUFirmwareWrite(unsigned char* Daten, int count) = 0;

  virtual void ControlJTAGPort(unsigned char* ptr_data1, unsigned char* ptr_data2, unsigned char rw, unsigned short bit_cnt, unsigned char sel) = 0; // Part of NF1574

  virtual bool MCSFirmwareCheck(unsigned char* fw_from_file, int fw_size) = 0; // Part of NF1574
  virtual bool MCSFirmwareWrite(unsigned char* fw_data, int fw_size) = 0; // Part of NF1574
  
  virtual TImagerType GetDeviceType(void) = 0; // New with T#539

  virtual bool GetPifPortInfo(TPifPortType PifPortType, unsigned char PifChannel, unsigned char *PifIndex, unsigned char *PinIndex) = 0; // Part of T#565

  virtual int GetCurrentEthernetVideoFormat(PVideoFormat videoformat) = 0; // T#1937
  virtual int GetEthernetVideoFormat(unsigned char VideoFormatIndex, PVideoFormat videoformat) = 0; // T#1937
  virtual int SetEthernetVideoFormat(unsigned char VideoFormatIndex) = 0; // T#1937

  virtual int SetTCPIPConfig(unsigned long DeviceAddress, unsigned long SendToAddress, unsigned long SubnetMask, unsigned short PortNumber) = 0; // Part of T#579,T#1084
  virtual int GetTCPIPConfig(unsigned long *DeviceAddress, unsigned long *SendToAddress, unsigned long *SubnetMask, unsigned short *PortNumber) = 0; // Part of T#579,T#1084

  virtual void SetReportHID(bool val) = 0; // T#1829
  virtual bool GetReportHID(void) = 0; // T#1829

  virtual void SetTemperatureMode(bool val) = 0; // T#1858
  virtual void GetTemperatureMode(bool *pVal) = 0; // T#1858

  void SetTempTecMax(float val);
  float GetTempTecMax();

  void SetTempTecMin(float val);
  float GetTempTecMin();

  void SetTempBoxOffset(float val);
  float GetTempBoxOffset();

  void SetTempChipFactor(float val);
  float GetTempChipFactor();

  void SetTempChipOffset(float val);
  float GetTempChipOffset();

  void SetTempFlagOffset(float val);
  float GetTempFlagOffset();

  void SetTempOpticsOffset(float val); // Part of T#403
  float GetTempOpticsOffset(); // Part of T#403

  void SetTempTecOffset(float val);
  float GetTempTecOffset();

  void SetTempTecGain(float val);
  float GetTempTecGain();

  void SetPifAIOffset(float val, unsigned char PifChn); // Part of NF1080
  float GetPifAIOffset(unsigned char PifChn); // Part of NF1080

  void SetPifAIGain(float val, unsigned char PifChn); // Part of NF1080
  float GetPifAIGain(unsigned char PifChn); // Part of NF1080

  void SetPifAOOffset(float val, unsigned char  PifChn); // Part of NF1080
  float GetPifAOOffset(unsigned char  PifChn); // Part of NF1080

  void SetPifAOGain(float val, unsigned char  PifChn); // Part of NF1080
  float GetPifAOGain(unsigned char  PifChn); // Part of NF1080

  int GetFlagSource(void); // Part of NF1080, changed with T#415/T#416

protected:

  float TempTecMax;

  float TempTecMin;

  float TempBoxOffset;

  float TempChipFactor;

  float TempChipOffset;

  float TempFlagOffset;

  float TempOpticsOffset; // Part of T#403

  float TempTecOffset;

  float TempTecGain;

  float *PifAI_Offset;// Part of NF1080, changed with T#416
  float *PifAI_Gain;// Part of NF1080, changed with T#416

  float *PifAO_Offset;// Part of NF1080, changed with T#415
  float *PifAO_Gain;// Part of NF1080, changed with T#415

  int FlagSource; // Part of NF1080, changed with T#415/T#416

  unsigned short FocusmotorMaxPos; // Part of T#404
  unsigned short FocusmotorMinPos; // Part of T#404

};

#endif
