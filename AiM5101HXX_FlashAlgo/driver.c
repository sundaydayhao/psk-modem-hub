/*******************************************************************************
 * Experimental AiM5101HXX embedded flash driver for T-Head CDK flash algorithm.
 *
 * This implements only Main Memory operations described in
 * "AiM5101HXX chip user guide", EFC chapter. It does not modify option bytes.
 ******************************************************************************/

#define ERROR_INIT             -200
#define ERROR_READID           -201
#define ERROR_PROGRAM          -202
#define ERROR_READ             -203
#define ERROR_ERASE            -204
#define ERROR_CHIPERASE        -205
#define ERROR_UNINIT           -206
#define ERROR_CHECKSUM         -207
#define ERROR_FLASH_INDEX      -208

#define AIM51_FLASH_BASE       0x08000000UL
#define AIM51_FLASH_SIZE       0x00080000UL
#define AIM51_FLASH_END        (AIM51_FLASH_BASE + AIM51_FLASH_SIZE)
#define AIM51_PAGE_SIZE        0x400UL
#define AIM51_ROW_SIZE         0x200UL
#define AIM51_DWORD_SIZE       8UL

#define AIM51_EFC_BASE         0x61001000UL

#define REG32(addr)            (*(volatile unsigned int *)(addr))

#define EF_ACC_CTRL            REG32(AIM51_EFC_BASE + 0x00)
#define EF_OPKEY               REG32(AIM51_EFC_BASE + 0x04)
#define EF_OPTRG               REG32(AIM51_EFC_BASE + 0x08)
#define EF_OPSET               REG32(AIM51_EFC_BASE + 0x0C)
#define EF_STAT                REG32(AIM51_EFC_BASE + 0x10)
#define EF_OPADDR              REG32(AIM51_EFC_BASE + 0x14)
#define EF_RDBUF0              REG32(AIM51_EFC_BASE + 0x18)
#define EF_RDBUF1              REG32(AIM51_EFC_BASE + 0x1C)
#define EF_PROGBUF0            REG32(AIM51_EFC_BASE + 0x20)
#define EF_PROGBUF1            REG32(AIM51_EFC_BASE + 0x24)
#define INT_RAW                REG32(AIM51_EFC_BASE + 0x58)
#define INT_CLR                REG32(AIM51_EFC_BASE + 0x60)
#define UID0                   REG32(AIM51_EFC_BASE + 0xC0)
#define UID1                   REG32(AIM51_EFC_BASE + 0xC4)
#define UID2                   REG32(AIM51_EFC_BASE + 0xC8)
#define UID3                   REG32(AIM51_EFC_BASE + 0xCC)

#define EF_STAT_BUSY           (1U << 0)
#define EF_STAT_RDATA_VLD      (1U << 1)
#define EF_STAT_CFG_LOCK       (1U << 8)

#define EFC_CMD_PAGE_ERASE     1U
#define EFC_CMD_CHIP_ERASE     2U
#define EFC_CMD_PROGRAM        3U
#define EFC_CMD_READ           4U

#define EFC_CFG_KEY            0x6969U
#define EFC_START              1U

#define INT_EOP                (1U << 0)
#define INT_ERROR_MASK         ((1U << 8) | (1U << 7) | (1U << 6) | \
                                (1U << 5) | (1U << 4) | (1U << 3) | \
                                (1U << 2))
#define INT_ALL                0x1FFU

static int wait_ready(void)
{
    unsigned int timeout = 8000000U;

    while ((EF_STAT & EF_STAT_BUSY) != 0U) {
        if (timeout-- == 0U) {
            return -1;
        }
    }

    return 0;
}

static int unlock_cfg(void)
{
    if (wait_ready() != 0) {
        return -1;
    }

    EF_OPKEY = EFC_CFG_KEY;
    return ((EF_STAT & EF_STAT_CFG_LOCK) == 0U) ? 0 : -1;
}

static unsigned int make_opaddr(unsigned int flash_addr)
{
    unsigned int off = flash_addr - AIM51_FLASH_BASE;
    unsigned int page = off / AIM51_PAGE_SIZE;
    unsigned int in_page = off % AIM51_PAGE_SIZE;
    unsigned int row = in_page / AIM51_ROW_SIZE;
    unsigned int word = (in_page % AIM51_ROW_SIZE) / AIM51_DWORD_SIZE;

    return ((page & 0x1FFU) << 7) | ((row & 0x1U) << 6) | (word & 0x3FU);
}

static int check_range(unsigned int addr, unsigned int length)
{
    if (addr < AIM51_FLASH_BASE) {
        return -1;
    }
    if (addr > AIM51_FLASH_END) {
        return -1;
    }
    if (length > (AIM51_FLASH_END - addr)) {
        return -1;
    }
    return 0;
}

static int start_command(unsigned int cmd)
{
    INT_CLR = INT_ALL;
    EF_OPSET = cmd;
    EF_OPTRG = EFC_START;

    if (wait_ready() != 0) {
        return -1;
    }

    if ((INT_RAW & INT_ERROR_MASK) != 0U) {
        INT_CLR = INT_ALL;
        return -1;
    }

    INT_CLR = INT_ALL;
    return 0;
}

static int efc_read64(unsigned int addr, unsigned int *lo, unsigned int *hi)
{
    if (unlock_cfg() != 0) {
        return -1;
    }

    EF_OPADDR = make_opaddr(addr);
    if (start_command(EFC_CMD_READ) != 0) {
        return -1;
    }

    if ((EF_STAT & EF_STAT_RDATA_VLD) == 0U) {
        return -1;
    }

    *lo = EF_RDBUF0;
    *hi = EF_RDBUF1;
    return 0;
}

static int efc_program64(unsigned int addr, unsigned int lo, unsigned int hi)
{
    if ((addr & (AIM51_DWORD_SIZE - 1U)) != 0U) {
        return -1;
    }

    if (unlock_cfg() != 0) {
        return -1;
    }

    EF_OPADDR = make_opaddr(addr);
    EF_PROGBUF0 = lo;
    EF_PROGBUF1 = hi;
    return start_command(EFC_CMD_PROGRAM);
}

static void copy_bytes(char *dst, const char *src, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        dst[i] = src[i];
    }
}

int flashInit(void)
{
    INT_CLR = INT_ALL;
    return wait_ready() == 0 ? 0 : ERROR_INIT;
}

int flashUnInit(void)
{
    return wait_ready() == 0 ? 0 : ERROR_UNINIT;
}

int flashID(unsigned int *flashID)
{
    if (flashID == 0) {
        return ERROR_READID;
    }

    /*
     * There is no JEDEC ID for embedded flash. Return a stable pseudo-ID mixed
     * from the chip UID block so CDK can display a nonzero value.
     */
    *flashID = 0x51010000U ^ UID0 ^ UID1 ^ UID2 ^ UID3;
    return 0;
}

int flashProgram(char *dst, char *src, int length)
{
    unsigned int addr = (unsigned int)dst;
    unsigned int end;
    unsigned int aligned;

    if (length <= 0) {
        return 0;
    }
    if (check_range(addr, (unsigned int)length) != 0) {
        return ERROR_PROGRAM;
    }

    end = addr + (unsigned int)length;
    aligned = addr & ~(AIM51_DWORD_SIZE - 1U);

    while (aligned < end) {
        unsigned int lo;
        unsigned int hi;
        unsigned int i;
        unsigned char image[8];

        if (efc_read64(aligned, &lo, &hi) != 0) {
            return ERROR_PROGRAM;
        }

        image[0] = (unsigned char)(lo >> 0);
        image[1] = (unsigned char)(lo >> 8);
        image[2] = (unsigned char)(lo >> 16);
        image[3] = (unsigned char)(lo >> 24);
        image[4] = (unsigned char)(hi >> 0);
        image[5] = (unsigned char)(hi >> 8);
        image[6] = (unsigned char)(hi >> 16);
        image[7] = (unsigned char)(hi >> 24);

        for (i = 0; i < AIM51_DWORD_SIZE; i++) {
            unsigned int byte_addr = aligned + i;
            if ((byte_addr >= addr) && (byte_addr < end)) {
                image[i] = (unsigned char)src[byte_addr - addr];
            }
        }

        lo = ((unsigned int)image[0]) |
             ((unsigned int)image[1] << 8) |
             ((unsigned int)image[2] << 16) |
             ((unsigned int)image[3] << 24);
        hi = ((unsigned int)image[4]) |
             ((unsigned int)image[5] << 8) |
             ((unsigned int)image[6] << 16) |
             ((unsigned int)image[7] << 24);

        if (efc_program64(aligned, lo, hi) != 0) {
            return ERROR_PROGRAM;
        }

        aligned += AIM51_DWORD_SIZE;
    }

    return 0;
}

int flashRead(char *dst, char *src, int length)
{
    if (length < 0) {
        return ERROR_READ;
    }
    if (check_range((unsigned int)src, (unsigned int)length) != 0) {
        return ERROR_READ;
    }

    copy_bytes(dst, src, length);
    return 0;
}

int flashErase(char *dst, int length)
{
    unsigned int addr = (unsigned int)dst;
    unsigned int first;
    unsigned int last;
    unsigned int page;

    if (length <= 0) {
        return 0;
    }
    if (check_range(addr, (unsigned int)length) != 0) {
        return ERROR_ERASE;
    }

    first = (addr - AIM51_FLASH_BASE) / AIM51_PAGE_SIZE;
    last = (addr + (unsigned int)length - 1U - AIM51_FLASH_BASE) / AIM51_PAGE_SIZE;

    for (page = first; page <= last; page++) {
        if (unlock_cfg() != 0) {
            return ERROR_ERASE;
        }

        EF_OPADDR = ((page & 0x1FFU) << 7);
        if (start_command(EFC_CMD_PAGE_ERASE) != 0) {
            return ERROR_ERASE;
        }
    }

    return 0;
}

int flashChipErase(void)
{
    if (unlock_cfg() != 0) {
        return ERROR_CHIPERASE;
    }

    return start_command(EFC_CMD_CHIP_ERASE) == 0 ? 0 : ERROR_CHIPERASE;
}

int flashChecksum(char *dst, int length, int checksum)
{
    int i;
    int sum = 0;

    if (length < 0) {
        return ERROR_CHECKSUM;
    }
    if (check_range((unsigned int)dst, (unsigned int)length) != 0) {
        return ERROR_CHECKSUM;
    }

    for (i = 0; i < length; i++) {
        sum += (unsigned char)dst[i];
    }

    return sum == checksum ? 0 : ERROR_CHECKSUM;
}

int flashIndex(int index)
{
    return index == 0 ? 0 : ERROR_FLASH_INDEX;
}

#define DEBUG_DRIVER 0

int flashTest(void)
{
#if DEBUG_DRIVER
    unsigned int id;
    if (flashID(&id) != 0) {
        return ERROR_READID;
    }
    return (int)id;
#else
    return 0;
#endif
}
