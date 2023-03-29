#pragma once

enum selection_t {
	NONE = 0,
	PARTICLE,
	BOUNDARY,
};

struct selectable_t {
	selection_t type;
	void* data;
};

void SelectionSelectEntry(selection_t type, void* entryData);
selectable_t SelectionDeselectEntry();
const selectable_t& SelectionGetSelectedEntry();