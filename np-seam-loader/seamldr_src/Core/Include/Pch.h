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

#ifndef PCH_H
#define PCH_H
#pragma pack (1)





#define IOD_IOE_REG_INIT            0x30010000
#define PCH_ACPI_BASE_ADDRESS       0x0400 // ACPI Power Management I/O Register Base Address

#define ME_DISABLED                 0xFFFFFFFF

UINT32 ReadHeciDeviceDwordRegister   (UINT32);
//added for UART support
#define PCI_CONFIG_ADDRESS        0x80000060
#define PCIEX_BASE_ADDRESS        0xE0000000
#define UART2_DEV                 0x19
#define UART2_FUNC                0x2
#define UART2_BAR                0xFE036000
#define UART2_RESET_OFFSET        0x204  
#define UART2_CLK_OFFSET          0x200 

#pragma pack ()
#endif

