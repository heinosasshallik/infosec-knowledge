if [ $# -ne 1 ]; 
then 
	echo "Error: Illegal number of arguments"
	echo "Input the directory you wish to audit as an argument"
	exit 1
fi

cd $1


echo "npm audit results:"
npm audit
echo ""
echo ""
echo ""
echo ""
echo ""
echo ""


number_of_vulnerable_packages=0
prefix="Package"
checked_packages=()
for line in $( npm audit | grep Package);
do
	# We get "Package" and the package name on separate lines for whatever reason.
	if [ $line == $prefix ] || [[ ${checked_packages[*]} =~ $line ]]
	then
		:
	else
		echo "Checking package $line:"
		checked_packages+=($line)
		number_of_vulnerable_packages="$(($number_of_vulnerable_packages+1))"
		grep $line -r --exclude-dir=node_modules --exclude="package-lock.json"
		echo ""
		echo ""
	fi
done
echo "Checked $number_of_vulnerable_packages vulnerable packages"
