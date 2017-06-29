<?php

/* Jacob Young 0932387
 * CIS*2750 Assignment 3
 * This program will take the streams.wpml file and will then output the interface
 * */

$cmd = "./pageRead streams.wpml";
exec($cmd, $stream, $return);
$file = fopen("temp.php", "w");

if($return)
{
    echo "Failed to interpret streams.php\n";
}
else
{
    foreach($stream as $line)
    {
        $txt = "$line"."\n";
        fwrite($file, $txt);
    }
}
fclose($file);
include 'temp.php';
unlink('temp.php');

?>
