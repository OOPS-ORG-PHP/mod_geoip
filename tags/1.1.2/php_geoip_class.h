/*
 * $Id$
 */

#ifndef PHP_GEOIP_CLASS_H
#define PHP_GEOIP_CLASS_H

/* {{{ Exception entry */
const zend_function_entry geoip_methods_exception[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/* Exception declear {{{
 *
 */
#if PHP_MAJOR_VERSION >= 5
#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif

#define GEOIP_REPLACE_ERROR_HANDLING \
	zend_replace_error_handling ( \
		object ? EH_THROW : EH_NORMAL, \
		geoip_ce_exception, \
		&error_handling TSRMLS_CC \
	)

#define GEOIP_RESTORE_ERROR_HANDLING zend_restore_error_handling (&error_handling TSRMLS_CC)
#else
#define GEOIP_REPLACE_ERROR_HANDLIN0 int geoip_error_dummy_handing = 1
#define GEOIP_RESTORE_ERROR_HANDLING geoip_error_dummy_handing = 0
#endif
/* }}} */

/* {{{ geoip_deps[]
 *
 * GEOIP dependancies
 */
const zend_module_dep geoip_deps[] = {
#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
	ZEND_MOD_REQUIRED("spl")
#endif
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ For Class declears */
const zend_function_entry geoip_methods[] = {
	PHP_ME_MAPPING (__construct,          geoip_open,                 NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,                geoip_close,                NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (database_info,        geoip_database_info,        NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (db_avail,             geoip_db_avail,             NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (country_code_by_name, geoip_country_code_by_name, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (country_name_by_name, geoip_country_name_by_name, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (id_by_name,           geoip_id_by_name,           NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (record_by_name,       geoip_record_by_name,       NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (org_by_name,          geoip_org_by_name,          NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

#define REGISTER_GEOIP_CLASS(parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY (ce, "GeoIP", geoip_methods); \
	ce.create_object = geoip_object_new_main; \
	geoip_ce = zend_register_internal_class_ex (&ce, parent, NULL TSRMLS_CC); \
	memcpy(&geoip_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	geoip_object_handlers.clone_obj = NULL; \
	geoip_ce->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}


#define REGISTER_GEOIP_PER_CLASS(name, c_name, parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY(ce, "GeoIP" # name, geoip_methods_ ## c_name); \
	ce.create_object = geoip_object_new_ ## c_name; \
	geoip_ce_ ## c_name = zend_register_internal_class_ex(&ce, parent, NULL TSRMLS_CC); \
	memcpy(&geoip_object_handlers_ ## c_name, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	geoip_object_handlers_ ## c_name.clone_obj = NULL; \
	geoip_ce_ ## c_name->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}

zend_class_entry * geoip_ce;
zend_class_entry * geoip_ce_exception;
static zend_object_handlers geoip_object_handlers;
static zend_object_handlers geoip_object_handlers_exception;

typedef struct _geoip_object {
	zend_object     std;
	union {
		GeoIP_API * db;
		void      * ptr;
	} u;
} geoip_object;

/* For Class declears }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
