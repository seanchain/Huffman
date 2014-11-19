<?php
$compfilename = $_POST['compfilename'];
$exe = "./decompress ".$compfilename;
system($exe);
 $infilename = $compfilename."decomp.dat";
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

 fclose($infile);
?>
