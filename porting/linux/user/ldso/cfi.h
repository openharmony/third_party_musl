#include "dynlink.h"

/* alignment bits in memory space for dso */
#define LIBRARY_ALIGNMENT_BITS  18
/* minimum unit in memory space for dso */
#define LIBRARY_ALIGNMENT       (1UL << LIBRARY_ALIGNMENT_BITS)

/* map all the dso and the dependents to cfi shadow */
bool init_cfi_shadow(struct dso *dso_list);
/* map a dso and the dependents to cfi shadow */
bool map_dso_to_cfi_shadow(struct dso *dso);
/* unmap a dso from cfi shadow */
void unmap_dso_from_cfi_shadow(struct dso *dso);