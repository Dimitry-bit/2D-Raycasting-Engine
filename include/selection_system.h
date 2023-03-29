#pragma once

#include "engine_primitives.h"

struct selectable_t {
	primitive_t type;
	void* data;
};

void SelectionSelectEntry(primitive_t type, void* entryData);
selectable_t SelectionDeselectEntry();
const selectable_t& SelectionGetSelectedEntry();