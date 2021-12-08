//**********************************************************************;
//*                                                                    *;
//* Intel Proprietary                                                  *;
//*                                                                    *;
//* Copyright 2021 Intel Corporation All Rights Reserved.              *;
//*                                                                    *;
//* Your use of this software is governed by the TDX Source Code       *;
//* LIMITED USE LICENSE.                                               *;
//*                                                                    *;
//* The Materials are provided "as is," without any express or         *;
//* implied warranty of any kind including warranties of               *;
//* merchantability, non-infringement, title, or fitness for a         *;
//* particular purpose.                                                *;
//*                                                                    *;
//**********************************************************************;

#ifndef __MMIO_H__
#define __MMIO_H__
#pragma   pack (push, 1)

#ifndef __ASM__

//#define reg64pub(R)   (*(volatile UINT64 *) (LT_PUB_BASE + offsetof(TXT, R)))
//#define reg64prv(R)   (*(volatile UINT64 *) (LT_PRV_BASE + offsetof(TXT, R)))

//#define reg32pub(R)   (*(volatile UINT32 *) (LT_PUB_BASE + offsetof(TXT, R)))
//#define reg32prv(R)   (*(volatile UINT32 *) (LT_PRV_BASE + offsetof(TXT, R)))

//#define reg64mmio(R)  (*(volatile UINT64 *) (R))
//#define reg32mmio(R)  (*(volatile UINT32 *) (R))
//#define reg16mmio(R)  (*(volatile UINT16 *) (R))
//#define reg8mmio(R)   (*(volatile UINT8 *) (R))

#if  TRACE_WITH_FUNCTIONS & TRACE_MMIO

extern UINT8 __ReadMmioByte(UINT32 addr);
extern void __WriteMmioByte(UINT32 addr, UINT8);
extern UINT16 __ReadMmioWord(UINT32 addr);
extern void __WriteMmioWord(UINT32 addr, UINT16);
extern UINT32 __ReadMmioDword(UINT32 addr);
extern void __WriteMmioDword(UINT32 addr, UINT32);
extern UINT64 __ReadMmioQword(UINT32 addr);
extern void __WriteMmioQword(UINT32 addr, UINT64);

extern UINT8 ReadMmioByteTr(UINT32 addr, const char *name, UINT32 line);
extern void WriteMmioByteTr(UINT32 addr, UINT8, const char *name, UINT32 line);
extern UINT16 ReadMmioWordTr(UINT32 addr, const char *name, UINT32 line);
extern void WriteMmioWordTr(UINT32 addr, UINT16, const char *name, UINT32 line);
extern UINT32 ReadMmioDwordTr(UINT32 addr, const char *name, UINT32 line);
extern void WriteMmioDwordTr(UINT32 addr, UINT32, const char *name, UINT32 line);
extern UINT64 ReadMmioQwordTr(UINT32 addr, const char *name, UINT32 line);
extern void WriteMmioQwordTr(UINT32 addr, UINT64, const char *name, UINT32 line);

#define ReadMmioByte(R)			      ReadMmioByteTr(R,__FUNCTION__, __LINE__)
#define WriteMmioByte(R,D)		    WriteMmioByteTr(R,D,__FUNCTION__, __LINE__)
#define ReadMmioWord(R)			      ReadMmioWordTr(R,__FUNCTION__, __LINE__)
#define WriteMmioWord(R,D)		    WriteMmioWordTr(R,D,__FUNCTION__, __LINE__)
#define ReadMmioDword(R)			    ReadMmioDwordTr(R,__FUNCTION__, __LINE__)
#define WriteMmioDword(R,D)		    WriteMmioDwordTr(R,D,__FUNCTION__, __LINE__)
#define ReadMmioQword(R)			    ReadMmioQwordTr(R,__FUNCTION__, __LINE__)
#define WriteMmioQword(R,D)		    WriteMmioQwordTr(R,D,__FUNCTION__, __LINE__)

#else

#define ReadMmioByte(R)			  (*(volatile UINT8 *) (R))
#define WriteMmioByte(R,D)	  (*(volatile UINT8 *) (R)) = (D)
#define ReadMmioWord(R)			  (*(volatile UINT16 *) (R))
#define WriteMmioWord(R,D)		(*(volatile UINT16 *) (R)) = (D)
#define ReadMmioDword(R)			(*(volatile UINT32 *) (R))
#define WriteMmioDword(R,D)		(*(volatile UINT32 *) (R)) = (D)
#define ReadMmioQword(R)			(*(volatile UINT64 *) (R))
#define WriteMmioQword(R,D)		(*(volatile UINT64 *) (R)) = (D)

#define __ReadMmioByte(R)       ReadMmioByte(R)
#define __WriteMmioByte(R,D)    WriteMmioByte(R,D)
#define __ReadMmioWord(R)       ReadMmioWord(R)
#define __WriteMmioWord(R,D)    WriteMmioWord(R,D)
#define __ReadMmioDword(R)      ReadMmioDword(R)
#define __WriteMmioDword(R,D)   WriteMmioDword(R,D)
#define __ReadMmioQword(R)      ReadMmioQword(R)
#define __WriteMmioQword(R,D)   WriteMmioQword(R,D)

#endif

#define ReadPublicDword(R)			  ReadMmioDword(LT_PUB_BASE + offsetof(TXT, R))
#define WritePublicDword(R,D)		  WriteMmioDword(LT_PUB_BASE + offsetof(TXT, R),D)
#define ReadPublicQword(R)			  ReadMmioQword(LT_PUB_BASE + offsetof(TXT, R))
#define WritePublicQword(R,D)		  WriteMmioQword(LT_PUB_BASE + offsetof(TXT, R),D)

#define ReadPrivateDword(R)			  ReadMmioDword(LT_PRV_BASE + offsetof(TXT, R))
#define WritePrivateDword(R,D)	  WriteMmioDword(LT_PRV_BASE + offsetof(TXT, R),D)
#define ReadPrivateQword(R)			  ReadMmioQword(LT_PRV_BASE + offsetof(TXT, R))
#define WritePrivateQword(R,D)	  WriteMmioQword(LT_PRV_BASE + offsetof(TXT, R),D)

#define __ReadPublicDword(R)      __ReadMmioDword(LT_PUB_BASE + offsetof(TXT, R))
#define __WritePublicDword(R,D)   __WriteMmioDword(LT_PUB_BASE + offsetof(TXT, R),D)
#define __ReadPublicQword(R)      __ReadMmioQword(LT_PUB_BASE + offsetof(TXT, R))
#define __WritePublicQword(R,D)   __WriteMmioQword(LT_PUB_BASE + offsetof(TXT, R),D)

#define __ReadPrivateDword(R)     __ReadMmioDword(LT_PRV_BASE + offsetof(TXT, R))
#define __WritePrivateDword(R,D)  __WriteMmioDword(LT_PRV_BASE + offsetof(TXT, R),D)
#define __ReadPrivateQword(R)     __ReadMmioQword(LT_PRV_BASE + offsetof(TXT, R))
#define __WritePrivateQword(R,D)  __WriteMmioQword(LT_PRV_BASE + offsetof(TXT, R),D)

#define ReadTpm3Qword(R)        ReadMmioQword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define WriteTpm3Qword(R,D)     WriteMmioQword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define ReadTpm3Dword(R)        ReadMmioDword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define WriteTpm3Dword(R,D)     WriteMmioDword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define ReadTpm3Word(R)         ReadMmioWord(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define WriteTpm3Word(R,D)      WriteMmioWord(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define ReadTpm3Byte(R)         ReadMmioByte(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define WriteTpm3Byte(R,D)      WriteMmioByte(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)

#define ReadTpm0Qword(R)        ReadMmioQword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define WriteTpm0Qword(R,D)     WriteMmioQword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define ReadTpm0Dword(R)        ReadMmioDword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define WriteTpm0Dword(R,D)     WriteMmioDword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define ReadTpm0Word(R)         ReadMmioWord(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define WriteTpm0Word(R,D)      WriteMmioWord(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define ReadTpm0Byte(R)         ReadMmioByte(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define WriteTpm0Byte(R,D)      WriteMmioByte(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)

#define __ReadTpm3Qword(R)        __ReadMmioQword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define __WriteTpm3Qword(R,D)     __WriteMmioQword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm3Dword(R)        __ReadMmioDword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define __WriteTpm3Dword(R,D)     __WriteMmioDword(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm3Word(R)         __ReadMmioWord(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define __WriteTpm3Word(R,D)      __WriteMmioWord(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm3Byte(R)         __ReadMmioByte(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R))
#define __WriteTpm3Byte(R,D)      __WriteMmioByte(TPM_ACCESS_L3 + offsetof(TPM_FIFO, R),D)

#define __ReadTpm0Qword(R)        __ReadMmioQword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define __WriteTpm0Qword(R,D)     __WriteMmioQword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm0Dword(R)        __ReadMmioDword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define __WriteTpm0Dword(R,D)     __WriteMmioDword(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm0Word(R)         __ReadMmioWord(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define __WriteTpm0Word(R,D)      __WriteMmioWord(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
#define __ReadTpm0Byte(R)         __ReadMmioByte(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R))
#define __WriteTpm0Byte(R,D)      __WriteMmioByte(TPM_ACCESS_L0 + offsetof(TPM_FIFO, R),D)
                                                                            
#define ReadCrb3Qword(R)        ReadMmioQword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define WriteCrb3Qword(R,D)     WriteMmioQword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define ReadCrb3Dword(R)        ReadMmioDword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define WriteCrb3Dword(R,D)     WriteMmioDword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define ReadCrb3Word(R)         ReadMmioWord(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define WriteCrb3Word(R,D)      WriteMmioWord(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define ReadCrb3Byte(R)         ReadMmioByte(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define WriteCrb3Byte(R,D)      WriteMmioByte(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)

#define ReadCrb0Qword(R)        ReadMmioQword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define WriteCrb0Qword(R,D)     WriteMmioQword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)
#define ReadCrb0Dword(R)        ReadMmioDword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define WriteCrb0Dword(R,D)     WriteMmioDword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)
#define ReadCrb0Word(R)         ReadMmioWord(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define WriteCrb0Word(R,D)      WriteMmioWord(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)
#define ReadCrb0Byte(R)         ReadMmioByte(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define WriteCrb0Byte(R,D)      WriteMmioByte(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)

#define __ReadCrb3Qword(R)        __ReadMmioQword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define __WriteCrb3Qword(R,D)     __WriteMmioQword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define __ReadCrb3Dword(R)        __ReadMmioDword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define __WriteCrb3Dword(R,D)     __WriteMmioDword(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define __ReadCrb3Word(R)         __ReadMmioWord(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define __WriteCrb3Word(R,D)      __WriteMmioWord(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)
#define __ReadCrb3Byte(R)         __ReadMmioByte(TPM_ACCESS_L3 + offsetof(TPM_CRB, R))
#define __WriteCrb3Byte(R,D)      __WriteMmioByte(TPM_ACCESS_L3 + offsetof(TPM_CRB, R),D)

#define __ReadCrb0Dword(R)        __ReadMmioDword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define __WriteCrb0Dword(R,D)     __WriteMmioDword(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)
#define __ReadCrb0Word(R)         __ReadMmioWord(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define __WriteCrb0Word(R,D)      __WriteMmioWord(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)
#define __ReadCrb0Byte(R)         __ReadMmioByte(TPM_ACCESS_L0 + offsetof(TPM_CRB, R))
#define __WriteCrb0Byte(R,D)      __WriteMmioByte(TPM_ACCESS_L0 + offsetof(TPM_CRB, R),D)

#define WaitPrivateStatusBitSet(R,M)     __WaitRegisterBitSet(LT_PRV_BASE + offsetof(TXT, R),M)                                       
#define WaitPrivateStatusBitClear(R,M)   __WaitRegisterBitClear(LT_PRV_BASE + offsetof(TXT, R),M)                                       

#endif // __ASM__

#pragma pack (pop)
#endif
