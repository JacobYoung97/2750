<?php

/* Jacob Young 0932387
 * CIS*2750 Assignment 3
 * This program will take the author.wpml file and will then output the interface
 * */

$cmd = "./pageRead author.wpml"; /* Note, in this one, the 'userID' is for the current page and 'user' is for the whole program */
exec($cmd, $author, $return);
$file = fopen("temp.php", "w");

if($return)
{
    echo "Failed to interpret author.php\n";
}
else
{
    foreach($author as $line)
    {
        $txt = "$line"."\n";
        fwrite($file, $txt);
    }
}
fclose($file);
include 'temp.php';
unlink('temp.php');

?>
