#ifndef CEVENTPROCESSOR_H
#define CEVENTPROCESSOR_H


class CEventProcessor
{
public:
    CEventProcessor();

public:
    typedef enum {evNone=0,
                  evMaketStart,     // 1
                  evCarOnKt,        // 2
                  evKpnInp,         // 3 +-TODO
                  evBpWarning,      // 4 -TODO
                  evBpFatal,        // 5 -TODO
                  evExt,            // 6 +-TODO
                  evActionDone,     // 7
                  evLighterState,   // 8
                  evMaketStop=255} eEventType;

    typedef enum {acNoAction=0,
                  acForkSetPos,     // 1
                  acLighterSetState,// 2
                  acKtCmd,          // 3
                  acKtRepCmd,       // 4
                  acKpnSetPwm,      // 5
                  acKpnSetSw,       // 6
                  acSendUdpPacket,  // 7
                  acEventOff,       // 8
                  acEventOn,        // 9
                  acEvChainOff,     // 10
                  acEvChainOn,      // 11
                  acStrangeAction=252,
                  acMaketStop=255} eActionType;

    void renewFromDb(char* txtFileBuffer);

    void eventloop();

    void slEvent(int evType, int srcId, int dstId, char* data, int dsize);
    void eventFilter(int evType, int srcId, int dstId, char* data, int dsize);
    void slDoAction(int acId);

};

#endif // CEVENTPROCESSOR_H
