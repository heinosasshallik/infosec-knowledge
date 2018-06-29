if [ $# -ne 1 ]; 
then 
	echo "Error: Illegal number of arguments"
	echo "Input the directory you wish to audit as an argument"
	echo "The package.json and package-lock.json will be audited in that directory."
	echo "The script will then attempt to find occurrences of these packages in your code."
	exit 1
fi

cd $1

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
