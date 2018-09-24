/**
* \brief Message type
*
* This enumeration defines the POWERLINK message type IDs.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" on page 349.
*/
typedef enum
{
    kMsgTypeNonPowerlink        = 0x00,                     ///< Defines non POWERLINK Frame
    kMsgTypeSoc                 = 0x01,                     ///< Defines Start of Cycle Frame
    kMsgTypePreq                = 0x03,                     ///< Defines Poll Request Frame
    kMsgTypePres                = 0x04,                     ///< Defines Poll Response Frame
    kMsgTypeSoa                 = 0x05,                     ///< Defines Start of Asynchronous Cycle Frame
    kMsgTypeAsnd                = 0x06,                     ///< Defines Asynchronous Send Frame
    kMsgTypeAmni                = 0x07,                     ///< Defines Active Managing Node Indication Frame
    kMsgTypeAInv                = 0x0D,                     ///< Defines Asynchronous Invite Frame
} eMsgType;

/**
\brief Message type data type

Data type for the enumerator \ref eMsgType.
*/
typedef UINT8 tMsgType;

// byte-align structures
#ifdef _MSC_VER
#pragma pack(push, packing)
#pragma pack(1)
#define PACK_STRUCT
#elif defined(__GNUC__)
#define PACK_STRUCT    __attribute__((packed))
#else /* defined(__GNUC__) */
#error You must Byte-align these structures with the appropriate compiler directives
#endif /* defined(__GNUC__) */

/**
* \brief Frame Data
*
* This union contains the various POWERLINK messages types.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" chapter 4.6.1.1.1.
*/
typedef union
{
    tSocFrame               soc;                            ///< Contains the Soc frame structure (Multicast)
    tPreqFrame              preq;                           ///< Contains the Poll request frame structure (Unicast)
    tPresFrame              pres;                           ///< Contains the Poll response frame structure (Multicast)
    tSoaFrame               soa;                            ///< Contains the Start of asynchronous frame structure (Multicast)
    tAsndFrame              asnd;                           ///< Contains the Asynchronous send frame structure (Multicast)
} tFrameData;

/**
* \brief Start of cycle Frame (SoC)
*
* This structure contains the layout of an SoC frame. At the beginning of a POWERLINK cycle, the MN sends an SoC frame to all nodes via Ethernet
* multicast. The send and receive time of this frame shall be the basis for the common timing of all the nodes.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" chapter 4.2.4.1.1 and 4.6.1.1.2.
* */
typedef struct
{
    UINT8                   reserved1;                      ///< Reserved (Offset 17)
    UINT8                   flag1;                          ///< Contains the flag Multiplexed Cycle Completed (MC) and the flag Prescaled Slot (PS). (Offset 18)
    UINT8                   flag2;                          ///< Reserved (Offset 19)
    tNetTime                netTimeLe;                      ///< Optional, if D_NMT_NetTimeIsRealTime_BOOL is set. MN may distribute the starting time of the POWERLINK cycle. (Offset 20)
    UINT64                  relativeTimeLe;                 ///< Optional, if D_NMT_RelativeTime_BOOL is set. The relative time (in us) is incremented in every cycle by the cycle time. It shall be set to 0 when NMT state equals NMT_GS_INITIALISING. (Offset 28)
} PACK_STRUCT tSocFrame;

/**
* \brief Poll Request Frame (PReq)
*
* This structure defines the PReq frame. It is transmitted cyclically by the MN to a CN via Ethernet unicast.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" chapter 4.6.1.1.3.
*/
typedef struct
{
    UINT8                   reserved1;                      ///< Reserved (Offset 17)
    UINT8                   flag1;                          ///< Contains the flags Multiplexed Slot (MS), Exception Acknowledge (EA) and Ready (RD). (Offset 18)
    UINT8                   flag2;                          ///< Reserved (Offset 19)
    UINT8                   pdoVersion;                     ///< Indicates the PDO Version (Offset 20)
    UINT8                   reserved2;                      ///< Reserved (Offset 21)
    UINT16                  sizeLe;                         ///< Contains the number of payload data octets (Offset 22)
    UINT8                   aPayload[256];                  ///< Payload (Offset 24)
} PACK_STRUCT tPreqFrame;

/**
* \brief Poll Response Frame (PRes)
*
* This structure defines the PRes frame. It is transmitted cyclically via Ethernet multicast.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" chapter 4.6.1.1.4.
*/
typedef struct
{
    UINT8                   nmtStatus;                      ///< Defines the NMT state. (Offset 17)
    UINT8                   flag1;                          ///< Contains the flags multiplexed Slot (MS), Exception New (EN) and Ready (RD). (Offset 18)
    UINT8                   flag2;                          ///< Contains the flags Priority (PR) and Request to send (RS). (Offset 19)
    UINT8                   pdoVersion;                     ///< Indicates the PDO Version. (Offset 20)
    UINT8                   reserved2;                      ///< Reserved (Offset 21)
    UINT16                  sizeLe;                         ///< Contains the number of payload data octets. (Offset 22)
    UINT8                   aPayload[256];                  ///< Payload (Offset 24)
} PACK_STRUCT tPresFrame;

/**
* \brief POWERLINK Frame
*
* This structure contains the POWERLINK Basic Frame Format. The POWERLINK Basic Frame format shall be encapsulated by the Ethernet wrapper consisting of
* 14 octets of leading Ethernet header (Destination and Source MAC addresses, EtherType) and 4 octets of terminating CRC32 checksum.
*
* For more information consult the POWERLINK specification document "EPSG DS 301 V1.2.0" chapter 4.6.1.1.
*/
typedef struct
{
    UINT8                   aDstMac[6];                     ///< Contains the MAC address of the addressed nodes (Offset 0)
    UINT8                   aSrcMac[6];                     ///< Contains the MAC address of the transmitting node (Offset 6)
    UINT16                  etherType;                      ///< Contains the Ethernet message type (big endian) (Offset 12)
    tMsgType                messageType;                    ///< Contains the POWERLINK message type (Offset 14)
    UINT8                   dstNodeId;                      ///< Contains the POWERLINK node ID of the addressed nodes (Offset 15)
    UINT8                   srcNodeId;                      ///< Contains the POWERLINK node ID of the transmitting node (Offset 16)
    tFrameData              data;                           ///< Contains the Frame Data (Offset 17)
} PACK_STRUCT tPlkFrame;

// reset byte-align of structures
#ifdef _MSC_VER
#pragma pack(pop, packing)
#endif /* _MSC_VER */
