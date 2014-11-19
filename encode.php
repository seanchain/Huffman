
<?php
$content = $_POST['content'];
$filename = $_POST['filename'];
$file = fopen("./".$filename.".dat", "w");
fwrite($file, $content);
$exe = "./compress ".$filename.".dat";
//echo $exe.'<br>';
system($exe);

?>
