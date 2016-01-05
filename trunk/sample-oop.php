#!/usr/bin/php
<?php
if ( ! extension_loaded ('geoip') ) {
	if ( version_compare (PHP_VERSION, '5.4.0', '>=') ) {
		fprintf (STDERR, "GeoIP extension is not loaded\n");
		exit (1);
	} else if ( version_compare (PHP_VERSION, '5.3.0', '>=') )
		dl ('geoip.so');
	else
		dl ('modules/geoip.so');
}

$searches = array ('www.example.com', 'oops.org', 'kornet.net', 'yahoo.com');

/*
 * GeoIP::__construct (database, flag)
 * GeoIP::__construct (database_code, flag)
 * GeoIP::__construct (flag)
 * GeoIP::__construct
 * 
 * open the geoip database
 *
 * argument info:
 *      database => GeoIP datafile path or database code
 *           database code :
 *                  GEOIP_COUNTRY_EDITION    -> default
 *                  GEOIP_ISP_EDITION        -> isp database (if you have)
 *                  GEOIP_CITY_EDITION_REV0  -> city database (if you have)
 *                  GEOIP_CITY_EDITION_REV1  -> city database (if you have)
 *                  GEOIP_ORG_EDITION        -> org database (if you have)
 *
 *      flag     => GEOIP_STANDARD
 *                  GEOIP_MEMORY_CACHE
 *                  GEOIP_CHECK_CACHE
 *                  GEOIP_INDEX_CACHE
 *
 * if flag is not specifyed, default "GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE"
 *
 * if failed, return FALSE
 *
 */

try {
	/* open conutry database */
	$g = new GeoIP (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);
	/* open city database */
	if ( GeoIP_db_avail (GEOIP_CITY_EDITION_REV0) )
		$gc = new GeoIP (GEOIP_CITY_EDITION_REV0, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);
	/* open isp database */
	if ( GeoIP_db_avail (GEOIP_ISP_EDITION) )
		$gi = new GeoIP (GEOIP_ISP_EDITION, GEOIP_INDEX_CACHE|GEOIP_CHECK_CACHE);

	#echo "TYPE: " . $g->database_info () ."\n";

	foreach ( $searches as $v ) {
		/*
		 * search geoip country database
		 *
		 * array GeoIP::id_by_name (geoip handle, host);
		 * return:
		 *      array (
		 *              country_id,
		 *              code,        // code of nation
		 *              name         // name of nation
		 *            );
		 *
		 * string GeoIP::country_code_by_name (host)
		 * string GeoIP::country_code_by_name (host)
		 *
		 */
		$r = $g->id_by_name ($v);
		print_r ($r);

		/* print city information
		 *
		 * array GeoIP::record_by_name (host)
		 * return:
		 *      array (
		 *              country_code,  // country 2 digit code
		 *              region,        // states or do (korean)
		 *              city,          // name of city
		 *              postal_code,   // postal code or post code
		 *              latitude,
		 *              longitude,
		 *              if ( DB is GEOIP_CITY_EDITION_REV1 ) {
		 *                  dma_code,
		 *                  area_code,
		 *              }
		 *            );
		 */
		if ( GeoIP_db_avail (GEOIP_CITY_EDITION_REV0) ) {
			$rc = $gc->record_by_name ($v);
			print_r ($rc);
		}

		/* print isp information
		 *
		 * string GeoIP::org_by_name (host)
		 */
		if ( GeoIP_db_avail (GEOIP_ISP_EDITION) ) {
			$ri = $gi->org_by_name ($v);
			echo "ISP NAME: $ri\n";
		}

		#echo "### " . $g->country_code_by_name ($v) . "\n";
		#echo "### " . $g->country_name_by_name ($v) . "\n";
	}
} catch ( GeoIPException $e ) {
	fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
	print_r ($err);
}


?>
