<?php
	$cs="KDQjW6Xn7UG1MjC0";
	$ss="Pld7wb3hDkwtNagNdPFUR8";
	$sk="";
	$gs=$cs.$ss.$sk;
	$schars=array("a","b","c","d","e","f","g","h","i","k",
		"j","l","m","n","o","p","q","r","s","t","u","v","w","x","y",
		"z","0","1","2","3","4","5","6","7","8","9");
	$gs2=0;
	for ($i=0; $i<strlen($gs); $i++) {
		$c=substr($gs,$i,1);
		$n=intval(array_search(strtolower($c),$schars));
		$gs2=ctype_upper($c)?($gs2+($n*strlen($gs))):($gs2+$n);
	}
	srand($gs2);
	//echo number_format((rand(0,10000)/100), 0, '.', '');
	$version = phpversion();
	srand(1000000);
	echo rand();
	srand(1000003);
	echo "\n";
	echo rand();
	
?>
