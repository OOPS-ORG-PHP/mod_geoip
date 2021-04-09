--TEST--
Check for geoip classfy multiple test 2
--SKIPIF--
<?php
if ( ! extension_loaded ('geoip') ) {
	if ( version_compare(PHP_VERSION, "5.1.0", "<") ) {
		dl ('geoip.so');
		if ( ! extension_loaded ('geoip') )
			print 'skip';
	} else
		print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
if ( version_compare(PHP_VERSION, "5.1.0", "<") )
    dl ('geoip.so');

$searches = array ('cnn.com', 'kornet.net', 'dacom.net');

try {
	/* open conutry database */
	$g = new GeoIP (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);
	$gi = new GeoIP (GEOIP_MEMORY_CACHE|GEOIP_CHECK_CACHE);

	$r = $g->id_by_name ($searches[0]);
	print_r ($r);
	$r = $gi->id_by_name ($searches[1]);
	print_r ($r);
} catch ( GeoIPException $e ) {
	fprintf (STDERR, "%s\n", $e->getMessage ());
	$err = preg_split ('/\r?\n/', $e->getTraceAsString ());
	print_r ($err);
}
?>
--EXPECT--
Array
(
    [country_id] => 225
    [code] => US
    [name] => United States
)
Array
(
    [country_id] => 119
    [code] => KR
    [name] => Korea, Republic of
)
