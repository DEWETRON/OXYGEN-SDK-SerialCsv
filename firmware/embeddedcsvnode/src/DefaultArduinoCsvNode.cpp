#include "CsvNode.h"

/*
 * Only put the Default-Csv-Node Instance in this .cpp file, since the linker pulls
 * in the entire file when any element inside is used. --gc-sections can
 * additionally cause unused symbols to be dropped. Putting each instance in its own
 * file prevents the linker from pulling in any unused instances in the
 * first place - Leading to zero-size library when user is customizing the CsvNode
 */


// ****************************************************************************
// Ensure dot_a_linkage is true in library.properties to enable this behaviour
// ****************************************************************************

#ifdef ARDUINO
csvnode::DefaultArduinoCsvNode CsvNode;
#endif
