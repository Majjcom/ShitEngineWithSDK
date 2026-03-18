#pragma once

#include "../../mayStlDefine.h"

NS_MAY_BEGIN

/**
 * 计算单个块的大小.<br>
 * 你可以这样使用这个函数:
 * @code
 * float w_items = calcuate_item_width(2, ImGui::CalcItemWidth(), style.ItemInnerSpacing.x);
 * @endcode
 *
 * @param item_count 组件个数
 * @param max_width 最大宽度
 * @param inner_spacing 组件间隔
 *
 * @return 每个组件的宽度
 */
MAY_DLL float calcuate_item_width(int item_count, float max_width, float inner_spacing);

NS_MAY_END
