// Copyright 2020 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <limits>
#include <memory>

#include "Common/CommonTypes.h"
#include "DiscIO/VolumeWii.h"

namespace DiscIO
{
class BlobReader;

class WiiEncryptionCache
{
public:
  using Key = std::array<u8, VolumeWii::AES_KEY_SIZE>;

  // The blob pointer is kept around for the lifetime of this object.
  explicit WiiEncryptionCache(BlobReader* blob);
  ~WiiEncryptionCache();

  // Encrypts exactly one group.
  // If the returned pointer is nullptr, reading from the blob failed.
  // If the returned pointer is not nullptr, it is guaranteed to be valid until
  // the next call of this function or the destruction of this object.
  const std::array<u8, VolumeWii::GROUP_TOTAL_SIZE>* EncryptGroup(u64 offset,
                                                                  u64 partition_data_offset,
                                                                  u64 partition_data_decrypted_size,
                                                                  const Key& key);

  // Encrypts a variable number of groups, as determined by the offset and size parameters.
  // Supports reading groups partially.
  bool EncryptGroups(u64 offset, u64 size, u8* out_ptr, u64 partition_data_offset,
                     u64 partition_data_decrypted_size, const Key& key);

private:
  BlobReader* m_blob;
  std::unique_ptr<std::array<u8, VolumeWii::GROUP_TOTAL_SIZE>> m_cache;
  u64 m_cached_offset = std::numeric_limits<u64>::max();
};

}  // namespace DiscIO
