<?php
/* Jacob Young 0932387
 * CIS*2750 Assignment 3
 * This program will take the index.wpml files and will then output the interface
 * */

$cmd = "./pageRead index.wpml";
exec($cmd, $output, $return); /* Runs the command to interpret it */

if($return)
{
    echo "Failed to interpret web page";
}
else
{
    foreach($output as $line)
    {
        echo "$line","\n";
    }
}

?>
