/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SPI.h"

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_SPI_FLASH		3

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
DSTATUS stat=STA_NOINIT;
	switch (pdrv) {
	case ATA :
    break;
	case MMC :
     break;
	case USB :
	   break;
	case DEV_SPI_FLASH :
		 if(MX25LX_sFLASH_ID==Read_SPI_Flash_ID())
		 {
			 stat &=~STA_NOINIT;
		 }
		 else
		 {
			 stat=STA_NOINIT;
		 }
		 break;	     
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat=STA_NOINIT;
	int i;

	switch (pdrv) {
	case ATA :
		break;
	case MMC :
		break;
	case USB :
		break;
	case DEV_SPI_FLASH :
		Flash_SPI_Config();
	  i=500;
	  while(i--);
	  //need weak up;
	  stat=disk_status(DEV_SPI_FLASH);
	  break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res=RES_PARERR;
	
	switch (pdrv) {
	case ATA :
		break;

	case MMC :
		break;

	case USB :
		break;
	case DEV_SPI_FLASH :
		SPI_FLASH_BufferRead(buff,sector<<12,count<<12);
	  res=RES_OK;
	  break;
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res=RES_PARERR;
//	uint32_t WriteAddr;
	if(!count)
	{
		return RES_PARERR;
	}

	switch (pdrv) {
	case ATA :
	  break;
	case MMC :
		break;

	case USB :
		break;
	case DEV_SPI_FLASH :
		SPI_FLASH_SectorErase(sector<<12);
		SPI_FLASH_BufferWrite((uint8_t *)buff,sector<<12,count<<12);
	  res=RES_OK;
	  break;
	}

	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res=RES_PARERR;
	switch (pdrv) {
	case ATA :

		break;

	case MMC :

		break;

	case USB :

		break;
	case DEV_SPI_FLASH :
		switch (cmd) {
			case GET_SECTOR_COUNT :
				*(DWORD *)buff = 2560;
				break;
			case GET_SECTOR_SIZE :
				*(WORD *)buff = 4096;
				break;
			case GET_BLOCK_SIZE :
				*(DWORD *)buff = 1;
				break;
		}
		res=RES_OK;
		break;
	}
	return res;
}
#endif

__weak DWORD get_fattime(void)
{
	return ((DWORD)(2015-1980)<<25)
				 |((DWORD)1<<21)
			   |((DWORD)1<<16)
	       |((DWORD)0<<11)
	       |((DWORD)0<<5)
	       |((DWORD)0<<1);
}

