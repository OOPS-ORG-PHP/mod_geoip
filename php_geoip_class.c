/**
 * Project: mod_geoip :: GeoIP php exntension
 * File:    php_geoip_class.c
 *
 * Copyright (c) 2012 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_geoip
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2012 OOPS.org
 * @license     GPL
 * @version     SVN: $Id$
 * @since       File available since release 0.0.1
 */


/*
 * PHP5 GeoIP IPS Class API
 */

/* {{{ Class API */

static int geoip_free_persistent (zend_rsrc_list_entry * le, void * ptr TSRMLS_DC) {
	return le->ptr == ptr ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_KEEP;
}

static void geoip_object_free_storage (void * object TSRMLS_DC) {
	geoip_object * intern = (geoip_object *) object;

#if PHP_VERSION_ID == 50000
	zend_hash_destroy(intern->std.properties);
	FREE_HASHTABLE(intern->std.properties);
#else
	zend_object_std_dtor (&intern->std TSRMLS_CC);
#endif

	if ( intern->u.ptr ) {
		if ( intern->u.db->rsrc ) {
			zend_list_delete (intern->u.db->rsrc);
			zend_hash_apply_with_argument (
					&EG(persistent_list),
					(apply_func_arg_t) geoip_free_persistent,
					&intern->u.ptr TSRMLS_CC
			);
		}
	}

	efree(object);
}

static void geoip_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object_value *retval TSRMLS_DC)
{
	geoip_object * intern;
	zval  * tmp;

	intern = emalloc (sizeof (geoip_object));
	memset (intern, 0, sizeof (geoip_object));
#if PHP_VERSION_ID == 50000
	intern->std.ce = class_type;

	ALLOC_HASHTABLE(intern->std.properties);
	zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	retval->handle = zend_objects_store_put(
		intern,
		NULL,
		(zend_objects_free_object_storage_t) geoip_object_free_storage,
		NULL TSRMLS_CC
	);
#else
	zend_object_std_init (&intern->std, class_type TSRMLS_CC);
	retval->handle = zend_objects_store_put(
		intern,
		(zend_objects_store_dtor_t) zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) geoip_object_free_storage,
		NULL TSRMLS_CC
	);
#endif
	retval->handlers = handlers;
}

static zend_object_value geoip_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

	geoip_object_new (class_type, &geoip_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object_value geoip_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

	geoip_object_new (class_type, &geoip_object_handlers_exception, &retval TSRMLS_CC);
	return retval;
}
/* Class API }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
