#include <cstdio>
#include <cassert>
#include <map>

#include "selection_system.h"
#include "particle.h"
#include "boundary.h"
#include "renderer.h"

static selectable_t selectedEntry;
static sf::Color cachedEntryColor;
static std::map<selection_t, const char*> selectionNameMap = {
	{NONE, "None"},
	{PARTICLE, "Particle"},
	{BOUNDARY, "Boundary"},
};

static void SelectionHighlightEntry();
static void SelectionDeHighlightEntry();

void SelectionSelectEntry(selection_t type, void* entryData)
{
	if (!entryData || (type == NONE)) {
		printf("[ERROR][Selection]: Trying to select null data.\n");
		assert(entryData);
		return;
	}

	if ((selectedEntry.type == type) && (selectedEntry.data == entryData)) {
		return;
	} else {
		SelectionDeselectEntry();
	}

	printf("[INFO][Selection]: %s selected.\n", selectionNameMap[type]);

	selectedEntry.type = type;
	selectedEntry.data = entryData;
	SelectionHighlightEntry();
}

selectable_t SelectionDeselectEntry()
{
	printf("[INFO][Selection]: %s deselected.\n", selectionNameMap[selectedEntry.type]);
	SelectionDeHighlightEntry();
	selectable_t entry = selectedEntry;
	selectedEntry.type = NONE;
	selectedEntry.data = nullptr;
	return entry;
}

const selectable_t& SelectionGetSelectedEntry()
{
	return selectedEntry;
}

void SelectionHighlightEntry()
{
	switch (selectedEntry.type) {
		case PARTICLE: {
			particle_t* p = (particle_t*) selectedEntry.data;
			cachedEntryColor = p->originCircle.getFillColor();
			p->originCircle.setFillColor(defaultColPallet.selection);
		}
			break;
		case BOUNDARY: {
			boundary_t* b = (boundary_t*) selectedEntry.data;
			cachedEntryColor = b->color;
			b->color = defaultColPallet.selection;
		}
			break;
		default: break;
	}
}

void SelectionDeHighlightEntry()
{
	switch (selectedEntry.type) {
		case PARTICLE: {
			particle_t* p = (particle_t*) selectedEntry.data;
			if (p->originCircle.getFillColor() != defaultColPallet.selection) {
				return;
			}

			p->originCircle.setFillColor(cachedEntryColor);
		}
			break;
		case BOUNDARY: {
			boundary_t* b = (boundary_t*) selectedEntry.data;
			if (b->color != defaultColPallet.selection) {
				return;
			}

			b->color = cachedEntryColor;
		}
			break;
		default: break;
	}
}