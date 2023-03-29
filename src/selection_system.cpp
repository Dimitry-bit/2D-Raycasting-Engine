#include <cstdio>
#include <cassert>
#include <string>
#include <map>

#include "selection_system.h"

static selectable_t selectedEntry;
static std::map<selection_t, const char*> selectionNameMap = {
	{NONE, "None"},
	{PARTICLE, "Particle"},
	{BOUNDARY, "Boundary"},
};

void SelectionSelectEntry(selection_t type, void* entryData)
{
	if (!entryData || (type == NONE)) {
		printf("[ERROR][Selection]: Trying to select null data.\n");
		assert(entryData);
		return;
	}

	selectedEntry.type = type;
	selectedEntry.data = entryData;
	printf("[INFO][Selection]: %s selected.\n", selectionNameMap[type]);
}

selectable_t SelectionDeselectEntry()
{
	printf("[INFO][Selection]: %s deselected.\n", selectionNameMap[selectedEntry.type]);
	selectable_t entry = selectedEntry;
	selectedEntry.type = NONE;
	selectedEntry.data = nullptr;
	return entry;
}

const selectable_t& SelectionGetSelectedEntry()
{
	return selectedEntry;
}