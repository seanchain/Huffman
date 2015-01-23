<?php
$compfilename = $_POST['compfilename']; //Get the name of the compile file name.
$exe = "./decompress ".$compfilename; //Get the execution.
system($exe); //Execute.
 $infilename = $compfilename."decomp.dat"; //decompress file name.
 $infile = fopen($infilename, "r");
 if(!$infile)
 {
     die("Can't find file");
 }
 else{
     while(!feof($infile))
     {
         echo fgetc($infile);
     }     
 }
 /*
 Above read the string of the decompressed file.
 */

 fclose($infile);
?>
