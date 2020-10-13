#include <expat.h>
#include "utils.h"

/*
Coverage 0: call to XML_ParserCreate_MM from __ipc_specialized_bus_config_load
Coverage 1: call to XML_SetHashSalt from __ipc_specialized_bus_config_load
Coverage 2: call to XML_SetUserData from __ipc_specialized_bus_config_load
Coverage 3: call to XML_SetElementHandler from __ipc_specialized_bus_config_load
Coverage 4: call to XML_SetCharacterDataHandler from __ipc_specialized_bus_config_load
Coverage 5: call to XML_Parse from __ipc_specialized_bus_config_load
Coverage 6: call to XML_GetErrorCode from __ipc_specialized_bus_config_load
Coverage 7: call to XML_GetCurrentLineNumber from __ipc_specialized_bus_config_load
Coverage 8: call to XML_GetCurrentColumnNumber from __ipc_specialized_bus_config_load
Coverage 9: call to XML_ErrorString from __ipc_specialized_bus_config_load
Coverage 10: call to XML_ParserFree from __ipc_specialized_bus_config_load
Coverage 11: call to XML_ParserFree from __ipc_specialized_bus_config_load
 */

// #define XML_Char char;

void instrument_before_XML_ParserCreate_MM(const XML_Char *encoding,
											 const XML_Memory_Handling_Suite *ms,
											 const XML_Char *sep) {
	debug_printf("XML_ParserCreate_MM(%p, %p, %p) ... => %p %p %p", encoding, ms, sep, ms->malloc_fcn, ms->realloc_fcn, ms->free_fcn);
}

void instrument_before_XML_SetHashSalt(XML_Parser p, long l) {
	debug_printf("XML_SetHashSalt(%p, %ld)", p, l);
}

void instrument_before_XML_SetUserData(XML_Parser parser, void *userData) {
	debug_printf("XML_SetUserData(%p, %p)", parser, userData);
}

void instrument_before_XML_SetElementHandler(XML_Parser parser,
											 XML_StartElementHandler start,
											 XML_EndElementHandler end) {
	debug_printf("XML_SetElementHandler(%p, %p, %p)", parser, start, end);
}

void instrument_before_XML_SetCharacterDataHandler(XML_Parser parser,
												   XML_CharacterDataHandler handler) {
	debug_printf("XML_SetCharacterDataHandler(%p, %p)", parser, handler);
}

void instrument_before_XML_Parse(XML_Parser parser, const char *s, int len, int isFinal) {
	debug_printf("XML_Parse(%p, %p, %d, %d)", parser, s, len, isFinal);
}

void instrument_after_XML_Parse(enum XML_Status status) {
	switch (status) {
		case XML_STATUS_OK: debug_printf(" => OK"); break;
		case XML_STATUS_ERROR: debug_printf(" => ERROR"); break;
		case XML_STATUS_SUSPENDED: debug_printf(" => SUSPENDED"); break;
		default:
			debug_printf(" => %d", status);
	}
}

void instrument_before_XML_GetErrorCode(XML_Parser parser) {
	debug_printf("XML_GetErrorCode(%p)", parser);
}

void instrument_after_XML_GetErrorCode(int result) {
	debug_printf(" => %d", result);
}

void instrument_before_XML_GetCurrentLineNumber() {
	debug_printf("XML_GetCurrentLineNumber(...)");
}

void instrument_before_XML_GetCurrentColumnNumber() {
	debug_printf("XML_GetCurrentColumnNumber(...)");
}

void instrument_before_XML_ErrorString() {
	debug_printf("XML_ErrorString(...)");
}

void instrument_before_XML_ParserFree(XML_Parser p) {
	debug_printf("XML_ParserFree(%p)", p);
}

