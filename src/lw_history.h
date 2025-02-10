#ifndef _DMC_LW_HISTORY_H_
#define _DMC_LW_HISTORY_H_

#include "dmc_table.h"
#include "dmc_utils.h"
#include "nm.h"

#include <stdint.h>

// `lw_history.h`可能实现了轻量级的历史记录管理，用于缓存淘汰策略。
// LWHistory类：管理被淘汰缓存项的历史信息，支持快速判断项是否被覆盖。
// 作用：在自适应策略中，快速过滤无效历史项，避免重复淘汰或无效访问。
// 历史记录：LWHistory记录淘汰项的时间戳，自适应策略通过has_overwritten判断历史有效性，动态调整权重。
class LWHistory {
 private:
  uint32_t hist_size_;
  uint64_t hist_head_raddr_;

  uint32_t occupy_size_;

 public:
  LWHistory(uint32_t hist_size, uint64_t hist_base_raddr, uint8_t type) {
    hist_size_ = hist_size;
    hist_head_raddr_ = hist_base_raddr;
    occupy_size_ = sizeof(uint64_t);
    if (type == SERVER)
      memset((void*)hist_base_raddr, 0, occupy_size_);
  }

  inline uint32_t size() { return occupy_size_; }
  inline uint64_t hist_cntr_raddr() { return hist_head_raddr_; }

  // has_overwritten：通过时间戳掩码（HIST_MASK）判断历史项是否已被新项覆盖。
  inline bool has_overwritten(uint64_t cur_head, uint64_t stored_head) {
    cur_head &= HIST_MASK;
    stored_head &= HIST_MASK;
    if (cur_head >= stored_head)
      return (cur_head - stored_head) >= hist_size_;
    return (cur_head + (1ULL << 48) - stored_head) >= hist_size_;
  }

  // is_in_history：检查槽位是否标记为历史项（kv_len设为特定值）。
  inline bool is_in_history(const Slot* slot) {
    return slot->atomic.kv_len == 0xF;
  }
};

#endif