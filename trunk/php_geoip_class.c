/**
 * Project: mod_geoip :: GeoIP php exntension
 * File:    php_geoip_class.c
 *
 * Copyright (c) 2015 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_geoip
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2015 OOPS.org
 * @license     GPL
 * @version     SVN: $Id$
 * @since       File available since release 0.0.1
 */


/*
 * PHP5 Korea IPS Class API
 */

/* {{{ Class API */
#define Z_GEOIP_P(zv) geoip_fetch_object(Z_OBJ_P(zv))

static inline geoip_object * geoip_fetch_object (zend_object * obj) {
	return (geoip_object *) ((char *) obj - XtOffsetOf(geoip_object, std));
}

static int geoip_free_persistent (zend_resource * le, void * ptr) {
	return le->ptr == ptr ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_KEEP;
}

static void geoip_object_free_storage (zend_object * object) {
	geoip_object * intern = (geoip_object *) geoip_fetch_object (object);

	zend_object_std_dtor (&intern->std);

	if ( intern->u.ptr ) {
		if ( intern->u.db->rsrc ) {
			zend_list_delete (intern->u.db->rsrc);
			zend_hash_apply_with_argument (
					&EG(persistent_list),
					(apply_func_arg_t) geoip_free_persistent,
					&intern->u.ptr
			);
		}
	}

	efree(object);
}

static void geoip_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object **retval TSRMLS_DC)
{
	geoip_object * intern;
	zval  * tmp;

	intern = ecalloc (1, sizeof (geoip_object) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	handlers->offset = XtOffsetOf(geoip_object, std);
	handlers->free_obj = (zend_object_free_obj_t) geoip_object_free_storage;
	intern->std.handlers = handlers;

	*retval = &intern->std;
}

static zend_object * geoip_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

	geoip_object_new (class_type, &geoip_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object * geoip_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

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
