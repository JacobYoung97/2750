<?php

$sorting = $_POST[sort];
$username = $_POST[user];
$stream = $_POST[streamChoice];
$file = "";
$total = 0;
$next = "Next"; //These are used so that my custom tags can be implemented
$previous = "Previous";
$sort = "Sort";
$mark = "Mark";
$check = "Check";

if($stream != "all")
{
    $table = $stream . "Stream";
    $cmd = "./db 'SELECT COUNT(*) AS POSTS FROM $table'";
    exec($cmd, $output, $return);
    
    if($return)
    {
        echo "Failed to see how many posts";
    }
    else
    {
        $file = "temp.txt";
        $open = fopen($file, "r");

        while(($line = fgets($open)) != false)
        {
            $total = trim($line, "\n");
        }

        fclose($open);
    }
    
    $table = $stream . "StreamUsers";
    $cmd = "./db 'SELECT $table.seen FROM $table WHERE $table.username = '$username''";
    unset($output);
    
    if($return)
    {
        echo "Failed to see where in stream they are!";
    }
    else
    {
        $file = "temp.txt";
        $open = fopen($file, "r");

        while(($line = fgets($open)) != false)
        {
            $num = trim($line, "\n");
        }

        fclose($open);
    }
    
}
/*
else
{
    $query = "SELECT streams.name FROM streams";
    $cmd = "./db '$query'"; //This will get all the stream names
    unset($output);
    $streams = array[];
    $permiss = array[];

    exec($cmd, $output, $return);

    if($return)
    {
        echo "Failed to send query";
    }
    foreach($output as $line) //This will get all stream names
    {
        array_push($streams, "$line");
    }

    foreach($streams as $name) //For every name we see if they are included in it
    {
        $check = $name . "StreamUsers"; //This will check every stream for if they are a part of it
        $query = "SELECT $check.username FROM $check WHERE $check.username = '$username'";
        unset($output);
        $cmd = "./db '$query'";
        exec($cmd, $output, $return);

        foreach($output as $line)
        {
            if(strlen($line) > 1) //They were found in the stream
            {
                array_push($permiss, $name);
            }
        }
    }
    unset($name);
    foreach($name as $theStream) //For every stream they are a part of
    {
        $stream = $theStream . "Stream";
        $query = "SELECT COUNT(*) FROM $stream";
        $cmd = "./db '$query'";
        exec($cmd, $output, $return);

        if($return)
        {
            echo "Failed to query";
        }
        foreach($output as $line)
        {
            $number = trim($line, "\n");
            $total += (int)$number; //For when we find how many posts there are in the file
        }
    }
}
*/
if(empty($sorting))
{
    $sorting = "chrono";
}

if(empty($_POST[inputChoice]))
{
    $inputChoice = "h"; //Placeholder value so nothing actual executes in the view.py program first time through
}
else
{
    $inputChoice = $_POST[inputChoice];
}

if($inputChoice == "Sort")
{
    if($sorting == "chrono")
    {
        $sorting = "name";
    }
    else
    {
        $sorting = "chrono";
    }
}

if(empty($_POST[viewing]) && $stream != "all" && empty($num))
{
    $cmd = "./postNum.py $stream $username";
    unset($output);
    exec($cmd, $output, $return);
    if($return)
    {
        echo "Failed to execute program!";
    }
    foreach($output as $line)
    {
        $num = $line;
    }
}
else if(empty($_POST[viewing]) && $stream == "all")
{
    $num = 1;
}

if($inputChoice == "Check" && $stream != "all")
{
    $table = $stream . "StreamUsers";
    $query = "./db 'SELECT $table.seen FROM $table WHERE $table.username = '$username''";
    unset($output);
    exec($cmd, $output, $return);
    
    if($return)
    {
        echo "Failed to see newest posts";
    }
    else
    {
        $file = "temp.txt";
        $open = fopen($file, "r");

        while(($line = fgets($open)) != false)
        {
            $num = trim($line, "\n");
        }

        fclose($open);
    }
}

if($inputChoice == "Next")
{
    $num += 1;
    if($num > $total && $total > 0)
    {
        $num -= 1; //Cancels out the incrementation
    }
    else if($total == 0)
    {
        $num = 0;
    }
}
else if($inputChoice == "Previous")
{
    $num -= 1;
    if($num <= 0 && $total > 0) //Need to see the first post with this
    {
        $num = 1;
    }
    else if($total == 0)
    {
        $num = 0;
    }
}

?>
