/*
 * Copyright (c) 2016 JoungKyun.Kim <http://oops.org> all rights reserved
 *
 * This file is part of mod_geoip
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU LESSER GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * Project: mod_geoip :: GeoIP php exntension
 * File:    php_geoip_class.c
 *
 * Copyright (c) 2016 JoungKyun.Kim
 *
 * LICENSE: GPL
 *
 * @category    Database
 * @package     mod_geoip
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2016 OOPS.org
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

static void geoip_object_free_storage (zend_object * object) {
	geoip_object * intern;

	GE_PRINT_CALL_API_NAME;

	intern = (geoip_object *) geoip_fetch_object (object);

	zend_object_std_dtor (&intern->std);

	if ( intern->db != NULL && intern->db->rsrc ) {
		zend_list_close (intern->db->rsrc);
	}
}

static void geoip_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object **retval TSRMLS_DC)
{
	geoip_object * intern;
	zval  * tmp;

	GE_PRINT_CALL_API_NAME;

	intern = ecalloc (1, sizeof (geoip_object) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	handlers->offset = XtOffsetOf(geoip_object, std);
	handlers->free_obj = (zend_object_free_obj_t) geoip_object_free_storage;
	intern->std.handlers = handlers;

	*retval = &intern->std;
}

static zend_object * geoip_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

	GE_PRINT_CALL_API_NAME;

	geoip_object_new (class_type, &geoip_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object * geoip_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

	GE_PRINT_CALL_API_NAME;

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
