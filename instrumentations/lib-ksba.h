#ifndef CALI_LIB_KSBA_H
#define CALI_LIB_KSBA_H

#include <ksba.h>
#include "utils.h"


enum reader_type {
	READER_TYPE_NONE = 0,
	READER_TYPE_MEM,
	READER_TYPE_FD,
	READER_TYPE_FILE,
	READER_TYPE_CB
};


struct ksba_reader_s {
	int eof;
	int error;   /* If an error occured, takes the value of errno. */
	unsigned long nread;
	struct {
		unsigned char *buf;
		size_t size;    /* allocated size */
		size_t length;  /* used size */
		size_t readpos; /* offset where to start the next read */
	} unread;
	enum reader_type type;
	union {
		struct {
			unsigned char *buffer;
			size_t size;
			size_t readpos;
		} mem;   /* for READER_TYPE_MEM */
		int fd;  /* for READER_TYPE_FD */
		FILE *file; /* for READER_TYPE_FILE */
		struct {
			int (*fnc)(void *, char *, size_t, size_t *);

			void *value;
		} cb;   /* for READER_TYPE_CB */
	} u;

	void (*notify_cb)(void *, ksba_reader_t);

	void *notify_cb_value;
};


void instrument_before_ksba_check_version(const char *req_version) {
	if (KSBA_VERBOSE | check_ptr(req_version)) debug_printf("ksba_check_version(%p)", req_version);
}

void instrument_before_ksba_set_malloc_hooks(void *(*new_alloc_func)(size_t n),
											 void *(*new_realloc_func)(void *p, size_t n),
											 void (*new_free_func)(void *)) {
	if (KSBA_VERBOSE) {
		debug_printf("ksba_set_malloc_hooks(%p, %p, %p)", new_alloc_func, new_realloc_func, new_free_func);
		//debug_printf("   malloc=%p realloc=%p free=%p", &malloc, &realloc, &free);
	}
}

void instrument_before_ksba_cert_release(ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_release(%p)", cert);
}

void instrument_before_ksba_reader_new(ksba_reader_t *r_r) {
	if (KSBA_VERBOSE | check_ptr(r_r)) debug_printf("ksba_reader_new(%p)", r_r);
}

static ksba_reader_t last_reader;

void instrument_before_ksba_reader_set_cb(ksba_reader_t r, int (*cb)(void *, char *, size_t, size_t *), void *cb_value) {
	if (KSBA_VERBOSE | check_ptr(r)) {
		debug_printf("ksba_reader_set_cb(%p, %p, %p)", r, cb, cb_value);
		debug_printf("    type=%d union=(%p, 0x%lx, 0x%lx)/(%p, %p)", r->type, r->u.mem.buffer, r->u.mem.size, r->u.mem.readpos, r->u.cb.fnc, r->u.cb.value);
	}
	last_reader = r;
}

void instrument_after_ksba_reader_set_cb() {
	if (KSBA_VERBOSE) {
		debug_printf(" => type=%d union=(%p, 0x%lx, 0x%lx)/(%p, %p)",
					 last_reader->type, last_reader->u.mem.buffer, last_reader->u.mem.size, last_reader->u.mem.readpos, last_reader->u.cb.fnc,
					 last_reader->u.cb.value);
	}
}

void instrument_before_ksba_cms_release(ksba_cms_t cms) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_release(%p)", cms);
}

void instrument_before_ksba_cms_identify(ksba_reader_t reader) {
	if (KSBA_VERBOSE | check_ptr(reader)) {
		debug_printf("ksba_cms_identify(%p)", reader);
		//debug_printf("    type=%d union=(%p, 0x%lx, 0x%lx)/(%p, %p)", reader->type, reader->u.mem.buffer, reader->u.mem.size, reader->u.mem.readpos, reader->u.cb.fnc, reader->u.cb.value);
	}
}

void instrument_before_ksba_cert_get_auth_key_id(ksba_cert_t cert, ksba_sexp_t *r_keyid, ksba_name_t *r_name, ksba_sexp_t *r_serial) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_keyid) | check_ptr(r_name) | check_ptr(r_serial))
		debug_printf("ksba_cert_get_auth_key_id(%p, %p, %p, %p)", cert, r_keyid, r_name, r_serial);
}

void instrument_before_ksba_cert_get_authority_info_access() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_authority_info_access(...)");
}

void instrument_before_ksba_cert_get_cert_policies(ksba_cert_t cert, char **r_policies) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_policies)) debug_printf("ksba_cert_get_cert_policies(%p, %p)", cert, r_policies);
}

void instrument_before_ksba_cert_get_crl_dist_point() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_crl_dist_point(...)");
}

void instrument_before_ksba_cert_get_digest_algo() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_digest_algo(...)");
}

void instrument_before_ksba_cert_get_ext_key_usages(ksba_cert_t cert, char **result) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(result)) debug_printf("ksba_cert_get_ext_key_usages(%p, %p)", cert, result);
}

void instrument_before_ksba_cert_get_extension(ksba_cert_t cert, int idx,
											   char const **r_oid, int *r_crit,
											   size_t *r_deroff, size_t *r_derlen) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_oid) | check_ptr(r_crit) | check_ptr(r_deroff) | check_ptr(r_derlen))
		debug_printf("ksba_cert_get_extension(%p, %d, %p, %p, %p, %p)", cert, idx, r_oid, r_crit, r_deroff, r_derlen);
}

void instrument_before_ksba_cert_get_image(ksba_cert_t cert, size_t *r_length) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_length)) debug_printf("ksba_cert_get_image(%p, %p)", cert, r_length);
}

void instrument_before_ksba_cert_get_issuer(ksba_cert_t cert, int idx) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_get_issuer(%p, %d)", cert, idx);
}

void instrument_after_ksba_cert_get_issuer(char *result) {
	if (KSBA_VERBOSE | check_ptr(result))
		debug_printf(" => %p  \"%s\"", result, result ? result : "-");
}

void instrument_before_ksba_cert_get_key_usage(ksba_cert_t cert, unsigned int *r_flags) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_flags)) debug_printf("ksba_cert_get_key_usage(%p, %p)", cert, r_flags);
}

void instrument_before_ksba_cert_get_public_key(ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_get_public_key(%p)", cert);
}

void instrument_before_ksba_cert_get_serial(ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_get_serial(%p)", cert);
}

void instrument_before_ksba_cert_get_sig_val() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_sig_val(...)");
}

void instrument_before_ksba_cert_get_subj_key_id() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_subj_key_id(...)");
}

void instrument_before_ksba_cert_get_subject(ksba_cert_t cert, int idx) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_get_subject(%p, %d)", cert, idx);
}

void instrument_after_ksba_cert_get_subject(char *result) {
	if (KSBA_VERBOSE | check_ptr(result))
		debug_printf(" => %p  \"%s\"", result, result ? result : "-");
}

void instrument_before_ksba_cert_get_subject_info_access() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_get_subject_info_access(...)");
}

void instrument_before_ksba_cert_get_user_data(ksba_cert_t cert, const char *key,
											   void *buffer, size_t bufferlen,
											   size_t *datalen) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(key) | check_ptr(buffer) | check_ptr(datalen))
		debug_printf("ksba_cert_get_user_data(%p, %p, %p, %ld, %p)", cert, key, buffer, bufferlen, datalen);
}

void instrument_before_ksba_cert_get_validity(ksba_cert_t cert, int what,
											  ksba_isotime_t r_time) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_time)) debug_printf("ksba_cert_get_validity(%p, %d, %p)", cert, what, r_time);
}

void instrument_before_ksba_cert_hash(ksba_cert_t cert, int what, void (*hasher)(void *, const void *, size_t length), void *hasher_arg) {
	if (KSBA_VERBOSE | check_ptr(cert))
		debug_printf("ksba_cert_hash(%p, %d, %p, %p)", cert, what, hasher, hasher_arg);
}

void instrument_before_ksba_cert_init_from_mem() {
	if (KSBA_VERBOSE) debug_printf("ksba_cert_init_from_mem(...)");
}

void instrument_before_ksba_cert_is_ca(ksba_cert_t cert, int *r_ca, int *r_pathlen) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(r_ca) | check_ptr(r_pathlen)) debug_printf("ksba_cert_is_ca(%p, %p, %p)", cert, r_ca, r_pathlen);
}

void instrument_before_ksba_cert_new(ksba_cert_t *acert) {
	if (KSBA_VERBOSE | check_ptr(acert)) debug_printf("ksba_cert_new(%p)", acert);
}

void instrument_after_ksba_cert_new(gpg_error_t result) {
	if (KSBA_VERBOSE) debug_printf(" => %u", result);
}

void instrument_before_ksba_cert_read_der(ksba_cert_t cert, ksba_reader_t reader) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(reader)) debug_printf("ksba_cert_read_der(%p, %p)", cert, reader);
}

void instrument_after_ksba_cert_read_der(gpg_error_t result) {
	if (KSBA_VERBOSE) debug_printf(" => %u", result);
}

void instrument_before_ksba_cert_ref(ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cert)) debug_printf("ksba_cert_ref(%p)", cert);
}

void instrument_before_ksba_cert_set_user_data(ksba_cert_t cert, const char *key,
											   const void *data, size_t datalen) {
	if (KSBA_VERBOSE | check_ptr(cert) | check_ptr(key) | check_ptr(data))
		debug_printf("ksba_cert_set_user_data(%p, %p, %p, %ld)", cert, key, data, datalen);
}

void instrument_before_ksba_certreq_add_extension() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_add_extension(...)");
}

void instrument_before_ksba_certreq_add_subject() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_add_subject(...)");
}

void instrument_before_ksba_certreq_build() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_build(...)");
}

void instrument_before_ksba_certreq_new() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_new(...)");
}

void instrument_before_ksba_certreq_release() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_release(...)");
}

void instrument_before_ksba_certreq_set_hash_function() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_hash_function(...)");
}

void instrument_before_ksba_certreq_set_issuer() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_issuer(...)");
}

void instrument_before_ksba_certreq_set_public_key() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_public_key(...)");
}

void instrument_before_ksba_certreq_set_serial() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_serial(...)");
}

void instrument_before_ksba_certreq_set_sig_val() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_sig_val(...)");
}

void instrument_before_ksba_certreq_set_siginfo() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_siginfo(...)");
}

void instrument_before_ksba_certreq_set_validity() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_validity(...)");
}

void instrument_before_ksba_certreq_set_writer() {
	if (KSBA_VERBOSE) debug_printf("ksba_certreq_set_writer(...)");
}

void instrument_before_ksba_cms_add_cert(ksba_cms_t cms, ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(cert)) debug_printf("ksba_cms_add_cert(%p, %p)", cms, cert);
}

void instrument_before_ksba_cms_add_digest_algo(ksba_cms_t cms, const char *oid) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(oid)) debug_printf("ksba_cms_add_digest_algo(%p, %p)", cms, oid);
}

void instrument_before_ksba_cms_add_recipient(ksba_cms_t cms, ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(cert)) debug_printf("ksba_cms_add_recipient(%p, %p)", cms, cert);
}

void instrument_before_ksba_cms_add_signer(ksba_cms_t cms, ksba_cert_t cert) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(cert)) debug_printf("ksba_cms_add_signer(%p, %p)", cms, cert);
}

void instrument_before_ksba_cms_add_smime_capability(ksba_cms_t cms, const char *oid, const unsigned char *der, size_t derlen) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(oid) | check_ptr(der))
		debug_printf("ksba_cms_add_smime_capability(%p, %p, %p, %ld)", cms, oid, der, derlen);
}

void instrument_before_ksba_cms_build(ksba_cms_t cms, ksba_stop_reason_t *r_stopreason) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r_stopreason)) debug_printf("ksba_cms_build(%p, %p)", cms, r_stopreason);
}

void instrument_before_ksba_cms_get_cert(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_cert(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_get_content_enc_iv(ksba_cms_t cms, void *iv, size_t maxivlen, size_t *ivlen) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(iv) | check_ptr(ivlen))
		debug_printf("ksba_cms_get_content_enc_iv(%p, %p, %ld, %p)", cms, iv, maxivlen, ivlen);
}

void instrument_before_ksba_cms_get_content_oid(ksba_cms_t cms, int what) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_content_oid(%p, %d)", cms, what);
}

void instrument_before_ksba_cms_get_digest_algo(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_digest_algo(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_get_digest_algo_list(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_digest_algo_list(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_get_enc_val(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_enc_val(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_get_issuer_serial(ksba_cms_t cms, int idx, char **r_issuer, ksba_sexp_t *r_serial) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r_issuer) | check_ptr(r_serial))
		debug_printf("ksba_cms_get_issuer_serial(%p, %d, %p, %p)", cms, idx, r_issuer, r_serial);
}

static char** last_r_digest;
void instrument_before_ksba_cms_get_message_digest(ksba_cms_t cms, int idx, char **r_digest, size_t *r_digest_len) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r_digest) | check_ptr(r_digest_len))
		debug_printf("ksba_cms_get_message_digest(%p, %d, %p, %p)", cms, idx, r_digest, r_digest_len);
	last_r_digest = r_digest;
}

void instrument_after_ksba_cms_get_message_digest() {
	if (KSBA_VERBOSE) {
		debug_printf(" => result@%p = %p", last_r_digest, last_r_digest ? *last_r_digest : 0);
		if (last_r_digest && *last_r_digest) {
			debug_printf("%p: %02hhx %02hhx %02hhx %02hhx", last_r_digest[0], last_r_digest[0][0], last_r_digest[0][1], last_r_digest[0][2], last_r_digest[0][3]);
		}
	}
}

void instrument_before_ksba_cms_get_sig_val(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_get_sig_val(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_get_sigattr_oids(ksba_cms_t cms, int idx, const char *reqoid, char **r_value) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(reqoid) | check_ptr(r_value))
		debug_printf("ksba_cms_get_sigattr_oids(%p, %d, %p, %p)", cms, idx, reqoid, r_value);
}

void instrument_before_ksba_cms_get_signing_time(ksba_cms_t cms, int idx, ksba_isotime_t r_sigtime) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r_sigtime)) debug_printf("ksba_cms_get_signing_time(%p, %d, %p)", cms, idx, r_sigtime);
}

void instrument_before_ksba_cms_hash_signed_attrs(ksba_cms_t cms, int idx) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_hash_signed_attrs(%p, %d)", cms, idx);
}

void instrument_before_ksba_cms_new(ksba_cms_t *r_cms) {
	if (KSBA_VERBOSE | check_ptr(r_cms)) debug_printf("ksba_cms_new(%p)", r_cms);
}

void instrument_before_ksba_cms_parse(ksba_cms_t cms, ksba_stop_reason_t *r_stopreason) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r_stopreason)) debug_printf("ksba_cms_parse(%p, %p)", cms, r_stopreason);
}

void instrument_before_ksba_cms_set_content_enc_algo(ksba_cms_t cms,
													 const char *oid,
													 const void *iv,
													 size_t ivlen) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(oid) | check_ptr(iv))
		debug_printf("ksba_cms_set_content_enc_algo(%p, %p, %p, %ld)", cms, oid, iv, ivlen);
}

void instrument_before_ksba_cms_set_content_type(ksba_cms_t cms, int what, ksba_content_type_t type) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_set_content_type(%p, %d, %d)", cms, what, type);
}

void instrument_before_ksba_cms_set_enc_val(ksba_cms_t cms, int idx, ksba_const_sexp_t encval) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(encval)) debug_printf("ksba_cms_set_enc_val(%p, %d, %p)", cms, idx, encval);
}

void instrument_before_ksba_cms_set_hash_function(ksba_cms_t cms, void (*hash_fnc)(void *, const void *, size_t), void *hash_fnc_arg) {
	if (KSBA_VERBOSE | check_ptr(cms)) debug_printf("ksba_cms_set_hash_function(%p, %p, %p)", cms, hash_fnc, hash_fnc_arg);
}

static const unsigned char *last_digest;
void instrument_before_ksba_cms_set_message_digest(ksba_cms_t cms, int idx,
												   const unsigned char *digest,
												   size_t digest_len) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(digest)) {
		debug_printf("ksba_cms_set_message_digest(%p, %d, %p, %ld)", cms, idx, digest, digest_len);
	}
	last_digest = digest;
}

void instrument_after_ksba_cms_set_message_digest() {
	if (KSBA_VERBOSE) {
		debug_printf(" => %p = %02hhx %02hhx %02hhx %02hhx", last_digest, last_digest[0], last_digest[1], last_digest[2], last_digest[3]);
	}
}

void instrument_before_ksba_cms_set_reader_writer(ksba_cms_t cms, ksba_reader_t r, ksba_writer_t w) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(r) | check_ptr(w)) debug_printf("ksba_cms_set_reader_writer(%p, %p, %p)", cms, r, w);
}

void instrument_before_ksba_cms_set_sig_val(ksba_cms_t cms, int idx, ksba_const_sexp_t sigval) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(sigval)) debug_printf("ksba_cms_set_sig_val(%p, %d, %p)", cms, idx, sigval);
}

void instrument_before_ksba_cms_set_signing_time(ksba_cms_t cms, int idx, const ksba_isotime_t sigtime) {
	if (KSBA_VERBOSE | check_ptr(cms) | check_ptr(sigtime)) debug_printf("ksba_cms_set_signing_time(%p, %d, %p)", cms, idx, sigtime);
}

void instrument_before_ksba_dn_teststr() {
	if (KSBA_VERBOSE) debug_printf("ksba_dn_teststr(...)");
}

void instrument_before_ksba_free(void *a) {
	if (KSBA_VERBOSE | check_ptr(a)) debug_printf("ksba_free(%p)", a);
}

void instrument_before_ksba_name_enum(ksba_name_t name, int idx) {
	if (KSBA_VERBOSE | check_ptr(name)) debug_printf("ksba_name_enum(%p, %d)", name, idx);
}

void instrument_before_ksba_name_get_uri() {
	if (KSBA_VERBOSE) debug_printf("ksba_name_get_uri(...)");
}

void instrument_before_ksba_name_release(ksba_name_t name) {
	if (KSBA_VERBOSE | check_ptr(name)) debug_printf("ksba_name_release(%p)", name);
}

void instrument_before_ksba_oid_to_str() {
	if (KSBA_VERBOSE) debug_printf("ksba_oid_to_str(...)");
}

void instrument_before_ksba_reader_clear(ksba_reader_t r, unsigned char **buffer, size_t *buflen) {
	if (KSBA_VERBOSE | check_ptr(r) | check_ptr(buffer) | check_ptr(buflen))
		debug_printf("ksba_reader_clear(%p, %p, %p)", r, buffer, buflen);
}

void instrument_before_ksba_reader_read() {
	if (KSBA_VERBOSE) debug_printf("ksba_reader_read(...)");
}

void instrument_before_ksba_reader_release(ksba_reader_t r) {
	if (KSBA_VERBOSE | check_ptr(r)) debug_printf("ksba_reader_release(%p)", r);
}

void instrument_before_ksba_reader_set_mem(ksba_reader_t r, const void *buffer, size_t length) {
	if (KSBA_VERBOSE | check_ptr(r) | check_ptr(buffer)) debug_printf("ksba_reader_set_mem(%p, %p, %ld)", r, buffer, length);
}

void instrument_before_ksba_writer_new(ksba_writer_t *r_w) {
	if (KSBA_VERBOSE | check_ptr(r_w)) debug_printf("ksba_writer_new(%p)", r_w);
}

void instrument_before_ksba_writer_release(ksba_writer_t w) {
	if (KSBA_VERBOSE | check_ptr(w)) debug_printf("ksba_writer_release(%p)", w);
}

void instrument_before_ksba_writer_set_cb(ksba_writer_t w,
										  int (*cb)(void *, const void *, size_t),
										  void *cb_value) {
	if (KSBA_VERBOSE | check_ptr(w) | check_ptr(cb_value)) debug_printf("ksba_writer_set_cb(%p, %p, %p)", w, cb, cb_value);
}

void instrument_before_ksba_writer_set_filter(ksba_writer_t w, gpg_error_t (*filter)(void *, const void *, size_t, size_t *, void *, size_t, size_t *),
											  void *filter_arg) {
	if (KSBA_VERBOSE | check_ptr(w) | check_ptr(filter_arg)) debug_printf("ksba_writer_set_filter(%p, %p, %p)", w, filter, filter_arg);
}

void instrument_before_ksba_writer_write() {
	if (KSBA_VERBOSE) debug_printf("ksba_writer_write(...)");
}

void instrument_before_ksba_writer_write_octet_string(ksba_writer_t w, const void *buffer, size_t length, int flush) {
	if (KSBA_VERBOSE | check_ptr(w) | check_ptr(buffer))
		debug_printf("ksba_writer_write_octet_string(%p, %p, %ld, %d)", w, buffer, length, flush);
}


#endif //CALI_LIB_KSBA_H
