/*
 * Copyright (C) 2016 OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    sys_spiffs  SPIFFS integration
 * @ingroup     pkg_spiffs
 * @brief       RIOT integration of SPIFFS
 *
 * The RIOT integration of SPIFFS follows the SPIFFS wiki:
 * https://github.com/pellepl/spiffs/wiki/Integrate-spiffs#integrating-spiffs
 *
 * @{
 *
 * @file
 * @brief       SPIFFS integration with vfs
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
 */

#ifndef FS_SPIFFS_FS_H
#define FS_SPIFFS_FS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "spiffs.h"
#include "spiffs_config.h"
#include "vfs.h"
#include "mtd.h"
#include "mutex.h"

/** Size of the buffer needed for directory */
#define SPIFFS_DIR_SIZE (12)

#if (VFS_DIR_BUFFER_SIZE < SPIFFS_DIR_SIZE)
#error "VFS_DIR_BUFFER_SIZE too small"
#endif

/**
 * @name SPIFFS config constants
 * @{
 */
#ifndef SPIFFS_FS_CACHE_SIZE
#if SPIFFS_CACHE || defined(DOXYGEN)
/**
 * @brief the size of the cache buffer
 *
 * Ignored if cache is disabled in build config. One cache page will be slightly
 * larger than the logical page size. The more ram, the more cache pages,
 * the quicker the system.
 */
#define SPIFFS_FS_CACHE_SIZE (512)
#else
#define SPIFFS_FS_CACHE_SIZE (0)
#endif /* SPIFFS_CACHE */
#endif /* SPIFFS_FS_CACHE_SIZE */
#ifndef SPIFFS_FS_WORK_SIZE
/**
 * @brief The size of the work buffer
 *
 * A ram memory buffer being double the size of the logical page size
 * This buffer is used extensively by the spiffs stack.
 * If logical page size is 256, this buffer must be 512 bytes.
 */
#define SPIFFS_FS_WORK_SIZE  (512)
#endif
#ifndef SPIFFS_FS_FD_SPACE_SIZE
/**
 * @brief the size of the file descriptor buffer
 *
 * A file descriptor normally is around 32 bytes depending on the build config -
 * the bigger the buffer, the more file descriptors are available.
 */
#define SPIFFS_FS_FD_SPACE_SIZE (4 * 32)
#endif
/** @} */

/**
 * This contains everything needed to run an instance of SPIFFS
 */
typedef struct spiffs_desc {
    spiffs fs;                                  /**< The SPIFFS struct */
    uint8_t work[SPIFFS_FS_WORK_SIZE];          /**< SPIFFS work buffer */
    uint8_t fd_space[SPIFFS_FS_FD_SPACE_SIZE];  /**< SPIFFS file descriptor cache */
#if (SPIFFS_CACHE == 1) || defined(DOXYGEN)
    uint8_t cache[SPIFFS_FS_CACHE_SIZE];        /**< SPIFFS cache */
#endif
    spiffs_config config;                       /**< SPIFFS config, filled at mount time depending
                                                 *  on the underlying mtdi_dev_t @p dev */
    mutex_t lock;                               /**< A lock for SPIFFS internal use */
#if (SPIFFS_HAL_CALLBACK_EXTRA == 1) || defined(DOXYGEN)
    mtd_dev_t *dev;                             /**< The underlying mtd device, must be set by user */
#endif
} spiffs_desc_t;

/** The SPIFFS vfs driver, a pointer to a spiffs_desc_t must be provided as vfs_mountp::private_data */
extern const vfs_file_system_t spiffs_file_system;

#ifdef __cplusplus
}
#endif

#endif /* FS_SPIFFS_FS_H */

/** @} */
