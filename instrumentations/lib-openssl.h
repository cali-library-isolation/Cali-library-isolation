#ifndef CALI_LIB_OPENSSL_H
#define CALI_LIB_OPENSSL_H

#include "utils.h"
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

void instrument_before_ASN1_STRING_get0_data(const ASN1_STRING *x) {
	if (SSL_VERBOSE | check_ptr(x)) debug_printf("ASN1_STRING_get0_data(...)");
}

void instrument_before_ASN1_STRING_to_UTF8(unsigned char **out, const ASN1_STRING *in) {
	if (SSL_VERBOSE | check_ptr(out) | check_ptr(in)) debug_printf("ASN1_STRING_to_UTF8(...)");
}

void instrument_before_BIO_ctrl(BIO *bp, int cmd, long larg, void *parg) {
	if (SSL_VERBOSE | check_ptr(bp) | check_ptr(parg)) debug_printf("BIO_ctrl(...)");
}

void instrument_before_BIO_free(BIO *a) {
	if (SSL_VERBOSE | check_ptr(a)) debug_printf("BIO_free(...)");
}

void instrument_before_BIO_new(BIO_METHOD *type) {
	if (SSL_VERBOSE | check_ptr(type)) debug_printf("BIO_new(...)");
}

void instrument_before_BIO_new_file(const char *filename, const char *mode) {
	if (SSL_VERBOSE | check_ptr(filename) | check_ptr(mode)) debug_printf("BIO_new_file(...)");
}

void instrument_before_BIO_s_mem(void) {
	if (SSL_VERBOSE) debug_printf("BIO_s_mem(...)");
}

void instrument_before_BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret) {
	if (SSL_VERBOSE | check_ptr(s) | check_ptr(ret)) debug_printf("BN_bin2bn(...)");
}

void instrument_before_BN_free(BIGNUM *a) {
	if (SSL_VERBOSE | check_ptr(a)) debug_printf("BN_free(...)");
}

void instrument_before_CRYPTO_free(void *str, const char *s, int i) {
	if (SSL_VERBOSE | check_ptr(str) | check_ptr(s)) debug_printf("CRYPTO_free(...)");
}

void instrument_before_DH_free(DH *dh) {
	if (SSL_VERBOSE | check_ptr(dh)) debug_printf("DH_free(...)");
}

void instrument_before_DH_new(void) {
	if (SSL_VERBOSE) debug_printf("DH_new(...)");
}

void instrument_before_DH_set0_pqg(DH *dh, BIGNUM *p, BIGNUM *q, BIGNUM *g) {
	if (SSL_VERBOSE | check_ptr(dh) | check_ptr(p) | check_ptr(q) | check_ptr(g)) debug_printf("DH_set0_pqg(...)");
}

void instrument_before_EC_KEY_new_by_curve_name(int nid) {
	if (SSL_VERBOSE) debug_printf("EC_KEY_new_by_curve_name(...)");
}

void instrument_before_ERR_error_string(unsigned long e, char *buf) {
	if (SSL_VERBOSE | check_ptr(buf)) debug_printf("ERR_error_string(...)");
}

void instrument_before_ERR_error_string_n(unsigned long e, char *buf, size_t len) {
	if (SSL_VERBOSE | check_ptr(buf)) debug_printf("ERR_error_string_n(...)");
}

void instrument_before_ERR_func_error_string(unsigned long e) {
	if (SSL_VERBOSE) debug_printf("ERR_func_error_string(...)");
}

void instrument_before_ERR_get_error(void) {
	if (SSL_VERBOSE) debug_printf("ERR_get_error(...)");
}

void instrument_before_ERR_lib_error_string(unsigned long e) {
	if (SSL_VERBOSE) debug_printf("ERR_lib_error_string(...)");
}

void instrument_before_ERR_peek_error(unsigned long e) {
	if (SSL_VERBOSE) debug_printf("ERR_peek_error(...)");
}

void instrument_before_ERR_reason_error_string(unsigned long e) {
	if (SSL_VERBOSE) debug_printf("ERR_reason_error_string(...)");
}

void instrument_before_OBJ_nid2ln(int n) {
	if (SSL_VERBOSE) debug_printf("OBJ_nid2ln(...)");
}

void instrument_before_OBJ_nid2sn(int n) {
	if (SSL_VERBOSE) debug_printf("OBJ_nid2sn(...)");
}

void instrument_before_OBJ_obj2nid(const ASN1_OBJECT *o) {
	if (SSL_VERBOSE | check_ptr(o)) debug_printf("OBJ_obj2nid(...)");
}

void instrument_before_OPENSSL_init_crypto(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings) {
	if (SSL_VERBOSE | check_ptr(settings)) debug_printf("OPENSSL_init_crypto(...)");
}

void instrument_before_OPENSSL_init_ssl(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings) {
	if (SSL_VERBOSE | check_ptr(settings)) debug_printf("OPENSSL_init_ssl(...)");
}

void instrument_before_OPENSSL_sk_num(const OPENSSL_STACK *stack) {
	if (SSL_VERBOSE | check_ptr(stack)) debug_printf("OPENSSL_sk_num(...)");
}

void instrument_before_OPENSSL_sk_value(const OPENSSL_STACK *stack, int x) {
	if (SSL_VERBOSE | check_ptr(stack)) debug_printf("OPENSSL_sk_value(...)");
}

void instrument_before_PEM_read_bio_DHparams(BIO *bp, DH **x, pem_password_cb *cb, void *u) {
	if (SSL_VERBOSE | check_ptr(bp) | check_ptr(x) | check_ptr(u)) debug_printf("PEM_read_bio_DHparams(...)");
}

void instrument_before_RAND_seed(const void *buf, int num) {
	if (SSL_VERBOSE | check_ptr(buf)) debug_printf("RAND_seed(...)");
}

void instrument_before_RAND_status(void) {
	if (SSL_VERBOSE) debug_printf("RAND_status(...)");
}

void instrument_before_SSL_accept(SSL *ssl) {
	if (SSL_VERBOSE | check_ptr(ssl)) debug_printf("SSL_accept(...)");
}

void instrument_before_SSL_CIPHER_get_name(const SSL_CIPHER *cipher) {
	// cipher might be a result from previous OpenSSL calls (to rw memory in library)
	if (SSL_VERBOSE/* | check_ptr(cipher)*/) debug_printf("SSL_CIPHER_get_name(...)");
}

void instrument_before_SSL_COMP_get_compression_methods(void) {
	if (SSL_VERBOSE) debug_printf("SSL_COMP_get_compression_methods(...)");
}

void instrument_before_SSL_COMP_get_name(const COMP_METHOD *comp) {
	if (SSL_VERBOSE | check_ptr(comp)) debug_printf("SSL_COMP_get_name(...)");
}

void instrument_before_SSL_connect(SSL *ssl) {
	if (SSL_VERBOSE | check_ptr(ssl)) debug_printf("SSL_connect(...)");
}

void instrument_before_SSL_CTX_ctrl(SSL_CTX *ctx, int cmd, long larg, void *parg) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(parg)) debug_printf("SSL_CTX_ctrl(...)");
}

void instrument_before_SSL_CTX_free(SSL_CTX *ctx) {
	if (SSL_VERBOSE | check_ptr(ctx)) debug_printf("SSL_CTX_free(...)");
}

void instrument_before_SSL_CTX_load_verify_locations(SSL_CTX *ctx, const char *CAfile, const char *CApath) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(CAfile) | check_ptr(CApath)) debug_printf("SSL_CTX_load_verify_locations(...)");
}

void instrument_before_SSL_CTX_new(const SSL_METHOD *method) {
	if (SSL_VERBOSE | check_ptr(method)) debug_printf("SSL_CTX_new(...)");
}

void instrument_before_SSL_CTX_set_cipher_list(SSL_CTX *ctx, const char *str) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(str)) debug_printf("SSL_CTX_set_cipher_list(...)");
}

void instrument_before_SSL_CTX_set_default_verify_paths(SSL_CTX *ctx) {
	if (SSL_VERBOSE | check_ptr(ctx)) debug_printf("SSL_CTX_set_default_verify_paths(...)");
}

void instrument_before_SSL_CTX_set_options(SSL_CTX *ctx, long options) {
	if (SSL_VERBOSE | check_ptr(ctx)) debug_printf("SSL_CTX_set_options(...)");
}

void instrument_before_SSL_CTX_set_verify(SSL_CTX *ctx, int mode, SSL_verify_cb verify_callback) {
	if (SSL_VERBOSE | check_ptr(ctx)) debug_printf("SSL_CTX_set_verify(...)");
}

void instrument_before_SSL_CTX_use_certificate_chain_file(SSL_CTX *ctx, const char *file) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(file)) debug_printf("SSL_CTX_use_certificate_chain_file(...)");
}

void instrument_before_SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file, int type) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(file)) debug_printf("SSL_CTX_use_certificate_file(...)");
}

void instrument_before_SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(file)) debug_printf("SSL_CTX_use_PrivateKey_file(...)");
}

void instrument_before_SSL_free(SSL *ssl) {
	if (SSL_VERBOSE | check_ptr(ssl)) debug_printf("SSL_free(...)");
}

void instrument_before_SSL_get_current_cipher(const SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_get_current_cipher(...)");
}

void instrument_before_SSL_get_current_compression(SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_get_current_compression(...)");
}

void instrument_before_SSL_get_current_expansion(SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_get_current_expansion(...)");
}

void instrument_before_SSL_get_error(const SSL *s, int ret_code) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_get_error(...)");
}

void instrument_before_SSL_get_peer_certificate(const SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_get_peer_certificate(...)");
}

void instrument_before_SSL_get_verify_result(const SSL *ssl) {
	if (SSL_VERBOSE | check_ptr(ssl)) debug_printf("SSL_get_verify_result(...)");
}

void instrument_before_SSL_new(SSL_CTX *ctx) {
	if (SSL_VERBOSE | check_ptr(ctx)) debug_printf("SSL_new(...)");
}

void instrument_before_SSL_pending(const SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_pending(...)");
}

void instrument_before_SSL_read(SSL *ssl, void *buf, int num) {
	if (SSL_VERBOSE | check_ptr(ssl) | check_ptr(buf)) debug_printf("SSL_read(...)");
}

void instrument_before_SSL_set_cipher_list(SSL_CTX *ctx, const char *str) {
	if (SSL_VERBOSE | check_ptr(ctx) | check_ptr(str)) debug_printf("SSL_set_cipher_list(...)");
}

void instrument_before_SSL_set_fd(SSL *s, int fd) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_set_fd(...)");
}

void instrument_before_SSL_shutdown(SSL *s) {
	if (SSL_VERBOSE | check_ptr(s)) debug_printf("SSL_shutdown(...)");
}

void instrument_before_SSL_write(SSL *ssl, const void *buf, int num) {
	if (SSL_VERBOSE | check_ptr(ssl) | check_ptr(buf)) debug_printf("SSL_write(...)");
}

void instrument_before_TLS_client_method(void) {
	if (SSL_VERBOSE) debug_printf("TLS_client_method(...)");
}

void instrument_before_TLS_server_method(void) {
	if (SSL_VERBOSE) debug_printf("TLS_server_method(...)");
}

void instrument_before_X509_EXTENSION_get_object(X509_EXTENSION *ex) {
	if (SSL_VERBOSE | check_ptr(ex)) debug_printf("X509_EXTENSION_get_object(...)");
}

void instrument_before_X509_free(X509 *a) {
	if (SSL_VERBOSE | check_ptr(a)) debug_printf("X509_free(...)");
}

void instrument_before_X509_get_ext(const X509 *x, int loc) {
	if (SSL_VERBOSE | check_ptr(x)) debug_printf("X509_get_ext(...)");
}

void instrument_before_X509_get_ext_count(const X509 *x) {
	if (SSL_VERBOSE | check_ptr(x)) debug_printf("X509_get_ext_count(...)");
}

void instrument_before_X509_get_ext_d2i(const X509 *x, int nid, int *crit, int *idx) {
	if (SSL_VERBOSE | check_ptr(x) | check_ptr(crit) | check_ptr(idx)) debug_printf("X509_get_ext_d2i(...)");
}

void instrument_before_X509_get_issuer_name(const X509 *a) {
	if (SSL_VERBOSE | check_ptr(a)) debug_printf("X509_get_issuer_name(...)");
}

void instrument_before_X509_get_subject_name(const X509 *a) {
	if (SSL_VERBOSE | check_ptr(a)) debug_printf("X509_get_subject_name(...)");
}

void instrument_before_X509_NAME_entry_count(const X509_NAME *name) {
	if (SSL_VERBOSE | check_ptr(name)) debug_printf("X509_NAME_entry_count(...)");
}

void instrument_before_X509_NAME_ENTRY_get_data(const X509_NAME_ENTRY *ne) {
	if (SSL_VERBOSE | check_ptr(ne)) debug_printf("X509_NAME_ENTRY_get_data(...)");
}

void instrument_before_X509_NAME_ENTRY_get_object(const X509_NAME_ENTRY *ne) {
	if (SSL_VERBOSE | check_ptr(ne)) debug_printf("X509_NAME_ENTRY_get_object(...)");
}

void instrument_before_X509_NAME_get_entry(const X509_NAME *name, int loc) {
	if (SSL_VERBOSE | check_ptr(name)) debug_printf("X509_NAME_get_entry(...)");
}

void instrument_before_X509_NAME_get_index_by_NID(X509_NAME *name, int nid, int lastpos) {
	if (SSL_VERBOSE | check_ptr(name)) debug_printf("X509_NAME_get_index_by_NID(...)");
}

void instrument_before_X509_NAME_print_ex(BIO *out, const X509_NAME *nm, int indent, unsigned long flags) {
	if (SSL_VERBOSE | check_ptr(out) | check_ptr(nm)) debug_printf("X509_NAME_print_ex(...)");
}

void instrument_before_X509V3_EXT_get(X509_EXTENSION *ext) {
	if (SSL_VERBOSE | check_ptr(ext)) debug_printf("X509V3_EXT_get(...)");
}


#endif //CALI_LIB_OPENSSL_H
