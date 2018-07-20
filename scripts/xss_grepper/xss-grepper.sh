# Excluding node-modules directory because I use it in node.js projects as well.
# Yes I know it's ugly and I should have used a for loop.
# At least it works.

if [ $# -ne 1 ]; 
then 
	echo "Error: Illegal number of arguments"
	echo "Input the directory you wish to audit as an argument"
	echo "The package.json and package-lock.json will be audited in that directory."
	echo "The script will then attempt to find occurrences of these packages in your code."
	exit 1
fi

cd $1


grep Window.name -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep Document.referrer -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep document.URL -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep document.documentURI -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep location.href -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep location.search -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep Location.hash -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep Eval -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep setTimeout -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep setInterval -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep Document.write -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep document.writeIn -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep innerHTML -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep outerHTML -r --exclude-dir 'node_modules'
echo ""
echo ""
echo ""
echo ""
echo ""
grep appendChild -r --exclude-dir 'node_modules'