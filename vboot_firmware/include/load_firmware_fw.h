/* Copyright (c) 2010 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * High-level firmware API for loading and verifying rewritable firmware.
 * (Firmware Portion)
 */

#ifndef VBOOT_REFERENCE_LOAD_FIRMWARE_FW_H_
#define VBOOT_REFERENCE_LOAD_FIRMWARE_FW_H_

#include <stdint.h>

/* Functions provided by PEI to LoadFirmware() */

/* Get the firmware data for [firmware_index], which is either
 * 0 (the first firmware image) or 1 (the second firmware image).
 *
 * This function must call LoadFirmwareUpdateDataHash() before
 * returning, to update the secure hash for the firmware image.  For
 * best performance, the reader should call this function periodically
 * during the read, so that updating the hash can be pipelined with
 * the read.  If the reader cannot update the hash during the read
 * process, it should call LoadFirmwareUpdateDataHash() on the entire
 * firmeware data after the read, before returning.
 *
 * On success, returns a pointer to the data and stores the data size
 * in [*size].  On error, returns NULL. */
void *FirmwareImageGetData(uint64_t firmware_index, uint64_t* size);


/* Interface provided by verified boot library to PEI */

/* Return codes for LoadFirmware() */
#define LOAD_FIRMWARE_SUCCESS 0
#define LOAD_FIRMWARE_RECOVERY 1

/* Update the data hash for the current firmware image, extending it
 * by [size] bytes stored in [*data].  This function must only be
 * called inside FirmwareImageGetData(). */
void LoadFirmwareUpdateDataHash(uint8_t* data, uint64_t size);


typedef struct LoadFirmwareParams {
  /* Inputs to LoadFirmware() */
  void *header_sign_key_blob;  /* Key used to sign firmware header */
  void *vblock0;              /* Key block + preamble for firmware 0 */
  void *vblock1;              /* Key block + preamble for firmware 1 */

  /* Outputs from LoadFirmware(); valid only if LoadFirmware() returns
   * LOAD_FIRMWARE_SUCCESS. */
  uint64_t fitmware_index;       /* Firmware index to run. */
  void *kernel_sign_key_blob;    /* Key to use when loading kernel.
                                  * Pass this data to LoadKernel() in
                                  * LoadKernelParams.header_sign_key_blob.
                                  * Key data may be copied/relocated
                                  * if necessary. */
  uint64_t kernel_sign_key_size;  /* Size of kernel signing key blob,
                                   * in bytes. */
} LoadFirmwareParams;


/* Attempts to load the rewritable firmware.
 *
 * Returns LOAD_FIRMWARE_SUCCESS if successful, error code on failure. */
int LoadFirmware(LoadFirmwareParams* params);


#endif  /* VBOOT_REFERENCE_LOAD_FIRMWARE_FW_H_ */