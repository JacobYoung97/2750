<?php

/* Jacob Young 0932387
 * CIS*2750 Assignment 3
 * This program will take the view.wpml file and will then output the interface
 * */

$cmd = "./pageRead view.wpml";
exec($cmd, $output, $return);
$file = fopen("temp.php", "w");

if($return)
{
    echo "Failed to interpret view.php";
}
else
{
    foreach($output as $line) /* Writes the HTML code from view.py to create it and input */
    {
        $txt = "$line"."\n";
        fwrite($file, $txt);
    }
}
fclose($file);
include 'temp.php';
unlink('temp.php');

?>
