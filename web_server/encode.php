
<?php
$content = $_POST['content']; //Get the content posted by the iOS client. 
$filename = $_POST['filename']; //Get the file name posted by the iOS client.
$file = fopen("./".$filename.".dat", "w"); //Create a new file by the name.
fwrite($file, $content); // write the content to the file.
$exe = "./compress ".$filename.".dat"; //execution
//echo $exe.'<br>';
system($exe); 

?>
