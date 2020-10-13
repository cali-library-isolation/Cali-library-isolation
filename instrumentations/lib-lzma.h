#ifndef CALI_LIB_LZMA_H
#define CALI_LIB_LZMA_H

#include <lzma.h>

void instrument_before_lzma_check_is_supported(lzma_check check) {
	if (LZMA_VERBOSE) debug_printf("lzma_check_is_supported(%d)", check);
}

void instrument_before_lzma_lzma_preset(lzma_options_lzma *options, uint32_t preset) {
	if (LZMA_VERBOSE | check_ptr(options)) debug_printf("lzma_lzma_preset(%p, %d)   {%p, %p, %p}", options, preset, options->preset_dict, options->reserved_ptr1, options->reserved_ptr2);
}

void instrument_after_lzma_lzma_preset(lzma_bool result) {
	if (LZMA_VERBOSE) debug_printf(" => %d", result);
}

void instrument_before_lzma_stream_encoder_mt_memusage() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_encoder_mt_memusage(...)");
}

void instrument_before_lzma_raw_encoder_memusage(const lzma_filter *filters) {
	if (LZMA_VERBOSE | check_ptr(filters) | check_ptr(filters->options)) debug_printf("lzma_raw_encoder_memusage(%p)  {%p}", filters, filters ? filters->options : 0);
}

void instrument_before_lzma_raw_decoder_memusage() {
	if (LZMA_VERBOSE) debug_printf("lzma_raw_decoder_memusage(...)");
}

void instrument_before_lzma_stream_encoder_mt() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_encoder_mt(...)");
}

void instrument_before_lzma_stream_encoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_encoder(...)");
}

void instrument_before_lzma_alone_encoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_alone_encoder(...)");
}

void instrument_before_lzma_raw_encoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_raw_encoder(...)");
}

void instrument_before_lzma_stream_decoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_decoder(...)");
}

void instrument_before_lzma_alone_decoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_alone_decoder(...)");
}

void instrument_before_lzma_raw_decoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_raw_decoder(...)");
}

void instrument_before_lzma_code() {
	if (LZMA_VERBOSE) debug_printf("lzma_code(...)");
}

void instrument_before_lzma_memusage() {
	if (LZMA_VERBOSE) debug_printf("lzma_memusage(...)");
}

void instrument_before_lzma_properties_decode() {
	if (LZMA_VERBOSE) debug_printf("lzma_properties_decode(...)");
}

void instrument_before_lzma_cputhreads() {
	if (LZMA_VERBOSE) debug_printf("lzma_cputhreads(...)");
}

void instrument_before_lzma_physmem() {
	if (LZMA_VERBOSE) debug_printf("lzma_physmem(...)");
}

void instrument_before_lzma_stream_footer_decode() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_footer_decode(...)");
}

void instrument_before_lzma_index_memused() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_memused(...)");
}

void instrument_before_lzma_index_decoder() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_decoder(...)");
}

void instrument_before_lzma_index_total_size() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_total_size(...)");
}

void instrument_before_lzma_stream_header_decode() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_header_decode(...)");
}

void instrument_before_lzma_stream_flags_compare() {
	if (LZMA_VERBOSE) debug_printf("lzma_stream_flags_compare(...)");
}

void instrument_before_lzma_index_stream_flags() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_stream_flags(...)");
}

void instrument_before_lzma_index_stream_padding() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_stream_padding(...)");
}

void instrument_before_lzma_index_cat() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_cat(...)");
}

void instrument_before_lzma_end() {
	if (LZMA_VERBOSE) debug_printf("lzma_end(...)");
}

void instrument_before_lzma_index_end() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_end(...)");
}

void instrument_before_lzma_index_checks() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_checks(...)");
}

void instrument_before_lzma_index_stream_count() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_stream_count(...)");
}

void instrument_before_lzma_index_block_count() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_block_count(...)");
}

void instrument_before_lzma_index_file_size() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_file_size(...)");
}

void instrument_before_lzma_index_uncompressed_size() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_uncompressed_size(...)");
}

void instrument_before_lzma_index_iter_init() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_iter_init(...)");
}

void instrument_before_lzma_index_iter_next() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_iter_next(...)");
}

void instrument_before_lzma_index_iter_rewind() {
	if (LZMA_VERBOSE) debug_printf("lzma_index_iter_rewind(...)");
}

void instrument_before_lzma_check_size() {
	if (LZMA_VERBOSE) debug_printf("lzma_check_size(...)");
}

void instrument_before_lzma_block_header_decode() {
	if (LZMA_VERBOSE) debug_printf("lzma_block_header_decode(...)");
}

void instrument_before_lzma_block_compressed_size() {
	if (LZMA_VERBOSE) debug_printf("lzma_block_compressed_size(...)");
}

void instrument_before_lzma_get_progress() {
	if (LZMA_VERBOSE) debug_printf("lzma_get_progress(...)");
}

void instrument_before_lzma_version_number() {
	if (LZMA_VERBOSE) debug_printf("lzma_version_number(...)");
}

void instrument_before_lzma_version_string() {
	if (LZMA_VERBOSE) debug_printf("lzma_version_string(...)");
}

void instrument_before_lzma_filter_flags_size(uint32_t *size, const lzma_filter *filter) {
	if (LZMA_VERBOSE | check_ptr(size) | check_ptr(filter) | check_ptr(filter ? filter->options : 0))
		debug_printf("lzma_filter_flags_size(%p, %p) {%lu, %p}", size, filter, filter->id, filter->options);
}

void instrument_before_lzma_filter_flags_encode(const lzma_filter *filter, uint8_t *out, size_t *out_pos, size_t out_size) {
	if (LZMA_VERBOSE | check_ptr(filter) | check_ptr(out) | check_ptr(out_pos) | check_ptr(filter ? filter->options : 0))
		debug_printf("lzma_filter_flags_encode(%p, %p, %p, %ld)  {%lu, %p}", filter, out, out_pos, out_size, filter->id, filter->options);
}

void instrument_before_lzma_filter_flags_decode(lzma_filter *filter, const lzma_allocator *allocator, const uint8_t *in, size_t *in_pos, size_t in_size) {
	if (LZMA_VERBOSE | check_ptr(filter) | check_ptr(allocator) | check_ptr(in) | check_ptr(in_pos) | check_ptr(filter ? filter->options : 0))
		debug_printf("lzma_filter_flags_decode(%p, %p, %p, %p, %ld)  {%lu, %p}", filter, allocator, in, in_pos, in_size, filter->id, filter->options);
}


#endif //CALI_LIB_LZMA_H
